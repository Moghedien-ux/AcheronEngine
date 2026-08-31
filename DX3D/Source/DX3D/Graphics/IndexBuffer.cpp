#include <DX3D/Graphics/IndexBuffer.h>
#include <DX3D/Graphics/GraphicsLogUtils.h>

dx3d::IndexBuffer::IndexBuffer(const IndexBufferDesc& desc) :
    GraphicsResource(desc.gResource),
    m_indexCount(desc.indexCount)
{
    D3D11_BUFFER_DESC bDesc{};
    bDesc.Usage = D3D11_USAGE_DEFAULT;
    bDesc.ByteWidth = sizeof(u32) * desc.indexCount;
    bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initData{};
    initData.pSysMem = desc.dataList;

    DX3DGraphicsLogThrowOnFail(
        m_device.CreateBuffer(&bDesc, &initData, &m_buffer),
        "CreateBuffer for IndexBuffer failed."
    );
}
