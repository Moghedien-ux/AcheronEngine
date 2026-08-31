#pragma once
#include <DX3D/Math/Vec3.h>
#include <DX3D/Math/Mat4.h>
#include <DX3D/Input/InputSystem.h>

namespace dx3d
{
    class Camera
    {
    public:
        Camera(const Vec3& position = { 0.0f, 1.0f, -4.0f });

        void update(const InputSystem& input, f32 deltaTime);
        void setPerspective(f32 fovRad, f32 aspectRatio, f32 nearZ, f32 farZ);

        Mat4 getViewMatrix() const;
        Mat4 getProjectionMatrix() const;

        Vec3 getPosition() const { return m_position; }
        f32 getYaw() const { return m_yaw; }
        f32 getPitch() const { return m_pitch; }

        void setPosition(const Vec3& pos) { m_position = pos; }
        void setRotation(f32 yaw, f32 pitch) { m_yaw = yaw; m_pitch = pitch; }
        void setMoveSpeed(f32 speed) { m_moveSpeed = speed; }
        void setMouseSensitivity(f32 sens) { m_mouseSensitivity = sens; }

    private:
        Vec3 m_position{ 0.0f, 1.0f, -4.0f };
        f32 m_yaw{ 0.0f };    // Rotation around Y axis in radians
        f32 m_pitch{ 0.0f };  // Rotation around X axis in radians

        f32 m_moveSpeed{ 4.0f };
        f32 m_mouseSensitivity{ 0.003f };

        Mat4 m_projectionMatrix{};
        f32 m_fov{ 1.0472f }; // 60 degrees in radians
        f32 m_aspect{ 16.0f / 9.0f };
        f32 m_nearZ{ 0.1f };
        f32 m_farZ{ 100.0f };
    };
}