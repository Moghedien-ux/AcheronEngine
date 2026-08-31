#include <DX3D/Physics/PhysicsEngine.h>
#include <DX3D/Physics/RigidBody.h>
#include <cmath>
#include <algorithm>

dx3d::PhysicsEngine::PhysicsEngine(const PhysicsEngineDesc& desc) :
    Base(desc.base),
    m_gravity(desc.gravity),
    m_enableGround(desc.enableGround),
    m_groundHeight(desc.groundHeight)
{}

void dx3d::PhysicsEngine::addObject(GameObjectPtr obj)
{
    if (obj) m_objects.push_back(obj);
}

void dx3d::PhysicsEngine::removeObject(GameObject* obj)
{
    std::erase_if(m_objects, [obj](const GameObjectPtr& ptr) { return ptr.get() == obj; });
}

void dx3d::PhysicsEngine::clear()
{
    m_objects.clear();
}

void dx3d::PhysicsEngine::update(f32 deltaTime)
{
    f32 clampedDt = (std::min)(deltaTime, 0.1f);
    ui32 steps = (std::max)(1u, m_subSteps);
    f32 subDt = clampedDt / static_cast<f32>(steps);

    for (ui32 s = 0; s < steps; ++s)
    {
        subStep(subDt);
    }
}

void dx3d::PhysicsEngine::subStep(f32 dt)
{
    // 1. Aplicare gravitație (doar dacă toggle-ul de gravitație e activ pe corp)
    for (auto& obj : m_objects)
    {
        if (!obj || !obj->m_active) continue;
        auto& rb = obj->m_rigidBody;
        if (!rb || rb->m_isStatic) continue;

        if (rb->m_useGravity)
        {
            // F = m * g (forța este direct proporțională cu masa)
            rb->applyForce(m_gravity * rb->m_mass);
        }
    }

    // 2. Coliziuni cu Solul
    if (m_enableGround)
    {
        for (auto& obj : m_objects)
        {
            if (obj && obj->m_active && obj->m_rigidBody && !obj->m_rigidBody->m_isStatic)
            {
                resolveGroundCollision(obj.get(), dt);
            }
        }
    }

    // 3. Coliziuni Cub - Cub (OBB / AABB simplificat)
    for (size_t i = 0; i < m_objects.size(); ++i)
    {
        for (size_t j = i + 1; j < m_objects.size(); ++j)
        {
            auto& a = m_objects[i];
            auto& b = m_objects[j];
            if (!a || !b || !a->m_active || !b->m_active) continue;
            if (a->m_rigidBody->m_isStatic && b->m_rigidBody->m_isStatic) continue;

            resolveBoxCollision(a.get(), b.get(), dt);
        }
    }

    // 4. Integrare numerică a poziției și rotației
    for (auto& obj : m_objects)
    {
        if (obj && obj->m_active && obj->m_rigidBody)
        {
            obj->m_rigidBody->integrate(dt);
        }
    }
}

void dx3d::PhysicsEngine::resolveGroundCollision(GameObject* obj, f32 dt)
{
    auto& rb = obj->m_rigidBody;
    f32 halfY = obj->m_scale.y * 0.5f;
    f32 bottomY = obj->m_position.y - halfY;

    if (bottomY < m_groundHeight)
    {
        f32 penetration = m_groundHeight - bottomY;

        // Corecție de poziție împotriva pătrunderii în sol
        obj->m_position.y += penetration;

        // Reacțiune la contact (restituție)
        if (rb->m_velocity.y < 0.0f)
        {
            f32 restitution = (std::min)(rb->m_restitution, m_groundRestitution);
            rb->m_velocity.y = -rb->m_velocity.y * restitution;

            // Frecare orizontală
            f32 friction = (std::min)(rb->m_friction, m_groundFriction);
            f32 frictionFactor = (std::max)(0.0f, 1.0f - friction * 10.0f * dt);
            rb->m_velocity.x *= frictionFactor;
            rb->m_velocity.z *= frictionFactor;
        }
    }
}

void dx3d::PhysicsEngine::resolveBoxCollision(GameObject* a, GameObject* b, f32 dt)
{
    Vec3 diff = b->m_position - a->m_position;
    f32 minX = (a->m_scale.x + b->m_scale.x) * 0.5f;
    f32 minY = (a->m_scale.y + b->m_scale.y) * 0.5f;
    f32 minZ = (a->m_scale.z + b->m_scale.z) * 0.5f;

    f32 overlapX = minX - std::abs(diff.x);
    f32 overlapY = minY - std::abs(diff.y);
    f32 overlapZ = minZ - std::abs(diff.z);

    if (overlapX > 0.0f && overlapY > 0.0f && overlapZ > 0.0f)
    {
        Vec3 normal{ 0.0f, 1.0f, 0.0f };
        f32 penetration = overlapY;

        if (overlapX < overlapY && overlapX < overlapZ)
        {
            normal = { diff.x > 0 ? 1.0f : -1.0f, 0.0f, 0.0f };
            penetration = overlapX;
        }
        else if (overlapZ < overlapY && overlapZ < overlapX)
        {
            normal = { 0.0f, 0.0f, diff.z > 0 ? 1.0f : -1.0f };
            penetration = overlapZ;
        }
        else
        {
            normal = { 0.0f, diff.y > 0 ? 1.0f : -1.0f, 0.0f };
            penetration = overlapY;
        }

        auto& rbA = a->m_rigidBody;
        auto& rbB = b->m_rigidBody;

        Vec3 relVel = rbB->m_velocity - rbA->m_velocity;
        f32 normalVel = Vec3::dot(relVel, normal);
        if (normalVel > 0.0f) return;

        f32 e = (std::min)(rbA->m_restitution, rbB->m_restitution);
        f32 totalInvMass = rbA->m_invMass + rbB->m_invMass;
        if (totalInvMass <= 0.0f) return;

        // Impuls normal
        f32 j = -(1.0f + e) * normalVel / totalInvMass;
        Vec3 impulse = normal * j;

        rbA->applyImpulse(impulse * -1.0f);
        rbB->applyImpulse(impulse);

        // Corecție Baumgarte pentru separare
        f32 percent = 0.5f;
        Vec3 correction = normal * (penetration * percent / totalInvMass);
        if (!rbA->m_isStatic) a->m_position -= correction * rbA->m_invMass;
        if (!rbB->m_isStatic) b->m_position += correction * rbB->m_invMass;
    }
}