#include <DX3D/Camera/Camera.h>
#include <algorithm>

dx3d::Camera::Camera(const Vec3& position) : m_position(position)
{
	setPerspective(m_fov, m_aspect, m_nearZ, m_farZ);
}

void dx3d::Camera::setPerspective(f32 fovRad, f32 aspectRatio, f32 nearZ, f32 farZ)
{
	m_fov = fovRad;
	m_aspect = aspectRatio;
	m_nearZ = nearZ;
	m_farZ = farZ;
	m_projectionMatrix = Mat4::PerspectiveFovLH(fovRad, aspectRatio, nearZ, farZ);
}

void dx3d::Camera::update(const InputSystem& input, f32 deltaTime)
{
	// Mouse rotation (Right mouse button drag or free look)
	if (input.isMouseButtonDown(1) || input.isMouseButtonDown(0))
	{
		Point delta = input.getMouseDelta();
		m_yaw += delta.x * m_mouseSensitivity;
		m_pitch -= delta.y * m_mouseSensitivity;

		// Clamp pitch to avoid gimbal flipping (-89 to +89 degrees)
		constexpr f32 maxPitch = 1.55f; // ~89 deg in radians
		m_pitch = std::clamp(m_pitch, -maxPitch, maxPitch);
	}

	// Calculate forward, right, and up vectors from yaw & pitch
	Vec3 forward{
		std::cos(m_pitch) * std::sin(m_yaw),
		-std::sin(m_pitch),
		std::cos(m_pitch) * std::cos(m_yaw)
	};
	forward = forward.normalized();

	Vec3 worldUp{ 0.0f, 1.0f, 0.0f };
	Vec3 right = Vec3::cross(worldUp, forward).normalized();
	Vec3 up = Vec3::cross(forward, right).normalized();

	f32 speed = m_moveSpeed * deltaTime;
	if (input.isKeyDown(VK_SHIFT)) speed *= 2.0f; // Sprint

	// Keyboard Movement: WASD + Space/Q (Up/Down)
	if (input.isKeyDown('W')) m_position += forward * speed;
	if (input.isKeyDown('S')) m_position -= forward * speed;
	if (input.isKeyDown('D')) m_position += right * speed;
	if (input.isKeyDown('A')) m_position -= right * speed;
	if (input.isKeyDown(VK_SPACE) || input.isKeyDown('E')) m_position += worldUp * speed;
	if (input.isKeyDown('Q') || input.isKeyDown(VK_CONTROL)) m_position -= worldUp * speed;
}

dx3d::Mat4 dx3d::Camera::getViewMatrix() const
{
	Vec3 forward{
		std::cos(m_pitch) * std::sin(m_yaw),
		-std::sin(m_pitch),
		std::cos(m_pitch) * std::cos(m_yaw)
	};
	Vec3 target = m_position + forward.normalized();
	Vec3 up{ 0.0f, 1.0f, 0.0f };

	return Mat4::LookAtLH(m_position, target, up);
}

dx3d::Mat4 dx3d::Camera::getProjectionMatrix() const
{
	return m_projectionMatrix;
}