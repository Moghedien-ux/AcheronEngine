#pragma once
#include <DX3D/Core/Base.h>
#include <DX3D/Core/Core.h>
#include <DX3D/Math/Vec3.h>
#include <DX3D/Physics/GameObject.h>
#include <vector>
#include <memory>

namespace dx3d
{
    struct PhysicsEngineDesc
    {
        BaseDesc base;
        Vec3 gravity{ 0.0f, -9.81f, 0.0f };
        bool enableGroundPlane{ true };
        f32 groundY{ 0.0f };
    };

    class PhysicsEngine final : public Base
    {
    public:
        explicit PhysicsEngine(const PhysicsEngineDesc& desc);
        virtual ~PhysicsEngine() override = default;

        void addObject(GameObjectPtr obj);
        void removeObject(GameObjectPtr obj);
        void clear();

        void update(f32 deltaTime);

        Vec3 getGravity() const { return m_gravity; }
        void setGravity(const Vec3& g) { m_gravity = g; }

    private:
        void resolveGroundCollision(GameObject& obj);
        void resolveObjectToBoxCollision(GameObject& a, GameObject& b);

    private:
        Vec3 m_gravity{ 0.0f, -9.81f, 0.0f };
        bool m_enableGroundPlane{ true };
        f32 m_groundY{ 0.0f };
        std::vector<GameObjectPtr> m_objects;
    };
}
