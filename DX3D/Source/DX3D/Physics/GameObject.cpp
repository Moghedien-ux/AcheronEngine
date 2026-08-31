// ==========================================
// Fișier: DX3D/Source/GameObject.cpp
// ==========================================
#include <DX3D/Physics/GameObject.h>
#include <DX3D/Physics/RigidBody.h>

dx3d::GameObject::GameObject(const GameObjectDesc& desc) :
    m_name(desc.name),
    m_position(desc.position),
    m_rotation(desc.rotation),
    m_scale(desc.scale),
    m_color(desc.color)
{
    m_rigidBody = std::make_shared<RigidBody>(this);
}

dx3d::Mat4 dx3d::GameObject::getWorldMatrix() const
{
    Mat4 s = Mat4::Scale(m_scale);
    Mat4 rx = Mat4::RotationX(m_rotation.x);
    Mat4 ry = Mat4::RotationY(m_rotation.y);
    Mat4 rz = Mat4::RotationZ(m_rotation.z);
    Mat4 t = Mat4::Translation(m_position);

    return s * rx * ry * rz * t;
}

void dx3d::GameObject::setScale(const Vec3& newScale)
{
    m_scale.x = (std::max)(0.1f, newScale.x);
    m_scale.y = (std::max)(0.1f, newScale.y);
    m_scale.z = (std::max)(0.1f, newScale.z);

    if (m_rigidBody)
    {
        m_rigidBody->recalculateInertia();
    }
}