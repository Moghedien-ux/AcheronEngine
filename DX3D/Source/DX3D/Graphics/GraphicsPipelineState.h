#pragma once
#include <DX3D/Graphics/GraphicsResource.h>
#include <DX3D/Graphics/ShaderBinary.h>
#include <wrl.h>

namespace dx3d
{
    struct GraphicsPipelineStateDesc
    {
        GraphicsResourceDesc gResource;
        const ShaderBinary& vsBinary;
        const ShaderBinary& psBinary;
    };

    class GraphicsPipelineState final : public GraphicsResource
    {
    public:
        explicit GraphicsPipelineState(const GraphicsPipelineStateDesc& desc);
        virtual ~GraphicsPipelineState() override = default;

        ID3D11VertexShader* getVertexShader() const { return m_vs.Get(); }
        ID3D11PixelShader* getPixelShader() const { return m_ps.Get(); }
        ID3D11InputLayout* getInputLayout() const { return m_inputLayout.Get(); }

    private:
        Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vs{};
        Microsoft::WRL::ComPtr<ID3D11PixelShader> m_ps{};
        Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout{};
    };

    using GraphicsPipelineStatePtr = std::shared_ptr<GraphicsPipelineState>;
}
