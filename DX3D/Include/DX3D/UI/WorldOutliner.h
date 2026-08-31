#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Physics/GameObject.h>
#include <vector>
#include <memory>

namespace dx3d
{
    class Game;

    class WorldOutliner
    {
    public:
        WorldOutliner() = default;

        void render(
            std::vector<GameObjectPtr>& objects,
            GameObjectPtr& selectedObject,
            Game* gameContext);

        bool isVisible() const { return m_isOpen; }
        void setVisible(bool visible) { m_isOpen = visible; }

    private:
        bool m_isOpen{ true };
        char m_searchFilter[64]{ 0 };
    };
}
