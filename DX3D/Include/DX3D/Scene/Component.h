#pragma once
#include <DX3D/Core/Core.h>
#include <string>

namespace dx3d
{
    class Actor;

    class Component
    {
    public:
        explicit Component(Actor* owner, const std::string& name = "Component")
            : m_owner(owner), m_name(name) {}
        virtual ~Component() = default;

        virtual void initialize() {}
        virtual void update(f32 deltaTime) { (void)deltaTime; }

        Actor* getOwner() const { return m_owner; }
        const std::string& getName() const { return m_name; }
        bool isEnabled() const { return m_enabled; }
        void setEnabled(bool enabled) { m_enabled = enabled; }

    protected:
        Actor* m_owner{ nullptr };
        std::string m_name;
        bool m_enabled{ true };
    };
}