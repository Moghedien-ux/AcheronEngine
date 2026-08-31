// ==========================================
// Fișier: DX3D/Game/Game.h
// ==========================================
#pragma once
#include <DX3D/Core/Base.h>
#include <DX3D/Core/Core.h>
#include <DX3D/Camera/Camera.h>
#include <DX3D/Input/InputSystem.h>
#include <DX3D/Physics/PhysicsEngine.h>
#include <DX3D/Physics/GameObject.h>
#include <vector>
#include <chrono>

namespace dx3d
{
    class Game : public Base
    {
    public:
        explicit Game(const GameDesc& desc);
        virtual ~Game() override;

        virtual void run() final;

    private:
        void onInternalUpdate(f32 deltaTime);
        void spawnCube(const Vec3& pos, const Vec3& scale, f32 mass, bool useGravity, const Vec3& color);

    private:
        std::unique_ptr<Logger> m_loggerPtr{};
        std::unique_ptr<GraphicsEngine> m_graphicsEngine{};
        std::unique_ptr<Display> m_display{};
        std::unique_ptr<InputSystem> m_inputSystem{};
        std::unique_ptr<Camera> m_camera{};
        std::unique_ptr<PhysicsEngine> m_physicsEngine{};

        std::vector<GameObjectPtr> m_gameObjects;
        GameObjectPtr m_selectedObject{};

        std::chrono::high_resolution_clock::time_point m_lastTime{};
        bool m_isRunning{ true };
    };
}