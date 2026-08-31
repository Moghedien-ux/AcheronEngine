#include <DX3D/Game/Game.h>
#include <DX3D/Window/Window.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Core/Logger.h>
#include <DX3D/Game/Display.h>
#include <DX3D/Physics/GameObject.h>
#include <DX3D/Physics/RigidBody.h>
#include <DX3D/Physics/Raycast.h>
#include <Windows.h>
#include <imgui.h>

namespace dx3d
{
    void SetGlobalInputSystem(InputSystem* input);
}

dx3d::Game::Game(const GameDesc& desc) :
    Base(BaseDesc{ *std::make_unique<Logger>(desc.logLevel).release() }),
    m_loggerPtr(&m_logger),
    m_windowSize(desc.windowSize)
{
    m_graphicsEngine = std::make_unique<GraphicsEngine>(GraphicsEngineDesc{ BaseDesc{ m_logger } });
    
    DisplayDesc dDesc{
        WindowDesc{
            BaseDesc{ m_logger },
            desc.windowSize,
            L"DX3D Game Engine - Unreal Style Inspector (DirectX 11)"
        },
        m_graphicsEngine->getGraphicsDevice()
    };
    m_display = std::make_unique<Display>(dDesc);

    m_inputSystem = std::make_unique<InputSystem>(BaseDesc{ m_logger });
    SetGlobalInputSystem(m_inputSystem.get());

    m_physicsEngine = std::make_unique<PhysicsEngine>(PhysicsEngineDesc{ BaseDesc{ m_logger }, { 0.0f, -9.81f, 0.0f }, true, 0.0f });

    // Initialize Camera
    m_camera = std::make_unique<Camera>(Vec3{ 0.0f, 3.5f, -8.0f });
    f32 aspect = static_cast<f32>(desc.windowSize.width) / static_cast<f32>(desc.windowSize.height);
    m_camera->setPerspective(1.0472f, aspect, 0.1f, 500.0f);

    // Initialize ImGui UI Subsystems
    HWND hwnd = static_cast<HWND>(m_display->getHandle());
    m_imguiLayer = std::make_unique<ImGuiLayer>(BaseDesc{ m_logger }, hwnd,
        &m_graphicsEngine->getD3DDevice(),
        &m_graphicsEngine->getD3DDeviceContext());

    m_detailsPanel = std::make_unique<DetailsPanel>();
    m_detailsPanel->setSpawnCallback([this](const Vec3& pos, const Vec3& scale, f32 mass, bool useGravity, const Vec3& color) {
        spawnCube(pos, scale, mass, useGravity, color);
    });

    m_worldOutliner = std::make_unique<WorldOutliner>();

    // Initial Scene Cubes: Platform slab + Standard cube + Heavy block + Bouncy cube
    spawnCube({  0.0f, 0.2f, 0.0f }, { 8.0f, 0.4f, 8.0f }, 100.0f, false, { 0.2f, 0.25f, 0.32f });
    m_gameObjects.back()->m_rigidBody->setStatic(true);
    m_gameObjects.back()->m_name = "Platform_Ground";

    spawnCube({ -2.2f, 3.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 1.0f, true, { 0.2f, 0.7f, 1.0f });
    m_gameObjects.back()->m_name = "Cube_Standard";

    spawnCube({  0.0f, 5.5f, 0.0f }, { 1.8f, 1.8f, 1.8f }, 25.0f, true, { 1.0f, 0.45f, 0.1f });
    m_gameObjects.back()->m_name = "Cube_HeavyBlock";

    spawnCube({  2.4f, 3.0f, 0.0f }, { 1.2f, 1.2f, 1.2f }, 2.0f, true, { 0.3f, 0.9f, 0.4f });
    m_gameObjects.back()->m_name = "Cube_Bouncy";
    m_gameObjects.back()->m_rigidBody->m_restitution = 0.85f;

    m_selectedObject = m_gameObjects[2]; // Select Heavy Block by default

    m_lastTime = std::chrono::high_resolution_clock::now();
    DX3DLogInfo("DX3D Engine & Unreal-Style Details Panel Ready.");
}

dx3d::Game::~Game()
{
    SetGlobalInputSystem(nullptr);
}

void dx3d::Game::spawnCube(const Vec3& pos, const Vec3& scale, f32 mass, bool useGravity, const Vec3& color)
{
    GameObjectDesc desc;
    desc.name = "CubeActor_" + std::to_string(m_gameObjects.size() + 1);
    desc.position = pos;
    desc.scale = scale;
    desc.color = color;

    auto cube = std::make_shared<GameObject>(desc);
    cube->m_rigidBody->setMass(mass);
    cube->m_rigidBody->m_useGravity = useGravity;

    m_gameObjects.push_back(cube);
    m_physicsEngine->addObject(cube);
    m_selectedObject = cube; // Automatically select newly spawned cube
}

void dx3d::Game::selectObject(GameObjectPtr obj)
{
    m_selectedObject = obj;
}

void dx3d::Game::handleObjectPicking()
{
    ImGuiIO& io = ImGui::GetIO();
    if (m_inputSystem->isMouseButtonDown(0) && !io.WantCaptureMouse)
    {
        Point mousePos = m_inputSystem->getMousePosition();
        Ray pickRay = Raycast::screenPointToRay(
            mousePos.x, mousePos.y,
            m_windowSize.width, m_windowSize.height,
            m_camera->getViewMatrix(),
            m_camera->getProjectionMatrix(),
            m_camera->getPosition()
        );

        RaycastHit hit = Raycast::pickObject(pickRay, m_gameObjects);
        if (hit.hasHit)
        {
            for (auto& obj : m_gameObjects)
            {
                if (obj.get() == hit.hitObject)
                {
                    m_selectedObject = obj;
                    break;
                }
            }
        }
    }
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

        if (!m_isRunning) break;

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

    // 2. Object Picking via Mouse Click
    handleObjectPicking();

    // 3. Update Camera (Right-click drag to look / WASD)
    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse)
    {
        m_camera->update(*m_inputSystem, deltaTime);
    }

    // 4. Update Physics Simulation
    if (m_isSimulating)
    {
        m_physicsEngine->update(deltaTime);
    }

    // 5. Begin ImGui Frame
    m_imguiLayer->beginFrame();

    // 6. Render Unreal-Style Details Panel & World Outliner
    m_detailsPanel->render(m_selectedObject.get(), this);
    m_worldOutliner->render(m_gameObjects, m_selectedObject, this);

    // 7. Render 3D Scene + Selection Outline
    m_graphicsEngine->renderObjects(m_display->getSwapChain(), *m_camera, m_gameObjects, m_selectedObject.get());

    // 8. End ImGui Frame & Present
    m_imguiLayer->endFrame();
    m_display->getSwapChain().present();
}
