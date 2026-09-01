#pragma once
#include <DX3D/Core/Core.h>
#include <cmath>

namespace dx3d
{
    struct Vec2
    {
        f32 u{ 0.0f };
        f32 v{ 0.0f };

        Vec2() = default;
        Vec2(f32 _u, f32 _v) : u(_u), v(_v) {}

        Vec2 operator+(const Vec2& o) const { return { u + o.u, v + o.v }; }
        Vec2 operator-(const Vec2& o) const { return { u - o.u, v - o.v }; }
        Vec2 operator*(f32 s) const { return { u * s, v * s }; }
        Vec2 operator/(f32 s) const { return { u / s, v / s }; }

        Vec2& operator+=(const Vec2& o) { u += o.u; v += o.v; return *this; }
        Vec2& operator-=(const Vec2& o) { u -= o.u; v -= o.v; return *this; }
        Vec2& operator*=(f32 s) { u *= s; v *= s; return *this; }

        f32 lengthSq() const { return u * u + v * v; }
        f32 length() const { return std::sqrt(lengthSq()); }

        Vec2 normalized() const
        {
            f32 len = length();
            return len > 1e-6f ? Vec2{ u / len, v / len } : Vec2{ 0.0f, 0.0f };
        }
    };
}