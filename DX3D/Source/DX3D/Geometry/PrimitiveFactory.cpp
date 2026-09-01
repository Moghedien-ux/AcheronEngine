#include <DX3D/Geometry/PrimitiveFactory.h>
#include <cmath>

using namespace dx3d;

MeshPtr PrimitiveFactory::CreateCube(const Vec3& size, const Vec4& color)
{
    auto mesh = std::make_shared<Mesh>("Primitive_Cube");
    Vec3 h = size * 0.5f;

    std::vector<Vertex> v = {
        // Front Face (+Z)
        { { -h.x, -h.y,  h.z }, {  0.0f,  0.0f,  1.0f }, { 1, 0, 0 }, { 0, 1, 0 }, { 0.0f, 1.0f }, color },
        { { -h.x,  h.y,  h.z }, {  0.0f,  0.0f,  1.0f }, { 1, 0, 0 }, { 0, 1, 0 }, { 0.0f, 0.0f }, color },
        { {  h.x,  h.y,  h.z }, {  0.0f,  0.0f,  1.0f }, { 1, 0, 0 }, { 0, 1, 0 }, { 1.0f, 0.0f }, color },
        { {  h.x, -h.y,  h.z }, {  0.0f,  0.0f,  1.0f }, { 1, 0, 0 }, { 0, 1, 0 }, { 1.0f, 1.0f }, color },

        // Back Face (-Z)
        { {  h.x, -h.y, -h.z }, {  0.0f,  0.0f, -1.0f }, { -1, 0, 0 }, { 0, 1, 0 }, { 0.0f, 1.0f }, color },
        { {  h.x,  h.y, -h.z }, {  0.0f,  0.0f, -1.0f }, { -1, 0, 0 }, { 0, 1, 0 }, { 0.0f, 0.0f }, color },
        { { -h.x,  h.y, -h.z }, {  0.0f,  0.0f, -1.0f }, { -1, 0, 0 }, { 0, 1, 0 }, { 1.0f, 0.0f }, color },
        { { -h.x, -h.y, -h.z }, {  0.0f,  0.0f, -1.0f }, { -1, 0, 0 }, { 0, 1, 0 }, { 1.0f, 1.0f }, color },

        // Top Face (+Y)
        { { -h.x,  h.y,  h.z }, {  0.0f,  1.0f,  0.0f }, { 1, 0, 0 }, { 0, 0, -1 }, { 0.0f, 1.0f }, color },
        { { -h.x,  h.y, -h.z }, {  0.0f,  1.0f,  0.0f }, { 1, 0, 0 }, { 0, 0, -1 }, { 0.0f, 0.0f }, color },
        { {  h.x,  h.y, -h.z }, {  0.0f,  1.0f,  0.0f }, { 1, 0, 0 }, { 0, 0, -1 }, { 1.0f, 0.0f }, color },
        { {  h.x,  h.y,  h.z }, {  0.0f,  1.0f,  0.0f }, { 1, 0, 0 }, { 0, 0, -1 }, { 1.0f, 1.0f }, color },

        // Bottom Face (-Y)
        { { -h.x, -h.y, -h.z }, {  0.0f, -1.0f,  0.0f }, { 1, 0, 0 }, { 0, 0, 1 }, { 0.0f, 1.0f }, color },
        { { -h.x, -h.y,  h.z }, {  0.0f, -1.0f,  0.0f }, { 1, 0, 0 }, { 0, 0, 1 }, { 0.0f, 0.0f }, color },
        { {  h.x, -h.y,  h.z }, {  0.0f, -1.0f,  0.0f }, { 1, 0, 0 }, { 0, 0, 1 }, { 1.0f, 0.0f }, color },
        { {  h.x, -h.y, -h.z }, {  0.0f, -1.0f,  0.0f }, { 1, 0, 0 }, { 0, 0, 1 }, { 1.0f, 1.0f }, color },

        // Right Face (+X)
        { {  h.x, -h.y,  h.z }, {  1.0f,  0.0f,  0.0f }, { 0, 0, -1 }, { 0, 1, 0 }, { 0.0f, 1.0f }, color },
        { {  h.x,  h.y,  h.z }, {  1.0f,  0.0f,  0.0f }, { 0, 0, -1 }, { 0, 1, 0 }, { 0.0f, 0.0f }, color },
        { {  h.x,  h.y, -h.z }, {  1.0f,  0.0f,  0.0f }, { 0, 0, -1 }, { 0, 1, 0 }, { 1.0f, 0.0f }, color },
        { {  h.x, -h.y, -h.z }, {  1.0f,  0.0f,  0.0f }, { 0, 0, -1 }, { 0, 1, 0 }, { 1.0f, 1.0f }, color },

        // Left Face (-X)
        { { -h.x, -h.y, -h.z }, { -1.0f,  0.0f,  0.0f }, { 0, 0, 1 }, { 0, 1, 0 }, { 0.0f, 1.0f }, color },
        { { -h.x,  h.y, -h.z }, { -1.0f,  0.0f,  0.0f }, { 0, 0, 1 }, { 0, 1, 0 }, { 0.0f, 0.0f }, color },
        { { -h.x,  h.y,  h.z }, { -1.0f,  0.0f,  0.0f }, { 0, 0, 1 }, { 0, 1, 0 }, { 1.0f, 0.0f }, color },
        { { -h.x, -h.y,  h.z }, { -1.0f,  0.0f,  0.0f }, { 0, 0, 1 }, { 0, 1, 0 }, { 1.0f, 1.0f }, color },
    };

    std::vector<u32> idx = {
        0, 1, 2, 0, 2, 3,       // Front
        4, 5, 6, 4, 6, 7,       // Back
        8, 9, 10, 8, 10, 11,    // Top
        12, 13, 14, 12, 14, 15, // Bottom
        16, 17, 18, 16, 18, 19, // Right
        20, 21, 22, 20, 22, 23  // Left
    };

    mesh->setVertices(v);
    mesh->setIndices(idx);
    mesh->calculateTangents();
    return mesh;
}

