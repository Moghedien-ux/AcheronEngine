#include <DX3D/Camera/Camera.h>
#include <DX3D/Input/InputSystem.h>
#include <algorithm>
#include <cmath>

using namespace dx3d;

dx3d::Camera::Camera(const Vec3& position) : m_position(position)
{
    setPerspective(1.0472f, 16.0f / 9.0f, 0.1f, 1000.0f);
}

void dx3d::Camera::setPerspective(f32 fovRadians, f32 aspect, f32 nearZ, f32 farZ)
{
    m_projMatrix = Mat4::PerspectiveLH(fovRadians, aspect, nearZ, farZ);
}

Vec3 dx3d::Camera::getForward() const
{
    f32 cosPitch = std::cos(m_pitch);
    f32 sinPitch = std::sin(m_pitch);
    f32 cosYaw   = std::cos(m_yaw);
    f32 sinYaw   = std::sin(m_yaw);

    return Vec3{ sinYaw * cosPitch, sinPitch, cosYaw * cosPitch }.normalized();
}

Vec3 dx3d::Camera::getRight() const
{
    Vec3 fwd = getForward();
    return Vec3::cross({ 0.0f, 1.0f, 0.0f }, fwd).normalized();
}

Vec3 dx3d::Camera::getUp() const
{
    return Vec3::cross(getForward(), getRight()).normalized();
}

Mat4 dx3d::Camera::getViewMatrix() const
{
    Vec3 fwd = getForward();
    Vec3 target = m_position + fwd;
    return Mat4::LookAtLH(m_position, target, { 0.0f, 1.0f, 0.0f });
}

void dx3d::Camera::update(const InputSystem& input, f32 deltaTime)
{
    if (input.isMouseButtonDown(1))
    {
        Point delta = input.getMouseDelta();
        m_yaw += delta.x * m_lookSensitivity;
        m_pitch -= delta.y * m_lookSensitivity;

        m_pitch = std::clamp(m_pitch, -1.55f, 1.55f);
    }

    // WASD Movement
    Vec3 fwd = getForward();
    Vec3 right = getRight();
    f32 speed = m_moveSpeed * deltaTime;

    if (input.isKeyDown('W')) m_position += fwd * speed;
    if (input.isKeyDown('S')) m_position -= fwd * speed;
    if (input.isKeyDown('D')) m_position += right * speed;
    if (input.isKeyDown('A')) m_position -= right * speed;
    if (input.isKeyDown('E') || input.isKeyDown(VK_SPACE)) m_position.y += speed;
    if (input.isKeyDown('Q') || input.isKeyDown(VK_CONTROL)) m_position.y -= speed;
}
