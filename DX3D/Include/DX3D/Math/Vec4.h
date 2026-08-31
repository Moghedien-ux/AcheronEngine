#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Math/Vec3.h>

namespace dx3d
{
    struct Vec4
    {
        f32 x{ 0.0f };
        f32 y{ 0.0f };
        f32 z{ 0.0f };
        f32 w{ 1.0f };

        Vec4() = default;
        Vec4(f32 _x, f32 _y, f32 _z, f32 _w = 1.0f) : x(_x), y(_y), z(_z), w(_w) {}
        Vec4(const Vec3& v, f32 _w = 1.0f) : x(v.x), y(v.y), z(v.z), w(_w) {}
    };
}
