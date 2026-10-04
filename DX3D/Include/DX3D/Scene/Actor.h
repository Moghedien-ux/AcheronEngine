#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Math/Transform.h>
#include <DX3D/Scene/MeshComponent.h>
#include <vector>
#include <memory>
#include <string>

namespace dx3d
{
    class RigidBody;

    class Actor
    {
    public:
        explicit Actor(const std::string& name = "Actor");
        virtual ~Actor() = default;

        Transform& getTransform() { return m_transform; }
        const Transform& getTransform() const { return m_transform; }

        void attachTo(Actor* parent, bool keepWorld = true);
        void detach();
        Actor* getParent() const { return m_parentActor; }
        const std::vector<Actor*>& getChildren() const { return m_childActors; }

        void setMeshComponent(MeshComponentPtr meshComp) { m_meshComponent = meshComp; }
        MeshComponentPtr getMeshComponent() const { return m_meshComponent; }

        void setRigidBody(std::shared_ptr<RigidBody> rb) { m_rigidBody = rb; }
        std::shared_ptr<RigidBody> getRigidBody() const { return m_rigidBody; }

        const std::string& getName() const { return m_name; }
        void setName(const std::string& name) { m_name = name; }
        bool isActive() const { return m_active; }
        void setActive(bool active) { m_active = active; }
        bool isSelected() const { return m_selected; }
        void setSelected(bool sel) { m_selected = sel; }

        Mat4 getWorldMatrix() const { return m_transform.getWorldMatrix(); }

    private:
        std::string m_name;
        Transform m_transform{};
        Actor* m_parentActor{ nullptr };
        std::vector<Actor*> m_childActors;

        MeshComponentPtr m_meshComponent{ nullptr };
        std::shared_ptr<RigidBody> m_rigidBody{ nullptr };

        bool m_active{ true };
        bool m_selected{ false };
    };

    using ActorPtr = std::shared_ptr<Actor>;
}