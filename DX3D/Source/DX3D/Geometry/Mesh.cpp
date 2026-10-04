#include <DX3D/Geometry/Mesh.h>
#include <DX3D/Graphics/GraphicsDevice.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Graphics/IndexBuffer.h>
#include <algorithm>

using namespace dx3d;

Mesh::Mesh(const std::string& name) : m_name(name) {}

void Mesh::setVertices(const std::vector<Vertex>& vertices)
{
    m_vertices = vertices;
    calculateBounds();
}

void Mesh::setIndices(const std::vector<u32>& indices)
{
    m_indices = indices;
    if (m_subMeshes.empty() && !indices.empty())
    {
        SubMesh sm;
        sm.name = m_name + "_Default";
        sm.indexStart = 0;
        sm.indexCount = static_cast<u32>(indices.size());
        sm.materialIndex = 0;
        sm.bounds = m_bounds;
        m_subMeshes.push_back(sm);
    }
}

void Mesh::addSubMesh(const SubMesh& subMesh)
{
    m_subMeshes.push_back(subMesh);
}

void Mesh::calculateBounds()
{
    m_bounds = AABB{};
    for (const auto& v : m_vertices)
    {
        m_bounds.minBounds.x = (std::min)(m_bounds.minBounds.x, v.position.x);
        m_bounds.minBounds.y = (std::min)(m_bounds.minBounds.y, v.position.y);
        m_bounds.minBounds.z = (std::min)(m_bounds.minBounds.z, v.position.z);

        m_bounds.maxBounds.x = (std::max)(m_bounds.maxBounds.x, v.position.x);
        m_bounds.maxBounds.y = (std::max)(m_bounds.maxBounds.y, v.position.y);
        m_bounds.maxBounds.z = (std::max)(m_bounds.maxBounds.z, v.position.z);
    }
}

void Mesh::calculateNormals()
{
    for (auto& v : m_vertices)
    {
        v.normal = Vec3{ 0.0f, 0.0f, 0.0f };
    }

    for (size_t i = 0; i + 2 < m_indices.size(); i += 3)
    {
        u32 i0 = m_indices[i];
        u32 i1 = m_indices[i + 1];
        u32 i2 = m_indices[i + 2];

        Vec3 p0 = m_vertices[i0].position;
        Vec3 p1 = m_vertices[i1].position;
        Vec3 p2 = m_vertices[i2].position;

        Vec3 edge1 = p1 - p0;
        Vec3 edge2 = p2 - p0;
        Vec3 faceNormal = Vec3::Cross(edge1, edge2);

        m_vertices[i0].normal += faceNormal;
        m_vertices[i1].normal += faceNormal;
        m_vertices[i2].normal += faceNormal;
    }

    for (auto& v : m_vertices)
    {
        v.normal = v.normal.normalized();
    }
}

void Mesh::calculateTangents()
{
    for (auto& v : m_vertices)
    {
        v.tangent = Vec3{ 0.0f, 0.0f, 0.0f };
        v.bitangent = Vec3{ 0.0f, 0.0f, 0.0f };
    }

    for (size_t i = 0; i + 2 < m_indices.size(); i += 3)
    {
        u32 i0 = m_indices[i];
        u32 i1 = m_indices[i + 1];
        u32 i2 = m_indices[i + 2];

        auto& v0 = m_vertices[i0];
        auto& v1 = m_vertices[i1];
        auto& v2 = m_vertices[i2];

        Vec3 edge1 = v1.position - v0.position;
        Vec3 edge2 = v2.position - v0.position;

        f32 deltaU1 = v1.texCoord.u - v0.texCoord.u;
        f32 deltaV1 = v1.texCoord.v - v0.texCoord.v;
        f32 deltaU2 = v2.texCoord.u - v0.texCoord.u;
        f32 deltaV2 = v2.texCoord.v - v0.texCoord.v;

        f32 f = (deltaU1 * deltaV2 - deltaU2 * deltaV1);
        if (std::abs(f) > 1e-6f)
        {
            f32 r = 1.0f / f;
            Vec3 tangent = (edge1 * deltaV2 - edge2 * deltaV1) * r;
            Vec3 bitangent = (edge2 * deltaU1 - edge1 * deltaU2) * r;

            v0.tangent += tangent;
            v1.tangent += tangent;
            v2.tangent += tangent;

            v0.bitangent += bitangent;
            v1.bitangent += bitangent;
            v2.bitangent += bitangent;
        }
    }

    for (auto& v : m_vertices)
    {
        Vec3 n = v.normal;
        Vec3 t = v.tangent;

        v.tangent = (t - n * Vec3::Dot(n, t)).normalized();
        v.bitangent = Vec3::Cross(n, v.tangent).normalized();
    }
}

bool Mesh::uploadToGPU(const GraphicsResourceDesc& gResource)
{
    if (m_vertices.empty() || m_indices.empty()) return false;

    VertexBufferDesc vbDesc{
        gResource,
        m_vertices.data(),
        static_cast<u32>(sizeof(Vertex)),
        static_cast<u32>(m_vertices.size())
    };
    m_vertexBuffer = std::make_shared<VertexBuffer>(vbDesc);

    IndexBufferDesc ibDesc{
        gResource,
        m_indices.data(),
        static_cast<u32>(m_indices.size())
    };
    m_indexBuffer = std::make_shared<IndexBuffer>(ibDesc);

    return m_vertexBuffer != nullptr && m_indexBuffer != nullptr;
}

bool Mesh::uploadToGPU(GraphicsDevice& device, Logger& logger)
{
    GraphicsResourceDesc gRes{ { logger }, device.getD3DDevice() };
    return uploadToGPU(gRes);
}

void Mesh::setBuffers(VertexBufferPtr vertexBuffer, IndexBufferPtr indexBuffer)
{
    m_vertexBuffer = vertexBuffer;
    m_indexBuffer = indexBuffer;
}

void Mesh::draw(DeviceContext& context) const
{
    if (!m_vertexBuffer || !m_indexBuffer) return;

    context.setVertexBuffer(*m_vertexBuffer);
    context.setIndexBuffer(*m_indexBuffer);

    if (m_subMeshes.empty())
    {
        context.drawIndexed(m_indexBuffer->getIndexCount());
    }
    else
    {
        for (const auto& sm : m_subMeshes)
        {
            context.drawIndexed(sm.indexCount, sm.indexStart, 0);
        }
    }
}