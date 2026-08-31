#include <DX3D/Graphics/ConstantBuffer.h>
#include <DX3D/Graphics/GraphicsLogUtils.h>

dx3d::ConstantBuffer::ConstantBuffer(const ConstantBufferDesc& desc, const GraphicsResourceDesc& gDesc) :
    GraphicsResource(gDesc), m_size(desc.bufferSize)
{
    if (!desc.bufferSize)
        DX3DLogThrowInvalidArgs("Constant buffer size cannot be zero.");

    ui32 alignedSize = (desc.bufferSize + 15) & ~15;
    m_size = alignedSize;

    D3D11_BUFFER_DESC buffDesc{};
    buffDesc.ByteWidth = alignedSize;
    buffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    buffDesc.Usage = D3D11_USAGE_DEFAULT;
    buffDesc.CPUAccessFlags = 0;

    if (desc.bufferData)
    {
        D3D11_SUBRESOURCE_DATA initData{};
        initData.pSysMem = desc.bufferData;
        DX3DGraphicsLogThrowOnFail(m_device.CreateBuffer(&buffDesc, &initData, &m_buffer),
            "CreateBuffer (ConstantBuffer with data) failed.");
    }
    else
    {
        DX3DGraphicsLogThrowOnFail(m_device.CreateBuffer(&buffDesc, nullptr, &m_buffer),
            "CreateBuffer (ConstantBuffer empty) failed.");
    }
}

void dx3d::ConstantBuffer::update(const void* bufferData, ui32 bufferSize)
{
    if (!bufferData) return;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> immContext;
    m_device.GetImmediateContext(&immContext);
    immContext->UpdateSubresource(m_buffer.Get(), 0, nullptr, bufferData, 0, 0);
}
