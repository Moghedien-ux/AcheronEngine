#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Physics/GameObject.h>
#include <DX3D/Physics/RigidBody.h>
#include <functional>

namespace dx3d
{
    class Game;

    class DetailsPanel
    {
    public:
        DetailsPanel() = default;
        ~DetailsPanel() = default;

        using SpawnCubeCallback = std::function<void(const Vec3& pos, const Vec3& scale, f32 mass, bool useGravity, const Vec3& color)>;
        void setSpawnCallback(SpawnCubeCallback callback) { m_spawnCallback = callback; }

        void render(GameObject* selectedObject, Game* gameContext);

        bool isVisible() const { return m_isOpen; }
        void setVisible(bool visible) { m_isOpen = visible; }

    private:
        void renderTransformSection(GameObject* obj);
        void renderPhysicsSection(GameObject* obj);
        void renderMaterialSection(GameObject* obj);
        void renderQuickActionsSection(GameObject* obj, Game* gameContext);

    private:
        bool m_isOpen{ true };
        bool m_uniformScale{ true };
        SpawnCubeCallback m_spawnCallback{};
    };
}
