#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Geometry/Vertex.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Graphics/IndexBuffer.h>
#include <vector>
#include <memory>
#include <string>

namespace dx3d
{
    class GraphicsDevice;
    class DeviceContext;
    class VertexBuffer;
    class IndexBuffer;
    class Logger;
    struct GraphicsResourceDesc;

    using VertexBufferPtr = std::shared_ptr<VertexBuffer>;
    using IndexBufferPtr = std::shared_ptr<IndexBuffer>;

    struct AABB
    {
        Vec3 minBounds{ 1e9f, 1e9f, 1e9f };
        Vec3 maxBounds{ -1e9f, -1e9f, -1e9f };

        Vec3 getCenter() const { return (minBounds + maxBounds) * 0.5f; }
        Vec3 getExtents() const { return (maxBounds - minBounds) * 0.5f; }
        Vec3 getSize() const { return maxBounds - minBounds; }
    };

    struct SubMesh
    {
        std::string name{ "SubMesh" };
        u32 indexStart{ 0 };
        u32 indexCount{ 0 };
        u32 materialIndex{ 0 };
        AABB bounds{};
    };

    class Mesh
    {
    public:
        explicit Mesh(const std::string& name = "Mesh");
        ~Mesh() = default;

        void setVertices(const std::vector<Vertex>& vertices);
        void setIndices(const std::vector<u32>& indices);
        void addSubMesh(const SubMesh& subMesh);

        const std::vector<Vertex>& getVertices() const { return m_vertices; }
        const std::vector<u32>& getIndices() const { return m_indices; }
        const std::vector<SubMesh>& getSubMeshes() const { return m_subMeshes; }
        const AABB& getBounds() const { return m_bounds; }
        const std::string& getName() const { return m_name; }

        void calculateNormals();
        void calculateTangents();
        void calculateBounds();

        bool uploadToGPU(const GraphicsResourceDesc& gResource);
        bool uploadToGPU(GraphicsDevice& device, Logger& logger);
        void setBuffers(VertexBufferPtr vertexBuffer, IndexBufferPtr indexBuffer);

        void draw(DeviceContext& context) const;
        bool isUploaded() const { return m_vertexBuffer != nullptr && m_indexBuffer != nullptr; }

        VertexBufferPtr getVertexBuffer() const { return m_vertexBuffer; }
        IndexBufferPtr getIndexBuffer() const { return m_indexBuffer; }

    private:
        std::string m_name;
        std::vector<Vertex> m_vertices;
        std::vector<u32> m_indices;
        std::vector<SubMesh> m_subMeshes;
        AABB m_bounds{};

        VertexBufferPtr m_vertexBuffer{};
        IndexBufferPtr m_indexBuffer{};
    };

    using MeshPtr = std::shared_ptr<Mesh>;
}