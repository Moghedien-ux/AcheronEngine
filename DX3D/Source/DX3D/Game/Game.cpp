// ==========================================
// Fișier: DX3D/Source/Game.cpp
// ==========================================
#include <DX3D/Game/Game.h>
#include <DX3D/Window/Window.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Core/Logger.h>
#include <DX3D/Game/Display.h>
#include <DX3D/Physics/GameObject.h>
#include <Windows.h>

#include "DX3D/Physics/RigidBody.h"

namespace dx3d
{
    void SetGlobalInputSystem(InputSystem* input);
}

dx3d::Game::Game(const GameDesc& desc) :
    Base({ *std::make_unique<Logger>(desc.logLevel).release() }),
    m_loggerPtr(&m_logger)
{
    m_graphicsEngine = std::make_unique<GraphicsEngine>(GraphicsEngineDesc{ m_logger });
    m_display = std::make_unique<Display>(DisplayDesc{ { m_logger, desc.windowSize }, m_graphicsEngine->getGraphicsDevice() });

    m_inputSystem = std::make_unique<InputSystem>(BaseDesc{ m_logger });
    SetGlobalInputSystem(m_inputSystem.get());

    m_physicsEngine = std::make_unique<PhysicsEngine>(PhysicsEngineDesc{ { m_logger }, { 0.0f, -9.81f, 0.0f }, true, 0.0f });

    m_camera = std::make_unique<Camera>(Vec3{ 0.0f, 2.5f, -6.0f });
    f32 aspect = static_cast<f32>(desc.windowSize.width) / static_cast<f32>(desc.windowSize.height);
    m_camera->setPerspective(1.0472f, aspect, 0.1f, 100.0f);

    // Inițializăm scena cu 3 cuburi având proprietăți fizice diferite
    spawnCube({ -2.0f, 3.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 1.0f, true, { 0.2f, 0.5f, 0.9f });
    spawnCube({  0.0f, 5.0f, 0.0f }, { 1.5f, 1.5f, 1.5f }, 15.0f, true, { 0.9f, 0.3f, 0.2f });
    spawnCube({  2.0f, 2.0f, 0.0f }, { 0.8f, 0.8f, 0.8f }, 0.5f, false, { 0.8f, 0.4f, 0.9f }); // Fără gravitație

    m_lastTime = std::chrono::high_resolution_clock::now();
    DX3DLogInfo("Physics Engine Initialized. Controls: [1] Spawn Cube, [2] Heavy Cube, [G] Toggle Gravity, [+/-] Resize.");
}

dx3d::Game::~Game()
{
    SetGlobalInputSystem(nullptr);
}

void dx3d::Game::spawnCube(const Vec3& pos, const Vec3& scale, f32 mass, bool useGravity, const Vec3& color)
{
    GameObjectDesc desc;
    desc.name = "Cube_" + std::to_string(m_gameObjects.size() + 1);
    desc.position = pos;
    desc.scale = scale;
    desc.color = color;

    auto cube = std::make_shared<GameObject>(desc);
    cube->m_rigidBody->setMass(mass);
    cube->m_rigidBody->m_useGravity = useGravity;

    m_gameObjects.push_back(cube);
    m_physicsEngine->addObject(cube);
    m_selectedObject = cube;
}

void dx3d::Game::run()
{
    MSG msg{};
    while (m_isRunning)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                m_isRunning = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        auto currentTime = std::chrono::high_resolution_clock::now();
        f32 deltaTime = std::chrono::duration<f32>(currentTime - m_lastTime).count();
        m_lastTime = currentTime;

        onInternalUpdate(deltaTime);
    }
}

void dx3d::Game::onInternalUpdate(f32 deltaTime)
{
    // 1. Update Input
    m_inputSystem->update();

    // 2. Comenzi tastatură pentru crearea și configurarea cuburilor:
    if (m_inputSystem->isKeyDown('1')) // Spawn Standard Cube
    {
        spawnCube(m_camera->getPosition() + Vec3{ 0.0f, 0.0f, 3.0f }, { 1.0f, 1.0f, 1.0f }, 1.0f, true, { 0.2f, 0.6f, 1.0f });
    }
    if (m_inputSystem->isKeyDown('2')) // Spawn Heavy Cube (25kg)
    {
        spawnCube(m_camera->getPosition() + Vec3{ 0.0f, 0.0f, 3.0f }, { 1.8f, 1.8f, 1.8f }, 25.0f, true, { 1.0f, 0.6f, 0.1f });
    }
    if (m_selectedObject && m_inputSystem->isKeyDown('G')) // Toggle Gravity
    {
        m_selectedObject->m_rigidBody->m_useGravity = !m_selectedObject->m_rigidBody->m_useGravity;
    }
    if (m_selectedObject && m_inputSystem->isKeyDown(VK_OEM_PLUS)) // Redimensionare (mărire)
    {
        m_selectedObject->setScale(m_selectedObject->m_scale * 1.05f);
    }
    if (m_selectedObject && m_inputSystem->isKeyDown(VK_OEM_MINUS)) // Redimensionare (micșorare)
    {
        m_selectedObject->setScale(m_selectedObject->m_scale * 0.95f);
    }

    // 3. Update Cameră
    m_camera->update(*m_inputSystem, deltaTime);

    // 4. Update Simularea Fizică
    m_physicsEngine->update(deltaTime);

    // 5. Randare Obiecte
    m_graphicsEngine->renderObjects(m_display->getSwapChain(), *m_camera, m_gameObjects);
}