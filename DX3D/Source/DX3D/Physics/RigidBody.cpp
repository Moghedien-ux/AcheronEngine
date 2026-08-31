#include <DX3D/Physics/RigidBody.h>
#include <DX3D/Physics/GameObject.h>

using namespace  dx3d;

dx3d::RigidBody::RigidBody(GameObject* owner, const RigidBodyDesc& desc) :
    m_owner(owner),
    m_mass(desc.mass),
    m_useGravity(desc.useGravity),
    m_isStatic(desc.isStatic),
    m_restitution(desc.restitution),
    m_friction(desc.friction),
    m_linearDamping(desc.linearDamping),
    m_angularDamping(desc.angularDamping)
{
    setMass(desc.mass);
    setStatic(desc.isStatic);
}

void dx3d::RigidBody::setMass(f32 newMass)
{
    m_mass = (std::max)(0.001f, newMass);
    m_invMass = m_isStatic ? 0.0f : (1.0f / m_mass);
    recalculateInertia();
}

void dx3d::RigidBody::setStatic(bool isStatic)
{
    m_isStatic = isStatic;
    m_invMass = m_isStatic ? 0.0f : (1.0f / m_mass);
    recalculateInertia();
}

void dx3d::RigidBody::recalculateInertia()
{
    if (m_isStatic || !m_owner)
    {
        m_inertia = { 0.0f, 0.0f, 0.0f };
        m_invInertia = { 0.0f, 0.0f, 0.0f };
        return;
    }

    // Cuboid Moment of Inertia tensor:
    // Ix = 1/12 * m * (h^2 + d^2)
    // Iy = 1/12 * m * (w^2 + d^2)
    // Iz = 1/12 * m * (w^2 + h^2)
    Vec3 size = m_owner->m_scale;
    f32 factor = (1.0f / 12.0f) * m_mass;

    m_inertia.x = factor * (size.y * size.y + size.z * size.z);
    m_inertia.y = factor * (size.x * size.x + size.z * size.z);
    m_inertia.z = factor * (size.x * size.x + size.y * size.y);

    m_invInertia.x = m_inertia.x > 1e-6f ? (1.0f / m_inertia.x) : 0.0f;
    m_invInertia.y = m_inertia.y > 1e-6f ? (1.0f / m_inertia.y) : 0.0f;
    m_invInertia.z = m_inertia.z > 1e-6f ? (1.0f / m_inertia.z) : 0.0f;
}

void dx3d::RigidBody::applyForce(const Vec3& force)
{
    if (m_isStatic) return;
    m_force += force;
}

void dx3d::RigidBody::applyImpulse(const Vec3& impulse)
{
    if (m_isStatic) return;
    m_velocity += impulse * m_invMass;
}

void dx3d::RigidBody::applyImpulseAtPoint(const Vec3& impulse, const Vec3& point)
{
    if (m_isStatic || !m_owner) return;
    applyImpulse(impulse);

    Vec3 r = point - m_owner->m_position;
    Vec3 torqueImpulse = Vec3::cross(r, impulse);
    m_angularVelocity += Vec3{
        torqueImpulse.x * m_invInertia.x,
        torqueImpulse.y * m_invInertia.y,
        torqueImpulse.z * m_invInertia.z
    };
}

void dx3d::RigidBody::integrate(f32 dt)
{
    if (m_isStatic || !m_owner) return;

    // Linear Integration
    Vec3 acceleration = m_force * m_invMass;
    m_velocity += acceleration * dt;

    // Linear Damping
    m_velocity *= std::clamp(1.0f - m_linearDamping * dt, 0.0f, 1.0f);
    m_owner->m_position += m_velocity * dt;

    // Angular Integration
    Vec3 angularAccel = {
        m_torque.x * m_invInertia.x,
        m_torque.y * m_invInertia.y,
        m_torque.z * m_invInertia.z
    };
    m_angularVelocity += angularAccel * dt;

    // Angular Damping
    m_angularVelocity *= std::clamp(1.0f - m_angularDamping * dt, 0.0f, 1.0f);
    m_owner->m_rotation += m_angularVelocity * dt;

    // Reset accumulated forces & torques
    m_force = { 0.0f, 0.0f, 0.0f };
    m_torque = { 0.0f, 0.0f, 0.0f };
}

f32 dx3d::RigidBody::getKineticEnergy() const
{
    if (m_isStatic) return 0.0f;
    f32 linearKE = 0.5f * m_mass * m_velocity.lengthSq();
    f32 angularKE = 0.5f * (
        m_inertia.x * m_angularVelocity.x * m_angularVelocity.x +
        m_inertia.y * m_angularVelocity.y * m_angularVelocity.y +
        m_inertia.z * m_angularVelocity.z * m_angularVelocity.z
    );
    return linearKE + angularKE;
}
