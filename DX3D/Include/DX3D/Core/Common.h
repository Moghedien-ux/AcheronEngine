#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Math/Vec2.h>
#include <DX3D/Math/Vec3.h>
#include <DX3D/Math/Vec4.h>
#include <DX3D/Math/Mat4.h>
#include <DX3D/Geometry/Vertex.h>

namespace dx3d
{
    struct ConstantData
    {
        Mat4 world{};
        Mat4 view{};
        Mat4 proj{};
        Vec4 customColor{ 1.0f, 1.0f, 1.0f, 1.0f };
        Vec4 highlightParams{ 0.0f, 0.4f, 0.0f, 0.0f }; // x: isHighlighted, y: roughness, z: metallic, w: wireframe
    };

    enum class PrimitiveType
    {
        TriangleList,
        TriangleStrip,
        LineList,
        LineStrip,
        PointList
    };

    enum class CullMode
    {
        None,
        Front,
        Back
    };
}