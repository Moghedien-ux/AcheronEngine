#include <DX3D/Physics/RigidBody.h>
#include <DX3D/Physics/GameObject.h>
#include <algorithm>

using namespace dx3d;

RigidBody::RigidBody(GameObject* owner, const RigidBodyDesc& desc) :
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

void RigidBody::setMass(f32 newMass)
{
    m_mass = (std::max)(0.001f, newMass);
    m_invMass = m_isStatic ? 0.0f : (1.0f / m_mass);
    recalculateInertia();
}

void RigidBody::setStatic(bool isStatic)
{
    m_isStatic = isStatic;
    m_invMass = isStatic ? 0.0f : (1.0f / m_mass);
    m_invInertia = isStatic ? Vec3{ 0.0f, 0.0f, 0.0f } : Vec3{ 1.0f / m_inertia.x, 1.0f / m_inertia.y, 1.0f / m_inertia.z };
}

void RigidBody::recalculateInertia()
{
    if (!m_owner) return;
    const Vec3& s = m_owner->m_scale;

    // Solid cuboid moment of inertia: I_x = (1/12) * m * (h^2 + d^2)
    m_inertia.x = (1.0f / 12.0f) * m_mass * (s.y * s.y + s.z * s.z);
    m_inertia.y = (1.0f / 12.0f) * m_mass * (s.x * s.x + s.z * s.z);
    m_inertia.z = (1.0f / 12.0f) * m_mass * (s.x * s.x + s.y * s.y);

    if (!m_isStatic && m_mass > 0.0001f)
    {
        m_invInertia.x = 1.0f / (std::max)(0.0001f, m_inertia.x);
        m_invInertia.y = 1.0f / (std::max)(0.0001f, m_inertia.y);
        m_invInertia.z = 1.0f / (std::max)(0.0001f, m_inertia.z);
    }
    else
    {
        m_invInertia = { 0.0f, 0.0f, 0.0f };
    }
}

void RigidBody::applyForce(const Vec3& force)
{
    if (m_isStatic) return;
    m_force += force;
}

void RigidBody::applyImpulse(const Vec3& impulse)
{
    if (m_isStatic) return;
    m_velocity += impulse * m_invMass;
}

void RigidBody::applyImpulseAtPoint(const Vec3& impulse, const Vec3& point)
{
    if (m_isStatic) return;
    m_velocity += impulse * m_invMass;

    Vec3 r = point - (m_owner ? m_owner->m_position : Vec3{ 0.0f, 0.0f, 0.0f });
    Vec3 torqueImpulse = Vec3::Cross(r, impulse);
    m_angularVelocity += Vec3{
        torqueImpulse.x * m_invInertia.x,
        torqueImpulse.y * m_invInertia.y,
        torqueImpulse.z * m_invInertia.z
    };
}

void RigidBody::integrate(f32 dt)
{
    if (m_isStatic || !m_owner) return;

    if (m_useGravity)
    {
        m_force += Vec3{ 0.0f, -9.81f * m_mass, 0.0f };
    }

    // Linear integration
    Vec3 accel = m_force * m_invMass;
    m_velocity += accel * dt;
    m_velocity = m_velocity * (1.0f - m_linearDamping * dt);
    m_owner->m_position += m_velocity * dt;

    // Angular integration
    Vec3 angAccel{
        m_torque.x * m_invInertia.x,
        m_torque.y * m_invInertia.y,
        m_torque.z * m_invInertia.z
    };
    m_angularVelocity += angAccel * dt;
    m_angularVelocity = m_angularVelocity * (1.0f - m_angularDamping * dt);
    m_owner->m_rotation += m_angularVelocity * dt;

    // Reset accumulators
    m_force  = { 0.0f, 0.0f, 0.0f };
    m_torque = { 0.0f, 0.0f, 0.0f };
}

f32 RigidBody::getKineticEnergy() const
{
    f32 linearKE = 0.5f * m_mass * m_velocity.lengthSquared();
    f32 angularKE = 0.5f * (
        m_inertia.x * m_angularVelocity.x * m_angularVelocity.x +
        m_inertia.y * m_angularVelocity.y * m_angularVelocity.y +
        m_inertia.z * m_angularVelocity.z * m_angularVelocity.z
    );
    return linearKE + angularKE;
}