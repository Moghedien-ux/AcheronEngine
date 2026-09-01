#include <DX3D/Math/Transform.h>

using namespace dx3d;

Transform::Transform()
    : m_location(0.0f, 0.0f, 0.0f),
      m_rotation(Quat::Identity()),
      m_scale(1.0f, 1.0f, 1.0f)
{
    markDirty();
}

Transform::Transform(const Vec3& location, const Quat& rotation, const Vec3& scale)
    : m_location(location),
      m_rotation(rotation),
      m_scale(scale)
{
    markDirty();
}

Transform::~Transform()
{
    if (m_parent)
    {
        m_parent->removeChild(this);
    }
    for (auto* child : m_children)
    {
        if (child) child->m_parent = nullptr;
    }
}

void Transform::setLocation(const Vec3& loc)
{
    m_location = loc;
    markDirty();
}

void Transform::setRotation(const Quat& rot)
{
    m_rotation = rot.normalized();
    markDirty();
}

void Transform::setRotationEuler(const Vec3& eulerRadians)
{
    m_rotation = Quat::FromEuler(eulerRadians);
    markDirty();
}

void Transform::setRotationDegrees(const Vec3& eulerDegrees)
{
    constexpr f32 degToRad = 0.0174532925f;
    setRotationEuler(eulerDegrees * degToRad);
}

Vec3 Transform::getRotationDegrees() const
{
    constexpr f32 radToDeg = 57.2957795f;
    return getRotationEuler() * radToDeg;
}

void Transform::setScale(const Vec3& scale)
{
    m_scale = scale;
    markDirty();
}

Vec3 Transform::getForward() const
{
    return getWorldRotation().rotate(Vec3::Forward).normalized();
}

Vec3 Transform::getRight() const
{
    return getWorldRotation().rotate(Vec3::Right).normalized();
}

Vec3 Transform::getUp() const
{
    return getWorldRotation().rotate(Vec3::Up).normalized();
}

void Transform::setParent(Transform* parent, bool keepWorldTransform)
{
    if (m_parent == parent || parent == this) return;

    Mat4 previousWorld = getWorldMatrix();

    if (m_parent)
    {
        m_parent->removeChild(this);
    }

    m_parent = parent;

    if (m_parent)
    {
        m_parent->m_children.push_back(this);
    }

    if (keepWorldTransform && m_parent)
    {
        Vec3 parentWorldLoc = m_parent->getWorldLocation();
        Quat parentWorldRot = m_parent->getWorldRotation();
        Vec3 parentWorldScale = m_parent->getWorldScale();

        Quat invParentRot = parentWorldRot.inverse();
        m_location = invParentRot.rotate(Vec3(
            previousWorld.m[3][0] - parentWorldLoc.x,
            previousWorld.m[3][1] - parentWorldLoc.y,
            previousWorld.m[3][2] - parentWorldLoc.z
        ));
        m_rotation = invParentRot * getWorldRotation();
        m_scale = { m_scale.x / parentWorldScale.x, m_scale.y / parentWorldScale.y, m_scale.z / parentWorldScale.z };
    }

    markDirty();
}

void Transform::removeChild(Transform* child)
{
    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it != m_children.end())
    {
        m_children.erase(it);
    }
}

Mat4 Transform::getLocalMatrix() const
{
    return Mat4::TransformMatrix(m_location, m_rotation, m_scale);
}

Mat4 Transform::getWorldMatrix() const
{
    if (m_isDirty)
    {
        updateWorldMatrix();
    }
    return m_cachedWorldMatrix;
}

void Transform::updateWorldMatrix() const
{
    m_cachedLocalMatrix = getLocalMatrix();
    if (m_parent)
    {
        m_cachedWorldMatrix = m_cachedLocalMatrix * m_parent->getWorldMatrix();
    }
    else
    {
        m_cachedWorldMatrix = m_cachedLocalMatrix;
    }
    m_isDirty = false;
}

Vec3 Transform::getWorldLocation() const
{
    Mat4 world = getWorldMatrix();
    return Vec3(world.m[3][0], world.m[3][1], world.m[3][2]);
}

Quat Transform::getWorldRotation() const
{
    if (m_parent)
    {
        return (m_parent->getWorldRotation() * m_rotation).normalized();
    }
    return m_rotation;
}

Vec3 Transform::getWorldScale() const
{
    if (m_parent)
    {
        Vec3 ps = m_parent->getWorldScale();
        return Vec3(m_scale.x * ps.x, m_scale.y * ps.y, m_scale.z * ps.z);
    }
    return m_scale;
}

Vec3 Transform::transformPoint(const Vec3& point) const
{
    Mat4 w = getWorldMatrix();
    return Vec3(
        point.x * w.m[0][0] + point.y * w.m[1][0] + point.z * w.m[2][0] + w.m[3][0],
        point.x * w.m[0][1] + point.y * w.m[1][1] + point.z * w.m[2][1] + w.m[3][1],
        point.x * w.m[0][2] + point.y * w.m[1][2] + point.z * w.m[2][2] + w.m[3][2]
    );
}

Vec3 Transform::transformVector(const Vec3& vector) const
{
    Quat wRot = getWorldRotation();
    return wRot.rotate(vector);
}

void Transform::markDirty()
{
    m_isDirty = true;
    for (auto* child : m_children)
    {
        if (child) child->markDirty();
    }
}