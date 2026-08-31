#include <DX3D/Physics/RigidBody.h>
#include <DX3D/Physics/GameObject.h>
#include <cmath>
#include <algorithm>

dx3d::RigidBody::RigidBody(GameObject* owner, const RigidBodyDesc& desc) :
    m_owner(owner),
    m_useGravity(desc.useGravity),
    m_isStatic(desc.isStatic),
    m_restitution(desc.restitution),
    m_friction(desc.friction),
    m_linearDamping(desc.linearDamping),
    m_angularDamping(desc.angularDamping)
{
    setMass(desc.mass);
}

void dx3d::RigidBody::setMass(f32 newMass)
{
    m_mass = (std::max)(0.01f, newMass);
    if (m_isStatic)
    {
        m_invMass = 0.0f;
        m_invInertia = { 0.0f, 0.0f, 0.0f };
    }
    else
    {
        m_invMass = 1.0f / m_mass;
        recalculateInertia();
    }
}

void dx3d::RigidBody::setStatic(bool isStatic)
{
    m_isStatic = isStatic;
    if (isStatic)
    {
        m_invMass = 0.0f;
        m_velocity = { 0.0f, 0.0f, 0.0f };
        m_angularVelocity = { 0.0f, 0.0f, 0.0f };
        m_invInertia = { 0.0f, 0.0f, 0.0f };
    }
    else
    {
        m_invMass = 1.0f / m_mass;
        recalculateInertia();
    }
}

void dx3d::RigidBody::recalculateInertia()
{
    if (m_isStatic || !m_owner)
    {
        m_inertia = { 0.0f, 0.0f, 0.0f };
        m_invInertia = { 0.0f, 0.0f, 0.0f };
        return;
    }

    // Momente de inerție pentru paralelipiped (w, h, d):
    f32 w = m_owner->m_scale.x;
    f32 h = m_owner->m_scale.y;
    f32 d = m_owner->m_scale.z;
    f32 m = m_mass;

    f32 ix = (1.0f / 12.0f) * m * (h * h + d * d);
    f32 iy = (1.0f / 12.0f) * m * (w * w + d * d);
    f32 iz = (1.0f / 12.0f) * m * (w * w + h * h);

    m_inertia = { (std::max)(1e-4f, ix), (std::max)(1e-4f, iy), (std::max)(1e-4f, iz) };
    m_invInertia = { 1.0f / m_inertia.x, 1.0f / m_inertia.y, 1.0f / m_inertia.z };
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

    // Impuls unghiular: delta_omega = I^-1 * (r x impulse)
    Vec3 r = point - m_owner->m_position;
    Vec3 angImpulse = Vec3::cross(r, impulse);
    m_angularVelocity.x += angImpulse.x * m_invInertia.x;
    m_angularVelocity.y += angImpulse.y * m_invInertia.y;
    m_angularVelocity.z += angImpulse.z * m_invInertia.z;
}

void dx3d::RigidBody::integrate(f32 dt)
{
    if (m_isStatic || !m_owner)
    {
        m_force = { 0.0f, 0.0f, 0.0f };
        m_torque = { 0.0f, 0.0f, 0.0f };
        return;
    }

    // 1. Integrare viteză liniară: v += (F / m) * dt
    m_velocity += m_force * (m_invMass * dt);

    // Amortizare liniară (drag)
    f32 linDamp = (std::max)(0.0f, 1.0f - m_linearDamping * dt);
    m_velocity *= linDamp;

    // 2. Integrare poziție: p += v * dt
    m_owner->m_position += m_velocity * dt;

    // 3. Integrare viteză unghiulară: omega += (Torque * invInertia) * dt
    m_angularVelocity.x += m_torque.x * m_invInertia.x * dt;
    m_angularVelocity.y += m_torque.y * m_invInertia.y * dt;
    m_angularVelocity.z += m_torque.z * m_invInertia.z * dt;

    // Amortizare unghiulară
    f32 angDamp = (std::max)(0.0f, 1.0f - m_angularDamping * dt);
    m_angularVelocity *= angDamp;

    // 4. Integrare rotație
    m_owner->m_rotation += m_angularVelocity * dt;

    // Resetare forțe per cadru
    m_force = { 0.0f, 0.0f, 0.0f };
    m_torque = { 0.0f, 0.0f, 0.0f };
}

dx3d::f32 dx3d::RigidBody::getKineticEnergy() const
{
    if (m_isStatic) return 0.0f;
    f32 vSq = m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y + m_velocity.z * m_velocity.z;
    f32 linKE = 0.5f * m_mass * vSq;
    f32 rotKE = 0.5f * (
        m_inertia.x * m_angularVelocity.x * m_angularVelocity.x +
        m_inertia.y * m_angularVelocity.y * m_angularVelocity.y +
        m_inertia.z * m_angularVelocity.z * m_angularVelocity.z
    );
    return linKE + rotKE;
}