#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Math/Vec2.h>
#include <DX3D/Math/Vec3.h>
#include <DX3D/Math/Vec4.h>

namespace dx3d
{
    struct Vertex
    {
        Vec3 position{ 0.0f, 0.0f, 0.0f };   // Offset 0 (12 bytes)
        Vec3 normal{ 0.0f, 1.0f, 0.0f };     // Offset 12 (12 bytes)
        Vec3 tangent{ 1.0f, 0.0f, 0.0f };    // Offset 24 (12 bytes)
        Vec3 bitangent{ 0.0f, 0.0f, 1.0f };  // Offset 36 (12 bytes)
        Vec2 texCoord{ 0.0f, 0.0f };         // Offset 48 (8 bytes)
        Vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };// Offset 56 (16 bytes)
        // Total Stride = 72 bytes
    };
}