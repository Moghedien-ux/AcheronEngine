#include <DX3D/Scene/MeshComponent.h>
#include <DX3D/Scene/Actor.h>

using namespace dx3d;

MeshComponent::MeshComponent(Actor* owner, MeshPtr mesh)
    : Component(owner, "MeshComponent"), m_mesh(mesh)
{
}

AABB MeshComponent::getWorldAABB() const
{
    if (!m_mesh || !m_owner) return AABB{};

    const AABB& local = m_mesh->getBounds();
    Transform& t = m_owner->getTransform();

    Vec3 corners[8] = {
        { local.minBounds.x, local.minBounds.y, local.minBounds.z },
        { local.maxBounds.x, local.minBounds.y, local.minBounds.z },
        { local.minBounds.x, local.maxBounds.y, local.minBounds.z },
        { local.maxBounds.x, local.maxBounds.y, local.minBounds.z },
        { local.minBounds.x, local.minBounds.y, local.maxBounds.z },
        { local.maxBounds.x, local.minBounds.y, local.maxBounds.z },
        { local.minBounds.x, local.maxBounds.y, local.maxBounds.z },
        { local.maxBounds.x, local.maxBounds.y, local.maxBounds.z },
    };

    AABB worldBounds{};
    for (int i = 0; i < 8; ++i)
    {
        Vec3 worldCorner = t.transformPoint(corners[i]);
        worldBounds.minBounds.x = (std::min)(worldBounds.minBounds.x, worldCorner.x);
        worldBounds.minBounds.y = (std::min)(worldBounds.minBounds.y, worldCorner.y);
        worldBounds.minBounds.z = (std::min)(worldBounds.minBounds.z, worldCorner.z);

        worldBounds.maxBounds.x = (std::max)(worldBounds.maxBounds.x, worldCorner.x);
        worldBounds.maxBounds.y = (std::max)(worldBounds.maxBounds.y, worldCorner.y);
        worldBounds.maxBounds.z = (std::max)(worldBounds.maxBounds.z, worldCorner.z);
    }

    return worldBounds;
}