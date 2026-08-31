#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Graphics/GraphicsLogUtils.h>

dx3d::VertexBuffer::VertexBuffer(const VertexBufferDesc& desc) :
    GraphicsResource(desc.gResource),
    m_vertexSize(desc.vertexSize),
    m_vertexCount(desc.vertexCount)
{
    D3D11_BUFFER_DESC bDesc{};
    bDesc.Usage = D3D11_USAGE_DEFAULT;
    bDesc.ByteWidth = desc.vertexSize * desc.vertexCount;
    bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initData{};
    initData.pSysMem = desc.dataList;

    DX3DGraphicsLogThrowOnFail(
        m_device.CreateBuffer(&bDesc, &initData, &m_buffer),
        "CreateBuffer for VertexBuffer failed."
    );
}
