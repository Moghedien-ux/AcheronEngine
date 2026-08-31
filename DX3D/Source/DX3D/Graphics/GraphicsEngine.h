#pragma once
#include <DX3D/Core/Base.h>
#include <DX3D/Graphics/GraphicsDevice.h>
#include <DX3D/Graphics/GraphicsPipelineState.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Graphics/IndexBuffer.h>
#include <DX3D/Graphics/ConstantBuffer.h>
#include <DX3D/Graphics/HighlightPass.h>
#include <DX3D/Camera/Camera.h>
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Physics/GameObject.h>
#include <vector>
#include <memory>

namespace dx3d
{
    struct GraphicsEngineDesc
    {
        BaseDesc base;
    };

    class GraphicsEngine final : public Base
    {
    public:
        explicit GraphicsEngine(const GraphicsEngineDesc& desc);
        virtual ~GraphicsEngine() override = default;

        GraphicsDevice& getGraphicsDevice() { return *m_graphicsDevice; }
        ID3D11Device& getD3DDevice() { return m_graphicsDevice->getD3DDevice(); }
        ID3D11DeviceContext& getD3DDeviceContext() { return m_graphicsDevice->getD3DDeviceContext(); }

        void renderObjects(
            SwapChain& swapChain,
            const Camera& camera,
            const std::vector<GameObjectPtr>& objects,
            GameObject* selectedObject = nullptr);

    private:
        void createCubeGeometry();
        void createPipeline();

    private:
        std::unique_ptr<GraphicsDevice> m_graphicsDevice{};
        std::unique_ptr<VertexBuffer> m_cubeVB{};
        std::unique_ptr<IndexBuffer> m_cubeIB{};
        std::unique_ptr<ConstantBuffer> m_constantBuffer{};
        std::unique_ptr<HighlightPass> m_highlightPass{};
        GraphicsPipelineStatePtr m_pipelineState{};
    };
}
