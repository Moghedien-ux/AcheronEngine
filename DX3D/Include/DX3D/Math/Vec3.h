#pragma once
#include <DX3D/Core/Core.h>
#include <cmath>

namespace dx3d
{
    struct Vec3
    {
        f32 x{ 0.0f };
        f32 y{ 0.0f };
        f32 z{ 0.0f };

        Vec3() = default;
        Vec3(f32 _x, f32 _y, f32 _z) : x(_x), y(_y), z(_z) {}

        Vec3 operator+(const Vec3& o) const { return { x + o.x, y + o.y, z + o.z }; }
        Vec3 operator-(const Vec3& o) const { return { x - o.x, y - o.y, z - o.z }; }
        Vec3 operator*(f32 s) const { return { x * s, y * s, z * s }; }
        Vec3 operator*(const Vec3& o) const { return { x * o.x, y * o.y, z * o.z }; }
        Vec3 operator/(f32 s) const { return { x / s, y / s, z / s }; }

        Vec3& operator+=(const Vec3& o) { x += o.x; y += o.y; z += o.z; return *this; }
        Vec3& operator-=(const Vec3& o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
        Vec3& operator*=(f32 s) { x *= s; y *= s; z *= s; return *this; }
        Vec3& operator/=(f32 s) { x /= s; y /= s; z /= s; return *this; }

        Vec3 operator-() const { return { -x, -y, -z }; }

        f32 lengthSq() const { return x * x + y * y + z * z; }
        f32 length() const { return std::sqrt(lengthSq()); }

        Vec3 normalized() const
        {
            f32 len = length();
            return len > 1e-6f ? Vec3{ x / len, y / len, z / len } : Vec3{ 0.0f, 0.0f, 0.0f };
        }

        static f32 dot(const Vec3& a, const Vec3& b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }

        static f32 Dot(const Vec3& a, const Vec3& b)
        {
            return dot(a, b);
        }

        static Vec3 cross(const Vec3& a, const Vec3& b)
        {
            return {
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x
            };
        }

        static Vec3 Cross(const Vec3& a, const Vec3& b)
        {
            return cross(a, b);
        }

        f32 lengthSquared() const { return lengthSq(); }

        static Vec3 lerp(const Vec3& a, const Vec3& b, f32 t)
        {
            return a + (b - a) * t;
        }

        static const Vec3 Zero;
        static const Vec3 One;
        static const Vec3 Forward;
        static const Vec3 Up;
        static const Vec3 Right;
    };

    inline const Vec3 Vec3::Zero{ 0.0f, 0.0f, 0.0f };
    inline const Vec3 Vec3::One{ 1.0f, 1.0f, 1.0f };
    inline const Vec3 Vec3::Forward{ 0.0f, 0.0f, 1.0f };
    inline const Vec3 Vec3::Up{ 0.0f, 1.0f, 0.0f };
    inline const Vec3 Vec3::Right{ 1.0f, 0.0f, 0.0f };

    inline Vec3 operator*(f32 s, const Vec3& v) { return v * s; }
}