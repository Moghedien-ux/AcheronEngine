#include <DX3D/Graphics/HighlightPass.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/ConstantBuffer.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Graphics/IndexBuffer.h>
#include <DX3D/Graphics/GraphicsLogUtils.h>
#include <DX3D/Core/Common.h>

dx3d::HighlightPass::HighlightPass(const GraphicsResourceDesc& gDesc) : GraphicsResource(gDesc)
{
    // 1. Wireframe Rasterizer State for selection outline
    D3D11_RASTERIZER_DESC rsDesc{};
    rsDesc.FillMode = D3D11_FILL_WIREFRAME;
    rsDesc.CullMode = D3D11_CULL_NONE;
    rsDesc.DepthClipEnable = TRUE;
    rsDesc.AntialiasedLineEnable = TRUE;

    DX3DGraphicsLogThrowOnFail(m_device.CreateRasterizerState(&rsDesc, &m_wireframeRS),
        "CreateRasterizerState for Wireframe failed.");

    // 2. Depth Stencil State
    D3D11_DEPTH_STENCIL_DESC dsDesc{};
    dsDesc.DepthEnable = TRUE;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    DX3DGraphicsLogThrowOnFail(m_device.CreateDepthStencilState(&dsDesc, &m_highlightDSS),
        "CreateDepthStencilState for Highlight failed.");
}

void dx3d::HighlightPass::renderHighlight(
    DeviceContext& context,
    const Camera& camera,
    const GameObject& selectedObject,
    ConstantBuffer& cb,
    const VertexBuffer& vb,
    const IndexBuffer& ib)
{
    // Scale slightly by 1.03x to create an outer highlight cage
    Mat4 s = Mat4::Scale(selectedObject.m_scale * 1.035f);
    Mat4 rx = Mat4::RotationX(selectedObject.m_rotation.x);
    Mat4 ry = Mat4::RotationY(selectedObject.m_rotation.y);
    Mat4 rz = Mat4::RotationZ(selectedObject.m_rotation.z);
    Mat4 t = Mat4::Translation(selectedObject.m_position);
    Mat4 world = s * rx * ry * rz * t;

    ConstantData cData{};
    cData.world = world;
    cData.view = camera.getViewMatrix();
    cData.proj = camera.getProjectionMatrix();
    cData.customColor = { 1.0f, 0.6f, 0.1f, 1.0f }; // Unreal Orange Outline
    cData.highlightParams = { 1.0f, 0.0f, 0.0f, 0.0f };

    cb.update(context, &cData);

    ID3D11DeviceContext* d3dContext = context.getD3DContext();
    d3dContext->RSSetState(m_wireframeRS.Get());
    d3dContext->OMSetDepthStencilState(m_highlightDSS.Get(), 0);

    context.setVertexBuffer(vb);
    context.setIndexBuffer(ib);
    context.setConstantBuffer(cb, 0);

    context.drawIndexed(ib.getIndexCount());

    // Reset rasterizer state to default solid
    d3dContext->RSSetState(nullptr);
    d3dContext->OMSetDepthStencilState(nullptr, 0);
}
