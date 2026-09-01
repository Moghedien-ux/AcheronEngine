#include <DX3D/Scene/Actor.h>
#include <DX3D/Physics/RigidBody.h>
#include <algorithm>

using namespace dx3d;

Actor::Actor(const std::string& name) : m_name(name)
{
    m_meshComponent = std::make_shared<MeshComponent>(this);
    m_rigidBody = nullptr; // Initialized when physics is added or via setRigidBody()
}

void Actor::attachTo(Actor* parent, bool keepWorld)
{
    if (m_parentActor == parent || parent == this) return;

    if (m_parentActor)
    {
        auto it = std::find(m_parentActor->m_childActors.begin(), m_parentActor->m_childActors.end(), this);
        if (it != m_parentActor->m_childActors.end())
        {
            m_parentActor->m_childActors.erase(it);
        }
    }

    m_parentActor = parent;

    if (m_parentActor)
    {
        m_parentActor->m_childActors.push_back(this);
        m_transform.setParent(&m_parentActor->getTransform(), keepWorld);
    }
    else
    {
        m_transform.setParent(nullptr, keepWorld);
    }
}

void Actor::detach()
{
    attachTo(nullptr, true);
}