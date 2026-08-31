#include <DX3D/Graphics/GraphicsPipelineState.h>
#include <DX3D/Graphics/GraphicsLogUtils.h>

dx3d::GraphicsPipelineState::GraphicsPipelineState(const GraphicsPipelineStateDesc& desc) :
    GraphicsResource(desc.gResource)
{
    // 1. Create Vertex Shader
    DX3DGraphicsLogThrowOnFail(
        m_device.CreateVertexShader(desc.vsBinary.getBufferPointer(), desc.vsBinary.getBufferSize(), nullptr, &m_vs),
        "CreateVertexShader failed."
    );

    // 2. Create Pixel Shader
    DX3DGraphicsLogThrowOnFail(
        m_device.CreatePixelShader(desc.psBinary.getBufferPointer(), desc.psBinary.getBufferSize(), nullptr, &m_ps),
        "CreatePixelShader failed."
    );

    // 3. Create Input Layout for Vertex (Position, Normal, Color)
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    DX3DGraphicsLogThrowOnFail(
        m_device.CreateInputLayout(
            layout,
            ARRAYSIZE(layout),
            desc.vsBinary.getBufferPointer(),
            desc.vsBinary.getBufferSize(),
            &m_inputLayout
        ),
        "CreateInputLayout failed."
    );
}
