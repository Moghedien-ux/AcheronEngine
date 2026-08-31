#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Core/Common.h>

dx3d::GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc) :
    Base(desc.base)
{
    m_graphicsDevice = std::make_unique<GraphicsDevice>(GraphicsDeviceDesc{ desc.base });
    createCubeGeometry();
    createPipeline();

    m_constantBuffer = std::make_unique<ConstantBuffer>(ConstantBufferDesc{
        { desc.base, m_graphicsDevice->getD3DDevice() },
        sizeof(ConstantData)
    });

    m_highlightPass = std::make_unique<HighlightPass>(GraphicsResourceDesc{
        desc.base,
        m_graphicsDevice->getD3DDevice()
    });
}

void dx3d::GraphicsEngine::createCubeGeometry()
{
    // Standard 1x1x1 cube vertex data with per-face normals
    Vertex vertices[] = {
        // Front Face (Z = -0.5)
        { { -0.5f, -0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f }, { 1.0f, 1.0f, 1.0f } },
        { { -0.5f,  0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f }, { 1.0f, 1.0f, 1.0f } },
        { {  0.5f,  0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f }, { 1.0f, 1.0f, 1.0f } },
        { {  0.5f, -0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f }, { 1.0f, 1.0f, 1.0f } },

        // Back Face (Z = +0.5)
        { {  0.5f, -0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f }, { 1.0f, 1.0f, 1.0f } },
        { {  0.5f,  0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f }, { 1.0f, 1.0f, 1.0f } },
        { { -0.5f,  0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f }, { 1.0f, 1.0f, 1.0f } },
        { { -0.5f, -0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f }, { 1.0f, 1.0f, 1.0f } },

        // Top Face (Y = +0.5)
        { { -0.5f,  0.5f, -0.5f }, {  0.0f,  1.0f,  0.0f }, { 1.0f, 1.0f, 1.0f } },
        { { -0.5f,  0.5f,  0.5f }, {  0.0f,  1.0f,  0.0f }, { 1.0f, 1.0f, 1.0f } },
        { {  0.5f,  0.5f,  0.5f }, {  0.0f,  1.0f,  0.0f }, { 1.0f, 1.0f, 1.0f } },
        { {  0.5f,  0.5f, -0.5f }, {  0.0f,  1.0f,  0.0f }, { 1.0f, 1.0f, 1.0f } },

        // Bottom Face (Y = -0.5)
        { { -0.5f, -0.5f,  0.5f }, {  0.0f, -1.0f,  0.0f }, { 1.0f, 1.0f, 1.0f } },
        { { -0.5f, -0.5f, -0.5f }, {  0.0f, -1.0f,  0.0f }, { 1.0f, 1.0f, 1.0f } },
        { {  0.5f, -0.5f, -0.5f }, {  0.0f, -1.0f,  0.0f }, { 1.0f, 1.0f, 1.0f } },
        { {  0.5f, -0.5f,  0.5f }, {  0.0f, -1.0f,  0.0f }, { 1.0f, 1.0f, 1.0f } },

        // Left Face (X = -0.5)
        { { -0.5f, -0.5f,  0.5f }, { -1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f, 1.0f } },
        { { -0.5f,  0.5f,  0.5f }, { -1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f, 1.0f } },
        { { -0.5f,  0.5f, -0.5f }, { -1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f, 1.0f } },
        { { -0.5f, -0.5f, -0.5f }, { -1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f, 1.0f } },

        // Right Face (X = +0.5)
        { {  0.5f, -0.5f, -0.5f }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f, 1.0f } },
        { {  0.5f,  0.5f, -0.5f }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f, 1.0f } },
        { {  0.5f,  0.5f,  0.5f }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f, 1.0f } },
        { {  0.5f, -0.5f,  0.5f }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f, 1.0f } },
    };

    u32 indices[] = {
        0, 1, 2, 0, 2, 3,       // Front
        4, 5, 6, 4, 6, 7,       // Back
        8, 9, 10, 8, 10, 11,    // Top
        12, 13, 14, 12, 14, 15, // Bottom
        16, 17, 18, 16, 18, 19, // Left
        20, 21, 22, 20, 22, 23  // Right
    };

    m_cubeVB = std::make_unique<VertexBuffer>(VertexBufferDesc{
        { BaseDesc{ m_logger }, m_graphicsDevice->getD3DDevice() },
        vertices,
        sizeof(Vertex),
        24
    });

    m_cubeIB = std::make_unique<IndexBuffer>(IndexBufferDesc{
        { BaseDesc{ m_logger }, m_graphicsDevice->getD3DDevice() },
        indices,
        36
    });
}

void dx3d::GraphicsEngine::createPipeline()
{
    // Try to load shader from Shaders/CubeShader.hlsl or DX3D/Shaders/CubeShader.hlsl
    ShaderBinary vs = m_graphicsDevice->compileShader(L"DX3D/Shaders/CubeShader.hlsl", "VSMain", "vs_5_0");
    ShaderBinary ps = m_graphicsDevice->compileShader(L"DX3D/Shaders/CubeShader.hlsl", "PSMain", "ps_5_0");

    m_pipelineState = std::make_shared<GraphicsPipelineState>(GraphicsPipelineStateDesc{
        { BaseDesc{ m_logger }, m_graphicsDevice->getD3DDevice() },
        vs,
        ps
    });
}

void dx3d::GraphicsEngine::renderObjects(
    SwapChain& swapChain,
    const Camera& camera,
    const std::vector<GameObjectPtr>& objects,
    GameObject* selectedObject)
{
    DeviceContext& context = m_graphicsDevice->getDeviceContext();

    // Clear Screen (Unreal Slate background #18191c)
    swapChain.clear(context, 0.09f, 0.10f, 0.12f, 1.0f);
    context.setViewport(swapChain.getSize());
    context.setPipelineState(*m_pipelineState);

    // 1. Draw solid scene objects
    for (const auto& obj : objects)
    {
        if (!obj || !obj->m_active) continue;

        ConstantData cData{};
        cData.world = obj->getWorldMatrix();
        cData.view = camera.getViewMatrix();
        cData.proj = camera.getProjectionMatrix();
        cData.customColor = { obj->m_color.x, obj->m_color.y, obj->m_color.z, 1.0f };
        cData.highlightParams = { (selectedObject == obj.get()) ? 1.0f : 0.0f, 0.0f, 0.0f, 0.0f };

        m_constantBuffer->update(context, &cData);

        context.setVertexBuffer(*m_cubeVB);
        context.setIndexBuffer(*m_cubeIB);
        context.setConstantBuffer(*m_constantBuffer, 0);

        context.drawIndexed(m_cubeIB->getIndexCount());
    }

    // 2. Draw selection highlight cage if an actor is selected
    if (selectedObject && selectedObject->m_active && m_highlightPass)
    {
        m_highlightPass->renderHighlight(
            context,
            camera,
            *selectedObject,
            *m_constantBuffer,
            *m_cubeVB,
            *m_cubeIB
        );
    }
}
