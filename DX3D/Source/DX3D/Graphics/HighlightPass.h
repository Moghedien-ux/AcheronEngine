#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Graphics/GraphicsResource.h>
#include <DX3D/Camera/Camera.h>
#include <DX3D/Physics/GameObject.h>
#include <d3d11.h>
#include <wrl.h>

namespace dx3d
{
    class DeviceContext;
    class ConstantBuffer;
    class VertexBuffer;
    class IndexBuffer;

    class HighlightPass final : public GraphicsResource
    {
    public:
        explicit HighlightPass(const GraphicsResourceDesc& gDesc);
        virtual ~HighlightPass() override = default;

        void renderHighlight(
            DeviceContext& context,
            const Camera& camera,
            const GameObject& selectedObject,
            ConstantBuffer& cb,
            const VertexBuffer& vb,
            const IndexBuffer& ib);

    private:
        Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_wireframeRS{};
        Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_highlightDSS{};
    };
}
