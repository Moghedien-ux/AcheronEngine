#include <DX3D/Physics/PhysicsEngine.h>
#include <DX3D/Physics/RigidBody.h>
#include <algorithm>
#include <cmath>

dx3d::PhysicsEngine::PhysicsEngine(const PhysicsEngineDesc& desc) :
    Base(desc.base),
    m_gravity(desc.gravity),
    m_enableGroundPlane(desc.enableGroundPlane),
    m_groundY(desc.groundY)
{
}

void dx3d::PhysicsEngine::addObject(GameObjectPtr obj)
{
    if (obj)
    {
        m_objects.push_back(obj);
    }
}

void dx3d::PhysicsEngine::removeObject(GameObjectPtr obj)
{
    std::erase(m_objects, obj);
}

void dx3d::PhysicsEngine::clear()
{
    m_objects.clear();
}

void dx3d::PhysicsEngine::resolveGroundCollision(GameObject& obj)
{
    auto& rb = obj.m_rigidBody;
    if (!rb || rb->m_isStatic) return;

    f32 halfHeight = obj.m_scale.y * 0.5f;
    f32 bottomY = obj.m_position.y - halfHeight;

    if (bottomY < m_groundY)
    {
        obj.m_position.y = m_groundY + halfHeight;

        if (rb->m_velocity.y < 0.0f)
        {
            if (std::abs(rb->m_velocity.y) < 0.2f)
            {
                rb->m_velocity.y = 0.0f; // Resting contact
            }
            else
            {
                rb->m_velocity.y = -rb->m_velocity.y * rb->m_restitution;
            }

            rb->m_velocity.x *= (1.0f - rb->m_friction * 0.1f);
            rb->m_velocity.z *= (1.0f - rb->m_friction * 0.1f);
            rb->m_angularVelocity *= 0.95f;
        }
    }
}

void dx3d::PhysicsEngine::resolveObjectToBoxCollision(GameObject& a, GameObject& b)
{
    auto& rba = a.m_rigidBody;
    auto& rbb = b.m_rigidBody;

    if (!rba || !rbb) return;
    if (rba->m_isStatic && rbb->m_isStatic) return;

    Vec3 halfA = a.m_scale * 0.5f;
    Vec3 halfB = b.m_scale * 0.5f;

    Vec3 delta = b.m_position - a.m_position;
    Vec3 overlap = {
        (halfA.x + halfB.x) - std::abs(delta.x),
        (halfA.y + halfB.y) - std::abs(delta.y),
        (halfA.z + halfB.z) - std::abs(delta.z)
    };

    if (overlap.x > 0.0f && overlap.y > 0.0f && overlap.z > 0.0f)
    {
        Vec3 normal{ 0.0f, 1.0f, 0.0f };
        f32 minOverlap = overlap.y;

        if (overlap.x < minOverlap)
        {
            minOverlap = overlap.x;
            normal = { delta.x > 0.0f ? 1.0f : -1.0f, 0.0f, 0.0f };
        }
        if (overlap.z < minOverlap)
        {
            minOverlap = overlap.z;
            normal = { 0.0f, 0.0f, delta.z > 0.0f ? 1.0f : -1.0f };
        }
        if (minOverlap == overlap.y)
        {
            normal = { 0.0f, delta.y > 0.0f ? 1.0f : -1.0f, 0.0f };
        }

        f32 totalInvMass = rba->m_invMass + rbb->m_invMass;
        if (totalInvMass > 1e-6f)
        {
            Vec3 separation = normal * (minOverlap / totalInvMass);
            if (!rba->m_isStatic) a.m_position -= separation * rba->m_invMass;
            if (!rbb->m_isStatic) b.m_position += separation * rbb->m_invMass;
        }

        Vec3 relVel = rbb->m_velocity - rba->m_velocity;
        f32 velAlongNormal = Vec3::dot(relVel, normal);

        if (velAlongNormal < 0.0f)
        {
            f32 e = (rba->m_restitution + rbb->m_restitution) * 0.5f;
            f32 j = -(1.0f + e) * velAlongNormal / totalInvMass;
            Vec3 impulse = normal * j;

            if (!rba->m_isStatic) rba->applyImpulse(impulse * -1.0f);
            if (!rbb->m_isStatic) rbb->applyImpulse(impulse);
        }
    }
}

void dx3d::PhysicsEngine::update(f32 deltaTime)
{
    const i32 subSteps = 4;
    const f32 subDt = std::clamp(deltaTime, 0.001f, 0.05f) / static_cast<f32>(subSteps);

    for (i32 step = 0; step < subSteps; ++step)
    {
        for (auto& obj : m_objects)
        {
            if (!obj || !obj->m_active) continue;
            auto& rb = obj->m_rigidBody;
            if (rb && !rb->m_isStatic)
            {
                if (rb->m_useGravity)
                {
                    rb->applyForce(m_gravity * rb->m_mass);
                }
                rb->integrate(subDt);
            }
        }

        if (m_enableGroundPlane)
        {
            for (auto& obj : m_objects)
            {
                if (obj && obj->m_active)
                {
                    resolveGroundCollision(*obj);
                }
            }
        }

        for (size_t i = 0; i < m_objects.size(); ++i)
        {
            for (size_t j = i + 1; j < m_objects.size(); ++j)
            {
                if (m_objects[i] && m_objects[j])
                {
                    resolveObjectToBoxCollision(*m_objects[i], *m_objects[j]);
                }
            }
        }
    }
}