MeshPtr PrimitiveFactory::CreateSphere(f32 radius, u32 slices, u32 stacks, const Vec4& color)
{
    auto mesh = std::make_shared<Mesh>("Primitive_Sphere");
    std::vector<Vertex> vertices;
    std::vector<u32> indices;

    constexpr f32 pi = 3.1415926535f;

    for (u32 stack = 0; stack <= stacks; ++stack)
    {
        f32 phi = stack * (pi / stacks);
        f32 sinPhi = std::sin(phi);
        f32 cosPhi = std::cos(phi);

        for (u32 slice = 0; slice <= slices; ++slice)
        {
            f32 theta = slice * (2.0f * pi / slices);
            f32 sinTheta = std::sin(theta);
            f32 cosTheta = std::cos(theta);

            Vec3 normal(sinPhi * cosTheta, cosPhi, sinPhi * sinTheta);
            Vec3 pos = normal * radius;
            Vec2 uv(static_cast<f32>(slice) / slices, static_cast<f32>(stack) / stacks);

            Vertex vert;
            vert.position = pos;
            vert.normal = normal;
            vert.texCoord = uv;
            vert.color = color;
            vertices.push_back(vert);
        }
    }

    for (u32 stack = 0; stack < stacks; ++stack)
    {
        for (u32 slice = 0; slice < slices; ++slice)
        {
            u32 first = stack * (slices + 1) + slice;
            u32 second = first + slices + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    mesh->calculateTangents();
    return mesh;
}

MeshPtr PrimitiveFactory::CreateCylinder(f32 radius, f32 height, u32 slices, const Vec4& color)
{
    auto mesh = std::make_shared<Mesh>("Primitive_Cylinder");
    std::vector<Vertex> vertices;
    std::vector<u32> indices;

    constexpr f32 pi = 3.1415926535f;
    f32 halfH = height * 0.5f;

    // Body vertices
    for (u32 i = 0; i <= slices; ++i)
    {
        f32 theta = i * (2.0f * pi / slices);
        f32 cosT = std::cos(theta);
        f32 sinT = std::sin(theta);

        Vec3 norm(cosT, 0.0f, sinT);
        f32 u = static_cast<f32>(i) / slices;

        // Top edge
        vertices.push_back({ { radius * cosT,  halfH, radius * sinT }, norm, {}, {}, { u, 0.0f }, color });
        // Bottom edge
        vertices.push_back({ { radius * cosT, -halfH, radius * sinT }, norm, {}, {}, { u, 1.0f }, color });
    }

    for (u32 i = 0; i < slices; ++i)
    {
        u32 i0 = i * 2;
        u32 i1 = i0 + 1;
        u32 i2 = i0 + 2;
        u32 i3 = i0 + 3;

        indices.push_back(i0);
        indices.push_back(i1);
        indices.push_back(i2);

        indices.push_back(i2);
        indices.push_back(i1);
        indices.push_back(i3);
    }

    // Top Cap
    u32 topCenterIdx = static_cast<u32>(vertices.size());
    vertices.push_back({ { 0.0f, halfH, 0.0f }, { 0.0f, 1.0f, 0.0f }, {}, {}, { 0.5f, 0.5f }, color });

    for (u32 i = 0; i <= slices; ++i)
    {
        f32 theta = i * (2.0f * pi / slices);
        vertices.push_back({ { radius * std::cos(theta), halfH, radius * std::sin(theta) },
                             { 0.0f, 1.0f, 0.0f }, {}, {},
                             { 0.5f + 0.5f * std::cos(theta), 0.5f + 0.5f * std::sin(theta) }, color });
    }
    for (u32 i = 0; i < slices; ++i)
    {
        indices.push_back(topCenterIdx);
        indices.push_back(topCenterIdx + 1 + i);
        indices.push_back(topCenterIdx + 2 + i);
    }

    // Bottom Cap
    u32 botCenterIdx = static_cast<u32>(vertices.size());
    vertices.push_back({ { 0.0f, -halfH, 0.0f }, { 0.0f, -1.0f, 0.0f }, {}, {}, { 0.5f, 0.5f }, color });

    for (u32 i = 0; i <= slices; ++i)
    {
        f32 theta = i * (2.0f * pi / slices);
        vertices.push_back({ { radius * std::cos(theta), -halfH, radius * std::sin(theta) },
                             { 0.0f, -1.0f, 0.0f }, {}, {},
                             { 0.5f + 0.5f * std::cos(theta), 0.5f + 0.5f * std::sin(theta) }, color });
    }
    for (u32 i = 0; i < slices; ++i)
    {
        indices.push_back(botCenterIdx);
        indices.push_back(botCenterIdx + 2 + i);
        indices.push_back(botCenterIdx + 1 + i);
    }

    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    mesh->calculateTangents();
    return mesh;
}

MeshPtr PrimitiveFactory::CreateCapsule(f32 radius, f32 height, u32 slices, u32 stacks, const Vec4& color)
{
    auto mesh = std::make_shared<Mesh>("Primitive_Capsule");
    std::vector<Vertex> vertices;
    std::vector<u32> indices;

    constexpr f32 pi = 3.1415926535f;
    f32 cylinderHalfH = height * 0.5f;

    // Top Hemisphere
    for (u32 stack = 0; stack <= stacks; ++stack)
    {
        f32 phi = stack * (pi * 0.5f / stacks);
        f32 sinPhi = std::sin(phi);
        f32 cosPhi = std::cos(phi);

        for (u32 slice = 0; slice <= slices; ++slice)
        {
            f32 theta = slice * (2.0f * pi / slices);
            Vec3 norm(sinPhi * std::cos(theta), cosPhi, sinPhi * std::sin(theta));
            Vec3 pos = norm * radius + Vec3(0.0f, cylinderHalfH, 0.0f);

            vertices.push_back({ pos, norm, {}, {}, { static_cast<f32>(slice) / slices, static_cast<f32>(stack) / (stacks * 2) }, color });
        }
    }

    // Bottom Hemisphere
    for (u32 stack = 0; stack <= stacks; ++stack)
    {
        f32 phi = pi * 0.5f + stack * (pi * 0.5f / stacks);
        f32 sinPhi = std::sin(phi);
        f32 cosPhi = std::cos(phi);

        for (u32 slice = 0; slice <= slices; ++slice)
        {
            f32 theta = slice * (2.0f * pi / slices);
            Vec3 norm(sinPhi * std::cos(theta), cosPhi, sinPhi * std::sin(theta));
            Vec3 pos = norm * radius - Vec3(0.0f, cylinderHalfH, 0.0f);

            vertices.push_back({ pos, norm, {}, {}, { static_cast<f32>(slice) / slices, 0.5f + static_cast<f32>(stack) / (stacks * 2) }, color });
        }
    }

    u32 totalStacks = stacks * 2;
    for (u32 stack = 0; stack < totalStacks; ++stack)
    {
        for (u32 slice = 0; slice < slices; ++slice)
        {
            u32 first = stack * (slices + 1) + slice;
            u32 second = first + slices + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    mesh->calculateTangents();
    return mesh;
}

MeshPtr PrimitiveFactory::CreatePlane(f32 width, f32 depth, u32 subDivX, u32 subDivZ, const Vec4& color)
{
    auto mesh = std::make_shared<Mesh>("Primitive_Plane");
    std::vector<Vertex> vertices;
    std::vector<u32> indices;

    f32 halfW = width * 0.5f;
    f32 halfD = depth * 0.5f;
    f32 dx = width / subDivX;
    f32 dz = depth / subDivZ;

    for (u32 z = 0; z <= subDivZ; ++z)
    {
        f32 posZ = -halfD + z * dz;
        for (u32 x = 0; x <= subDivX; ++x)
        {
            f32 posX = -halfW + x * dx;
            Vec2 uv(static_cast<f32>(x) / subDivX, static_cast<f32>(z) / subDivZ);
            vertices.push_back({ { posX, 0.0f, posZ }, { 0.0f, 1.0f, 0.0f }, { 1, 0, 0 }, { 0, 0, 1 }, uv, color });
        }
    }

    for (u32 z = 0; z < subDivZ; ++z)
    {
        for (u32 x = 0; x < subDivX; ++x)
        {
            u32 row1 = z * (subDivX + 1);
            u32 row2 = (z + 1) * (subDivX + 1);

            indices.push_back(row1 + x);
            indices.push_back(row2 + x);
            indices.push_back(row1 + x + 1);

            indices.push_back(row1 + x + 1);
            indices.push_back(row2 + x);
            indices.push_back(row2 + x + 1);
        }
    }

    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    return mesh;
}

MeshPtr PrimitiveFactory::CreateTorus(f32 outerRadius, f32 innerRadius, u32 segments, u32 tubeSegments, const Vec4& color)
{
    auto mesh = std::make_shared<Mesh>("Primitive_Torus");
    std::vector<Vertex> vertices;
    std::vector<u32> indices;

    constexpr f32 pi = 3.1415926535f;

    for (u32 i = 0; i <= segments; ++i)
    {
        f32 u = i * (2.0f * pi / segments);
        f32 cosU = std::cos(u);
        f32 sinU = std::sin(u);

        for (u32 j = 0; j <= tubeSegments; ++j)
        {
            f32 v = j * (2.0f * pi / tubeSegments);
            f32 cosV = std::cos(v);
            f32 sinV = std::sin(v);

            f32 x = (outerRadius + innerRadius * cosV) * cosU;
            f32 y = innerRadius * sinV;
            f32 z = (outerRadius + innerRadius * cosV) * sinU;

            Vec3 norm(cosV * cosU, sinV, cosV * sinU);
            Vec2 uv(static_cast<f32>(i) / segments, static_cast<f32>(j) / tubeSegments);

            vertices.push_back({ { x, y, z }, norm, {}, {}, uv, color });
        }
    }

    for (u32 i = 0; i < segments; ++i)
    {
        for (u32 j = 0; j < tubeSegments; ++j)
        {
            u32 first = i * (tubeSegments + 1) + j;
            u32 second = first + tubeSegments + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    mesh->calculateTangents();
    return mesh;
}

MeshPtr PrimitiveFactory::CreateCone(f32 radius, f32 height, u32 slices, const Vec4& color)
{
    auto mesh = std::make_shared<Mesh>("Primitive_Cone");
    std::vector<Vertex> vertices;
    std::vector<u32> indices;

    constexpr f32 pi = 3.1415926535f;
    f32 halfH = height * 0.5f;

    // Apex vertex
    u32 apexIdx = 0;
    vertices.push_back({ { 0.0f, halfH, 0.0f }, { 0.0f, 1.0f, 0.0f }, {}, {}, { 0.5f, 0.0f }, color });

    for (u32 i = 0; i <= slices; ++i)
    {
        f32 theta = i * (2.0f * pi / slices);
        f32 cosT = std::cos(theta);
        f32 sinT = std::sin(theta);

        Vec3 pos(radius * cosT, -halfH, radius * sinT);
        Vec3 norm = Vec3(cosT, radius / height, sinT).normalized();
        Vec2 uv(static_cast<f32>(i) / slices, 1.0f);

        vertices.push_back({ pos, norm, {}, {}, uv, color });
    }

    for (u32 i = 1; i <= slices; ++i)
    {
        indices.push_back(apexIdx);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    // Base Cap
    u32 baseCenterIdx = static_cast<u32>(vertices.size());
    vertices.push_back({ { 0.0f, -halfH, 0.0f }, { 0.0f, -1.0f, 0.0f }, {}, {}, { 0.5f, 0.5f }, color });

    for (u32 i = 0; i <= slices; ++i)
    {
        f32 theta = i * (2.0f * pi / slices);
        vertices.push_back({ { radius * std::cos(theta), -halfH, radius * std::sin(theta) },
                             { 0.0f, -1.0f, 0.0f }, {}, {},
                             { 0.5f + 0.5f * std::cos(theta), 0.5f + 0.5f * std::sin(theta) }, color });
    }

    for (u32 i = 0; i < slices; ++i)
    {
        indices.push_back(baseCenterIdx);
        indices.push_back(baseCenterIdx + 2 + i);
        indices.push_back(baseCenterIdx + 1 + i);
    }

    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    mesh->calculateTangents();
    return mesh;
}