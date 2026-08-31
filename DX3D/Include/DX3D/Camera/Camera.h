#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Math/Vec3.h>
#include <DX3D/Math/Mat4.h>

namespace dx3d
{
    class InputSystem;

    class Camera
    {
    public:
        explicit Camera(const Vec3& position = { 0.0f, 2.0f, -5.0f });
        ~Camera() = default;

        void update(const InputSystem& input, f32 deltaTime);
        void setPerspective(f32 fovRadians, f32 aspect, f32 nearZ, f32 farZ);

        Mat4 getViewMatrix() const;
        Mat4 getProjectionMatrix() const { return m_projMatrix; }
        Vec3 getPosition() const { return m_position; }
        void setPosition(const Vec3& pos) { m_position = pos; }

        Vec3 getForward() const;
        Vec3 getRight() const;
        Vec3 getUp() const;

    private:
        Vec3 m_position{ 0.0f, 2.0f, -5.0f };
        f32 m_yaw{ 0.0f };   // In radians
        f32 m_pitch{ 0.0f }; // In radians
        f32 m_moveSpeed{ 6.0f };
        f32 m_lookSensitivity{ 0.003f };

        Mat4 m_projMatrix{};
    };
}
