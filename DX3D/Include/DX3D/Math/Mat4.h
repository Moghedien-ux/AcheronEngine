#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Math/Vec4.h>
#include <DX3D/Math/Vec3.h>
#include <cmath>
#include <cstring>

namespace dx3d
{
	class Mat4
	{
	public:
		f32 m[4][4]{};

		Mat4()
		{
			setIdentity();
		}

		void setIdentity()
		{
			std::memset(m, 0, sizeof(m));
			m[0][0] = 1.0f;
			m[1][1] = 1.0f;
			m[2][2] = 1.0f;
			m[3][3] = 1.0f;
		}

		static Mat4 Identity()
		{
			Mat4 res;
			res.setIdentity();
			return res;
		}

		static Mat4 Translation(const Vec3& translation)
		{
			Mat4 res;
			res.setIdentity();

			res.m[3][0] = translation.x;
			res.m[3][1] = translation.y;
			res.m[3][2] = translation.z;

			return res;
		}

		static Mat4 Scale(const Vec3& scale)
		{
			Mat4 res;
			res.setIdentity();

			res.m[0][0] = scale.x;
			res.m[1][1] = scale.y;
			res.m[2][2] = scale.z;

			return res;
		}

		static Mat4 RotationX(f32 angleRad)
		{
			Mat4 res;
			res.setIdentity();

			f32 c = std::cos(angleRad);
			f32 s = std::sin(angleRad);

			res.m[1][1] = c;
			res.m[1][2] = s;
			res.m[2][1] = -s;
			res.m[2][2] = c;

			return res;
		}

		static Mat4 RotationY(f32 angleRad)
		{
			Mat4 res;
			res.setIdentity();

			f32 c = std::cos(angleRad);
			f32 s = std::sin(angleRad);

			res.m[0][0] = c;
			res.m[0][2] = -s;
			res.m[2][0] = s;
			res.m[2][2] = c;

			return res;
		}

		static Mat4 RotationZ(f32 angleRad)
		{
			Mat4 res;
			res.setIdentity();

			f32 c = std::cos(angleRad);
			f32 s = std::sin(angleRad);

			res.m[0][0] = c;
			res.m[0][1] = s;
			res.m[1][0] = -s;
			res.m[1][1] = c;

			return res;
		}

		static Mat4 LookAtLH(const Vec3& eye, const Vec3& target, const Vec3& up)
		{
			Vec3 zAxis = (target - eye).normalized();
			Vec3 xAxis = Vec3::cross(up, zAxis).normalized();
			Vec3 yAxis = Vec3::cross(xAxis, zAxis);

			Mat4 res;
			res.m[0][0] = xAxis.x; res.m[0][1] = yAxis.x; res.m[0][2] = zAxis.x; res.m[0][3] = 0.0f;
			res.m[1][0] = xAxis.y; res.m[1][1] = yAxis.y; res.m[1][2] = zAxis.y; res.m[1][3] = 0.0f;
			res.m[2][0] = xAxis.z; res.m[2][1] = yAxis.z; res.m[2][2] = zAxis.z; res.m[2][3] = 0.0f;
			res.m[3][0] = -Vec3::dot(xAxis, eye);
			res.m[3][1] = -Vec3::dot(yAxis, eye);
			res.m[3][2] = -Vec3::dot(zAxis, eye);
			res.m[3][3] = 1.0f;

			return res;
		}

		static Mat4 PerspectiveFovLH(f32 fovRad, f32 aspect, f32 nearZ, f32 farZ)
		{
			Mat4 res;
			std::memset(res.m, 0, sizeof(res.m));
			f32 tanHalfFov = std::tan(fovRad * 0.5f);
			res.m[0][0] = 1.0f / (aspect * tanHalfFov);
			res.m[1][1] = 1.0f / tanHalfFov;
			res.m[2][2] = farZ / (farZ - nearZ);
			res.m[2][3] = 1.0f;
			res.m[3][2] = (-nearZ * farZ) / (farZ - nearZ);
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