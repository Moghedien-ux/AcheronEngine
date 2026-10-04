#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Math/Vec3.h>
#include <DX3D/Math/Mat4.h>
#include <string>
#include <memory>

namespace dx3d
{
    class RigidBody;

    struct GameObjectDesc
    {
        std::string name = "CubeActor";
        Vec3 position{ 0.0f, 0.0f, 0.0f };
        Vec3 rotation{ 0.0f, 0.0f, 0.0f };
        Vec3 scale{ 1.0f, 1.0f, 1.0f };
        Vec3 color{ 0.2f, 0.6f, 1.0f };
    };

    class GameObject
    {
    public:
        explicit GameObject(const GameObjectDesc& desc = {});
        virtual ~GameObject() = default;

        // Transform & Entity Properties
        Vec3 m_position{ 0.0f, 0.0f, 0.0f };
        Vec3 m_rotation{ 0.0f, 0.0f, 0.0f };
        Vec3 m_scale{ 1.0f, 1.0f, 1.0f };
        Vec3 m_color{ 0.2f, 0.6f, 1.0f };
        std::string m_name{ "GameObject" };
        bool m_active{ true };
        bool m_selected{ false };

        std::shared_ptr<RigidBody> m_rigidBody{};

        virtual Mat4 getWorldMatrix() const;

        virtual void setScale(const Vec3& newScale);
    };

    using GameObjectPtr = std::shared_ptr<GameObject>;
}