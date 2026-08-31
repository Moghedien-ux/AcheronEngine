// ==========================================
// Fișier: DX3D/Include/DX3D/Physics/GameObject.h
// ==========================================
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
        std::string name = "GameObject";
        Vec3 position{ 0.0f, 0.0f, 0.0f };
        Vec3 rotation{ 0.0f, 0.0f, 0.0f }; // Unghiuri Euler (radiani)
        Vec3 scale{ 1.0f, 1.0f, 1.0f };    // Dimensiuni (lățime, înălțime, adâncime)
        Vec3 color{ 0.2f, 0.5f, 0.9f };
    };

    class GameObject
    {
    public:
        explicit GameObject(const GameObjectDesc& desc = {});
        virtual ~GameObject() = default;

        // Proprietăți de Transformare
        Vec3 m_position{ 0.0f, 0.0f, 0.0f };
        Vec3 m_rotation{ 0.0f, 0.0f, 0.0f };
        Vec3 m_scale{ 1.0f, 1.0f, 1.0f };
        Vec3 m_color{ 1.0f, 1.0f, 1.0f };
        std::string m_name;
        bool m_active{ true };

        // Componenta de fizică atașată
        std::shared_ptr<RigidBody> m_rigidBody{};

        // Matricea de transformare Lume (World Matrix: Scale * RotX * RotY * RotZ * Trans)
        virtual Mat4 getWorldMatrix() const;

        // Redimensionare
        virtual void setScale(const Vec3& newScale);
    };

    using GameObjectPtr = std::shared_ptr<GameObject>;
}