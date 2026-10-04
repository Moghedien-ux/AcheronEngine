#include <DX3D/Graphics/ConstantBuffer.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/GraphicsLogUtils.h>

dx3d::ConstantBuffer::ConstantBuffer(const ConstantBufferDesc& desc) :
    GraphicsResource(desc.gResource),
    m_bufferSize(desc.bufferSize)
{
    D3D11_BUFFER_DESC bDesc{};
    bDesc.Usage = D3D11_USAGE_DEFAULT;
    bDesc.ByteWidth = (desc.bufferSize + 15) & ~15;
    bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bDesc.CPUAccessFlags = 0;

    DX3DGraphicsLogThrowOnFail(
        m_device.CreateBuffer(&bDesc, nullptr, &m_buffer),
        "CreateBuffer for ConstantBuffer failed."
    );
}

void dx3d::ConstantBuffer::update(DeviceContext& context, const void* data)
{
    context.getD3DContext()->UpdateSubresource(m_buffer.Get(), 0, nullptr, data, 0, 0);
}
