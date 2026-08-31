#pragma once
#include <DX3D/Graphics/GraphicsResource.h>
#include <d3dcompiler.h>

namespace dx3d
{
    class GraphicsPipelineState final : GraphicsResource
    {
    public:
        explicit GraphicsPipelineState(const GraphicsPipelineStateDesc& desc, const GraphicsResourceDesc& gDesc);

    private:
        Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vs{};
        Microsoft::WRL::ComPtr<ID3D11PixelShader> m_ps{};
        Microsoft::WRL::ComPtr<ID3D11InputLayout> m_layout{};
        friend class DeviceContext;
    };
}

