#pragma once
#include <DX3D/Core/Core.h>
#include <cmath>

namespace dx3d
{
    class Vec3
    {
    public:
        f32 x{ 0.0f }, y{ 0.0f }, z{ 0.0f };

        Vec3() = default;
        Vec3(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}

        Vec3 operator+(const Vec3& rhs) const { return { x + rhs.x, y + rhs.y, z + rhs.z }; }
        Vec3 operator-(const Vec3& rhs) const { return { x - rhs.x, y - rhs.y, z - rhs.z }; }
        Vec3 operator*(const f32 scalar) const { return{x * scalar, y * scalar, z * scalar}; }
        Vec3 operator/(const f32 scalar) const { return{ x / scalar, y / scalar, z / scalar }; }

        Vec3& operator+=(const Vec3& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
        Vec3& operator-=(const Vec3& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
        Vec3& operator*=(f32 scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }

        f32 length() const { return std::sqrt(x * x + y * y + z * z); }

        Vec3 normalized() const
        {
            f32 len = length();
            if (len > 0.00001f)
                return *this / len;
            return *this;
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
}