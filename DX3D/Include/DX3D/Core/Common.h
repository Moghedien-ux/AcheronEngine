#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Math/Vec3.h>
#include <DX3D/Math/Vec4.h>
#include <DX3D/Math/Mat4.h>

namespace dx3d
{
    struct Vertex
    {
        Vec3 position;
        Vec3 normal;
        Vec3 color;
    };

    struct ConstantData
    {
        Mat4 world;
        Mat4 view;
        Mat4 proj;
        Vec4 customColor;
        Vec4 highlightParams; // x: isHighlighted (1.0 or 0.0), y: custom pulse
    };
}
