#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Graphics/IndexBuffer.h>
#include <DX3D/Graphics/ConstantBuffer.h>
#include <DX3D/Graphics/GraphicsPipelineState.h>

dx3d::DeviceContext::DeviceContext(const DeviceContextDesc& desc) :
	GraphicsResource(desc.gResource),
	m_context(desc.context)
{
}

void dx3d::DeviceContext::setViewport(const Rect& size)
{
	D3D11_VIEWPORT vp{};
	vp.Width = static_cast<f32>(size.width);
	vp.Height = static_cast<f32>(size.height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	m_context->RSSetViewports(1, &vp);
}

void dx3d::DeviceContext::setPipelineState(const GraphicsPipelineState& state)
{
	m_context->IASetInputLayout(state.getInputLayout());
	m_context->VSSetShader(state.getVertexShader(), nullptr, 0);
	m_context->PSSetShader(state.getPixelShader(), nullptr, 0);
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void dx3d::DeviceContext::setVertexBuffer(const VertexBuffer& vb)
{
	UINT stride = vb.getVertexSize();
	UINT offset = 0;
	ID3D11Buffer* buf = vb.get();
	m_context->IASetVertexBuffers(0, 1, &buf, &stride, &offset);
}

void dx3d::DeviceContext::setIndexBuffer(const IndexBuffer& ib)
{
	m_context->IASetIndexBuffer(ib.get(), DXGI_FORMAT_R32_UINT, 0);
}

void dx3d::DeviceContext::setConstantBuffer(const ConstantBuffer& cb, u32 slot)
{
	ID3D11Buffer* buf = cb.get();
	m_context->VSSetConstantBuffers(slot, 1, &buf);
	m_context->PSSetConstantBuffers(slot, 1, &buf);
}

void dx3d::DeviceContext::drawIndexed(u32 indexCount, u32 startIndexLocation, i32 baseVertexLocation)
{
	m_context->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}
