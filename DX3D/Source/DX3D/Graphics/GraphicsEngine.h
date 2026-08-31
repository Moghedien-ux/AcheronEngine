#pragma once

#include <DX3D/Core/Core.h>
#include <DX3D/Core/Base.h>
#include <DX3D/Core/Common.h>
#include <DX3D/Math/Mat4.h>
#include <DX3D/Camera/Camera.h>
#include <DX3D/Physics/GameObject.h>

namespace dx3d
{
    struct Vertex
    {
        Vec3 position;
        Vec3 color;
    };

    struct ConstantData
    {
        Mat4 world;
        Mat4 view;
        Mat4 proj;
    };

    class GraphicsEngine final : public Base
    {
    public:
        explicit GraphicsEngine(const GraphicsEngineDesc& desc);
        virtual ~GraphicsEngine() override;

        GraphicsDevice& getGraphicsDevice() noexcept;

        void render(SwapChain& swapChain, const Camera& camera, f32 cubeRotation = 0.0f);
        void renderObjects(SwapChain& swapChain, const Camera& camera, const std::vector<GameObjectPtr>& objects);

    private:
        std::shared_ptr<GraphicsDevice> m_graphicsDevice{};
        DeviceContextPtr m_deviceContext{};
        GraphicsPilepineStatePtr m_pipeline{};
        VertexBufferPtr m_vb{};
        IndexBufferPtr m_ib{};
        ConstantBufferPtr m_cb{};
    };
}