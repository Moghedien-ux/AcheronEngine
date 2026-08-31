#include <DX3D/Graphics/IndexBuffer.h>
#include <DX3D/Graphics/GraphicsLogUtils.h>


dx3d::IndexBuffer::IndexBuffer(const IndexBufferDesc& desc, const GraphicsResourceDesc& gDesc) :
    GraphicsResource(gDesc), m_indexListSize(desc.indexListSize)
{
    if (!desc.indexList) DX3DLogThrowInvalidArgs("No index list provided.");
    if (!desc.indexListSize) DX3DLogThrowInvalidArgs("No index list size provided.");

    D3D11_BUFFER_DESC buffDesc{};
    buffDesc.ByteWidth = desc.indexListSize * sizeof(ui32);
    buffDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    buffDesc.Usage = D3D11_USAGE_DEFAULT;

    D3D11_SUBRESOURCE_DATA initData{};
    initData.pSysMem = desc.indexList;

    DX3DGraphicsLogThrowOnFail(m_device.CreateBuffer(&buffDesc, &initData, &m_buffer),
        "CreateBuffer (IndexBuffer) failed.");
}

dx3d::ui32 dx3d::IndexBuffer::getIndexListSize() const noexcept
{
    return m_indexListSize;
}
