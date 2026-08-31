// ==========================================
// Fișier: DX3D/Include/DX3D/Physics/PhysicsEngine.h
// ==========================================
#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Core/Base.h>
#include <DX3D/Physics/GameObject.h>
#include <vector>

namespace dx3d
{
    struct CollisionContact
    {
        GameObject* bodyA{ nullptr };
        GameObject* bodyB{ nullptr };
        Vec3 point{};
        Vec3 normal{}; // Dinspre A spre B
        f32 penetration{ 0.0f };
    };

    struct PhysicsEngineDesc
    {
        BaseDesc base;
        Vec3 gravity{ 0.0f, -9.81f, 0.0f };
        bool enableGround{ true };
        f32 groundHeight{ 0.0f };
    };

    class PhysicsEngine : public Base
    {
    public:
        explicit PhysicsEngine(const PhysicsEngineDesc& desc);
        ~PhysicsEngine() = default;

        void addObject(GameObjectPtr obj);
        void removeObject(GameObject* obj);
        void clear();

        void update(f32 deltaTime);

        Vec3 m_gravity{ 0.0f, -9.81f, 0.0f };
        bool m_enableGround{ true };
        f32 m_groundHeight{ 0.0f };
        f32 m_groundRestitution{ 0.4f };
        f32 m_groundFriction{ 0.5f };

        ui32 m_subSteps{ 4 };

    private:
        void subStep(f32 dt);
        void resolveGroundCollision(GameObject* obj, f32 dt);
        void resolveBoxCollision(GameObject* a, GameObject* b, f32 dt);

    private:
        std::vector<GameObjectPtr> m_objects;
    };
}