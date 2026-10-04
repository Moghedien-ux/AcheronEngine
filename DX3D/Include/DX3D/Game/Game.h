#pragma once
#include <DX3D/Core/Base.h>
#include <DX3D/Core/Core.h>
#include <DX3D/Camera/Camera.h>
#include <DX3D/Input/InputSystem.h>
#include <DX3D/Physics/PhysicsEngine.h>
#include <DX3D/Physics/GameObject.h>
#include <DX3D/UI/ImGuiLayer.h>
#include <DX3D/UI/DetailsPanel.h>
#include <DX3D/UI/WorldOutliner.h>
#include <vector>
#include <chrono>

namespace dx3d
{
    class GraphicsEngine;
    class Display;

    struct GameDesc
    {
        Rect windowSize{ 1440, 850 };
        Logger::LogLevel logLevel{ Logger::LogLevel::Info };
    };

    class Game : public Base
    {
    public:
        explicit Game(const GameDesc& desc);
        virtual ~Game() override;

        virtual void run() final;

        void spawnCube(const Vec3& pos, const Vec3& scale, f32 mass, bool useGravity, const Vec3& color);
        void selectObject(GameObjectPtr obj);
        GameObjectPtr getSelectedObject() const { return m_selectedObject; }
        std::vector<GameObjectPtr>& getGameObjects() { return m_gameObjects; }

        void setSimulating(bool sim) { m_isSimulating = sim; }
        bool isSimulating() const { return m_isSimulating; }

    private:
        void onInternalUpdate(f32 deltaTime);
        void handleObjectPicking();

    private:
        std::unique_ptr<Logger> m_loggerPtr{};
        std::unique_ptr<GraphicsEngine> m_graphicsEngine{};
        std::unique_ptr<Display> m_display{};
        std::unique_ptr<InputSystem> m_inputSystem{};
        std::unique_ptr<Camera> m_camera{};
        std::unique_ptr<PhysicsEngine> m_physicsEngine{};

        std::unique_ptr<ImGuiLayer> m_imguiLayer{};
        std::unique_ptr<DetailsPanel> m_detailsPanel{};
        std::unique_ptr<WorldOutliner> m_worldOutliner{};

        std::vector<GameObjectPtr> m_gameObjects;
        GameObjectPtr m_selectedObject{};

        std::chrono::high_resolution_clock::time_point m_lastTime{};
        bool m_isRunning{ true };
        bool m_isSimulating{ true };
        Rect m_windowSize{ 1440, 850 };
    };
}
