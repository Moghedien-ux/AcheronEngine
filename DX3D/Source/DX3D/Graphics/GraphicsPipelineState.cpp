#include <DX3D/Graphics/GraphicsPipelineState.h>
#include <DX3D/Graphics/GraphicsLogUtils.h>

dx3d::GraphicsPipelineState::GraphicsPipelineState(const GraphicsPipelineStateDesc& desc) :
    GraphicsResource(desc.gResource)
{
    DX3DGraphicsLogThrowOnFail(
        m_device.CreateVertexShader(desc.vsBinary.getBufferPointer(), desc.vsBinary.getBufferSize(), nullptr, &m_vs),
        "CreateVertexShader failed."
    );

    DX3DGraphicsLogThrowOnFail(
        m_device.CreatePixelShader(desc.psBinary.getBufferPointer(), desc.psBinary.getBufferSize(), nullptr, &m_ps),
        "CreatePixelShader failed."
    );

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,       0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    DX3DGraphicsLogThrowOnFail(
        m_device.CreateInputLayout(
            layout,
            ARRAYSIZE(layout),
            desc.vsBinary.getBufferPointer(),
            desc.vsBinary.getBufferSize(),
            &m_inputLayout
        ),
        "CreateInputLayout for Mesh Vertex failed."
    );
}