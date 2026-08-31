#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Math/Vec3.h>
#include <algorithm>

namespace dx3d
{
    class GameObject;

    struct RigidBodyDesc
    {
        f32 mass = 1.0f;
        bool useGravity = true;
        bool isStatic = false;
        f32 restitution = 0.5f;  // Bounciness (0.0 = clay, 1.0 = superball)
        f32 friction = 0.5f;     // Coulomb friction coefficient
        f32 linearDamping = 0.03f;
        f32 angularDamping = 0.06f;
    };

    class RigidBody
    {
    public:
        explicit RigidBody(GameObject* owner, const RigidBodyDesc& desc = {});
        ~RigidBody() = default;

        // Physical Properties
        f32 m_mass{ 1.0f };
        f32 m_invMass{ 1.0f };
        bool m_useGravity{ true };
        bool m_isStatic{ false };

        f32 m_restitution{ 0.5f };
        f32 m_friction{ 0.5f };
        f32 m_linearDamping{ 0.03f };
        f32 m_angularDamping{ 0.06f };

        // State Vectors
        Vec3 m_velocity{ 0.0f, 0.0f, 0.0f };
        Vec3 m_angularVelocity{ 0.0f, 0.0f, 0.0f };
        Vec3 m_force{ 0.0f, 0.0f, 0.0f };
        Vec3 m_torque{ 0.0f, 0.0f, 0.0f };

        // Moments of Inertia (3D Cuboid Tensor)
        Vec3 m_inertia{ 1.0f, 1.0f, 1.0f };
        Vec3 m_invInertia{ 1.0f, 1.0f, 1.0f };

        void setMass(f32 newMass);
        void setStatic(bool isStatic);
        void recalculateInertia();

        void applyForce(const Vec3& force);
        void applyImpulse(const Vec3& impulse);
        void applyImpulseAtPoint(const Vec3& impulse, const Vec3& point);

        void integrate(f32 dt);
        f32 getKineticEnergy() const;

    private:
        GameObject* m_owner{ nullptr };
    };
}
