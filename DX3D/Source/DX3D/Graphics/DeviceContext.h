#pragma once
#include <DX3D/Graphics/GraphicsResource.h>
#include <DX3D/Math/Vec4.h>
#include <DX3D/Math/Rect.h>

namespace dx3d
{
    class DeviceContext final : public GraphicsResource
    {
    public:
        explicit DeviceContext(const GraphicsResourceDesc& gDesc);
        void clearAndSetBackBuffer(const SwapChain& swapChain, const Vec4& color);
        void setGraphicsPipelineState(const GraphicsPipelineState& pipeline);
        void setVertexBuffer(const VertexBuffer& buffer);
        void setIndexBuffer(const IndexBuffer& buffer);
        void setConstantBuffer(const ConstantBuffer& buffer, ui32 slot = 0);
        void setViewportSize(const Rect& size);
        void drawTriangleList(ui32 vertexCount, ui32 startVertexLocation);
        void drawIndexedTriangleList(ui32 indexCount, ui32 startIndexLocation, ui32 baseVertexLocation);

    private:
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context{};

        friend class GraphicsDevice;
    };
}

