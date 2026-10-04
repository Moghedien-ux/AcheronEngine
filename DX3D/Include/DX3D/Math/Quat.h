#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Math/Vec3.h>
#include <cmath>
#include <algorithm>

namespace dx3d
{
    struct Mat4;

    struct Quat
    {
        f32 x{ 0.0f };
        f32 y{ 0.0f };
        f32 z{ 0.0f };
        f32 w{ 1.0f };

        Quat() = default;
        Quat(f32 _x, f32 _y, f32 _z, f32 _w) : x(_x), y(_y), z(_z), w(_w) {}

        static Quat Identity() { return Quat(0.0f, 0.0f, 0.0f, 1.0f); }

        static Quat FromEuler(f32 pitch, f32 yaw, f32 roll)
        {
            f32 p = pitch * 0.5f;
            f32 y = yaw * 0.5f;
            f32 r = roll * 0.5f;

            f32 sinP = std::sin(p), cosP = std::cos(p);
            f32 sinY = std::sin(y), cosY = std::cos(y);
            f32 sinR = std::sin(r), cosR = std::cos(r);

            Quat q;
            q.w = cosR * cosP * cosY + sinR * sinP * sinY;
            q.x = cosR * sinP * cosY + sinR * cosP * sinY;
            q.y = cosR * cosP * sinY - sinR * sinP * cosY;
            q.z = sinR * cosP * cosY - cosR * sinP * sinY;
            return q.normalized();
        }

        static Quat FromEuler(const Vec3& eulerRad)
        {
            return FromEuler(eulerRad.x, eulerRad.y, eulerRad.z);
        }

        static Quat FromAxisAngle(const Vec3& axis, f32 angleRad)
        {
            f32 halfAngle = angleRad * 0.5f;
            f32 s = std::sin(halfAngle);
            Vec3 normAxis = axis.normalized();
            return Quat(normAxis.x * s, normAxis.y * s, normAxis.z * s, std::cos(halfAngle));
        }

        Vec3 toEuler() const
        {
            Vec3 euler;

            // Pitch (x)
            f32 sinr_cosp = 2.0f * (w * x + y * z);
            f32 cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
            euler.x = std::atan2(sinr_cosp, cosr_cosp);

            // Yaw (y)
            f32 sinp = 2.0f * (w * y - z * x);
            if (std::abs(sinp) >= 1.0f)
                euler.y = std::copysign(3.14159265f * 0.5f, sinp);
            else
                euler.y = std::asin(sinp);

            // Roll (z)
            f32 siny_cosp = 2.0f * (w * z + x * y);
            f32 cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
            euler.z = std::atan2(siny_cosp, cosy_cosp);

            return euler;
        }

        Quat operator*(const Quat& rhs) const
        {
            return Quat(
                w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
                w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
                w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w,
                w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z
            );
        }

        f32 lengthSq() const { return x * x + y * y + z * z + w * w; }
        f32 length() const { return std::sqrt(lengthSq()); }

        Quat normalized() const
        {
            f32 len = length();
            if (len > 1e-6f)
            {
                f32 inv = 1.0f / len;
                return Quat(x * inv, y * inv, z * inv, w * inv);
            }
            return Identity();
        }

        Quat conjugate() const { return Quat(-x, -y, -z, w); }
        Quat inverse() const
        {
            f32 lenSq = lengthSq();
            if (lenSq > 1e-6f)
            {
                f32 inv = 1.0f / lenSq;
                return Quat(-x * inv, -y * inv, -z * inv, w * inv);
            }
            return Identity();
        }

        // Rotates by : v' = q * v * q^-1
        Vec3 rotate(const Vec3& v) const
        {
            Vec3 qVec(x, y, z);
            Vec3 uv = Vec3::cross(qVec, v);
            Vec3 uuv = Vec3::cross(qVec, uv);
            return v + ((uv * w) + uuv) * 2.0f;
        }

        // (SLERP)
        static Quat slerp(const Quat& a, const Quat& b, f32 t)
        {
            Quat q2 = b;
            f32 cosTheta = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;

            if (cosTheta < 0.0f)
            {
                q2 = Quat(-b.x, -b.y, -b.z, -b.w);
                cosTheta = -cosTheta;
            }

            if (cosTheta > 0.9995f)
            {
                return Quat(
                    a.x + t * (q2.x - a.x),
                    a.y + t * (q2.y - a.y),
                    a.z + t * (q2.z - a.z),
                    a.w + t * (q2.w - a.w)
                ).normalized();
            }

            f32 theta = std::acos(cosTheta);
            f32 sinTheta = std::sin(theta);
            f32 wA = std::sin((1.0f - t) * theta) / sinTheta;
            f32 wB = std::sin(t * theta) / sinTheta;

            return Quat(
                a.x * wA + q2.x * wB,
                a.y * wA + q2.y * wB,
                a.z * wA + q2.z * wB,
                a.w * wA + q2.w * wB
            ).normalized();
        }
    };
}