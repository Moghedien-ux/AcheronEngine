#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Math/Vec3.h>
#include <DX3D/Math/Vec4.h>
#include <DX3D/Math/Quat.h>
#include <cmath>
#include <cstring>

namespace dx3d
{
    struct Mat4
    {
        f32 m[4][4]{};

        Mat4() { setIdentity(); }

        void setIdentity()
        {
            std::memset(m, 0, sizeof(m));
            m[0][0] = 1.0f;
            m[1][1] = 1.0f;
            m[2][2] = 1.0f;
            m[3][3] = 1.0f;
        }

        static Mat4 Identity() { return Mat4(); }

        static Mat4 Translation(const Vec3& t)
        {
            Mat4 res;
            res.m[3][0] = t.x;
            res.m[3][1] = t.y;
            res.m[3][2] = t.z;
            return res;
        }

        static Mat4 Scale(const Vec3& s)
        {
            Mat4 res;
            res.m[0][0] = s.x;
            res.m[1][1] = s.y;
            res.m[2][2] = s.z;
            return res;
        }

        static Mat4 RotationX(f32 angle)
        {
            Mat4 res;
            f32 c = std::cos(angle);
            f32 s = std::sin(angle);
            res.m[1][1] = c;
            res.m[1][2] = s;
            res.m[2][1] = -s;
            res.m[2][2] = c;
            return res;
        }

        static Mat4 RotationY(f32 angle)
        {
            Mat4 res;
            f32 c = std::cos(angle);
            f32 s = std::sin(angle);
            res.m[0][0] = c;
            res.m[0][2] = -s;
            res.m[2][0] = s;
            res.m[2][2] = c;
            return res;
        }

        static Mat4 RotationZ(f32 angle)
        {
            Mat4 res;
            f32 c = std::cos(angle);
            f32 s = std::sin(angle);
            res.m[0][0] = c;
            res.m[0][1] = s;
            res.m[1][0] = -s;
            res.m[1][1] = c;
            return res;
        }

        static Mat4 rotationX(f32 angle) { return RotationX(angle); }
        static Mat4 rotationY(f32 angle) { return RotationY(angle); }
        static Mat4 rotationZ(f32 angle) { return RotationZ(angle); }

        static Mat4 RotationFromQuat(const Quat& q)
        {
            Mat4 res;
            f32 xx = q.x * q.x, yy = q.y * q.y, zz = q.z * q.z;
            f32 xy = q.x * q.y, xz = q.x * q.z, yz = q.y * q.z;
            f32 wx = q.w * q.x, wy = q.w * q.y, wz = q.w * q.z;

            res.m[0][0] = 1.0f - 2.0f * (yy + zz);
            res.m[0][1] = 2.0f * (xy + wz);
            res.m[0][2] = 2.0f * (xz - wy);
            res.m[0][3] = 0.0f;

            res.m[1][0] = 2.0f * (xy - wz);
            res.m[1][1] = 1.0f - 2.0f * (xx + zz);
            res.m[1][2] = 2.0f * (yz + wx);
            res.m[1][3] = 0.0f;

            res.m[2][0] = 2.0f * (xz + wy);
            res.m[2][1] = 2.0f * (yz - wx);
            res.m[2][2] = 1.0f - 2.0f * (xx + yy);
            res.m[2][3] = 0.0f;

            return res;
        }

        // Direct Transform Matrix (Scale * Rotation * Translation in row-vector DirectX notation)
        static Mat4 TransformMatrix(const Vec3& translation, const Quat& rotation, const Vec3& scale)
        {
            Mat4 s = Mat4::Scale(scale);
            Mat4 r = Mat4::RotationFromQuat(rotation);
            Mat4 t = Mat4::Translation(translation);
            return s * r * t;
        }

        static Mat4 PerspectiveLH(f32 fov, f32 aspect, f32 nearZ, f32 farZ)
        {
            Mat4 res;
            std::memset(res.m, 0, sizeof(res.m));
            f32 tanHalfFov = std::tan(fov * 0.5f);

            res.m[0][0] = 1.0f / (aspect * tanHalfFov);
            res.m[1][1] = 1.0f / tanHalfFov;
            res.m[2][2] = farZ / (farZ - nearZ);
            res.m[2][3] = 1.0f;
            res.m[3][2] = (-nearZ * farZ) / (farZ - nearZ);
            res.m[3][3] = 0.0f;
            return res;
        }

        static Mat4 LookAtLH(const Vec3& eye, const Vec3& target, const Vec3& up)
        {
            Vec3 zAxis = (target - eye).normalized();
            Vec3 xAxis = Vec3::Cross(up, zAxis).normalized();
            Vec3 yAxis = Vec3::Cross(zAxis, xAxis);

            Mat4 res;
            res.m[0][0] = xAxis.x; res.m[0][1] = yAxis.x; res.m[0][2] = zAxis.x; res.m[0][3] = 0.0f;
            res.m[1][0] = xAxis.y; res.m[1][1] = yAxis.y; res.m[1][2] = zAxis.y; res.m[1][3] = 0.0f;
            res.m[2][0] = xAxis.z; res.m[2][1] = yAxis.z; res.m[2][2] = zAxis.z; res.m[2][3] = 0.0f;

            res.m[3][0] = -Vec3::Dot(xAxis, eye);
            res.m[3][1] = -Vec3::Dot(yAxis, eye);
            res.m[3][2] = -Vec3::Dot(zAxis, eye);
            res.m[3][3] = 1.0f;
            return res;
        }

        Mat4 operator*(const Mat4& rhs) const
        {
            Mat4 out;
            for (int r = 0; r < 4; ++r)
            {
                for (int c = 0; c < 4; ++c)
                {
                    out.m[r][c] = m[r][0] * rhs.m[0][c] +
                                  m[r][1] * rhs.m[1][c] +
                                  m[r][2] * rhs.m[2][c] +
                                  m[r][3] * rhs.m[3][c];
                }
            }
            return out;
        }
    };
}