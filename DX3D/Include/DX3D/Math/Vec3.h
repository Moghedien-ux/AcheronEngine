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

        Vec3 operator+(const Vec3& other) const { return { x + other.x, y + other.y, z + other.z }; }
        Vec3 operator-(const Vec3& other) const { return { x - other.x, y - other.y, z - other.z }; }
        Vec3 operator*(f32 scalar) const { return { x * scalar, y * scalar, z * scalar }; }
        Vec3 operator/(f32 scalar) const { return { x / scalar, y / scalar, z / scalar }; }

        Vec3& operator+=(const Vec3& other)
        {
            x += other.x; y += other.y; z += other.z;
            return *this;
        }

        Vec3& operator-=(const Vec3& other)
        {
            x -= other.x; y -= other.y; z -= other.z;
            return *this;
        }

        Vec3& operator*=(f32 scalar)
        {
            x *= scalar; y *= scalar; z *= scalar;
            return *this;
        }

        f32 lengthSq() const { return x * x + y * y + z * z; }
        f32 length() const { return std::sqrt(lengthSq()); }

        Vec3 normalized() const
        {
            f32 len = length();
            if (len > 1e-6f)
                return { x / len, y / len, z / len };
            return { 0.0f, 0.0f, 0.0f };
        }

        static f32 dot(const Vec3& a, const Vec3& b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }

        static Vec3 cross(const Vec3& a, const Vec3& b)
        {
            return {
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x
            };
        }
    };

    inline Vec3 operator*(f32 scalar, const Vec3& v) { return v * scalar; }
}
