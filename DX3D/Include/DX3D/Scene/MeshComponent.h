#pragma once
#include <DX3D/Scene/Component.h>
#include <DX3D/Geometry/Mesh.h>
#include <DX3D/Math/Vec4.h>
#include <memory>

namespace dx3d
{
    struct MaterialDesc
    {
        Vec4 baseColor{ 0.2f, 0.6f, 1.0f, 1.0f };
        f32 roughness{ 0.4f };
        f32 metallic{ 0.0f };
        bool wireframe{ false };
    };

    class MeshComponent : public Component
    {
    public:
        MeshComponent(Actor* owner, MeshPtr mesh = nullptr);
        virtual ~MeshComponent() override = default;

        void setMesh(MeshPtr mesh) { m_mesh = mesh; }
        MeshPtr getMesh() const { return m_mesh; }

        void setMaterial(const MaterialDesc& mat) { m_material = mat; }
        MaterialDesc& getMaterial() { return m_material; }
        const MaterialDesc& getMaterial() const { return m_material; }

        void setBaseColor(const Vec4& color) { m_material.baseColor = color; }
        const Vec4& getBaseColor() const { return m_material.baseColor; }

        AABB getWorldAABB() const;

    private:
        MeshPtr m_mesh{ nullptr };
        MaterialDesc m_material{};
    };

    using MeshComponentPtr = std::shared_ptr<MeshComponent>;
}