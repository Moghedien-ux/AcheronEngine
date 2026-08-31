#pragma once
#include <DX3D/Graphics/GraphicsResource.h>
#include <wrl.h>

namespace dx3d
{
    class VertexBuffer;
    class IndexBuffer;
    class ConstantBuffer;
    class GraphicsPipelineState;
    class SwapChain;

    struct DeviceContextDesc
    {
        GraphicsResourceDesc gResource;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
    };

    class DeviceContext final : public GraphicsResource
    {
    public:
        explicit DeviceContext(const DeviceContextDesc& desc);
        virtual ~DeviceContext() override = default;

        ID3D11DeviceContext* getD3DContext() const { return m_context.Get(); }

        void setViewport(const Rect& size);
        void setPipelineState(const GraphicsPipelineState& state);
        void setVertexBuffer(const VertexBuffer& vb);
        void setIndexBuffer(const IndexBuffer& ib);
        void setConstantBuffer(const ConstantBuffer& cb, u32 slot = 0);

        void drawIndexed(u32 indexCount, u32 startIndexLocation = 0, i32 baseVertexLocation = 0);

    private:
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context{};
    };
}
