#pragma once
#include <DX3D/Graphics/GraphicsResource.h>
#include <wrl.h>

namespace dx3d
{
    struct VertexBufferDesc
    {
        GraphicsResourceDesc gResource;
        const void* dataList{ nullptr };
        u32 vertexSize{ 0 };
        u32 vertexCount{ 0 };
    };

    class VertexBuffer final : public GraphicsResource
    {
    public:
        explicit VertexBuffer(const VertexBufferDesc& desc);
        virtual ~VertexBuffer() override = default;

        ID3D11Buffer* get() const { return m_buffer.Get(); }
        u32 getVertexSize() const { return m_vertexSize; }
        u32 getVertexCount() const { return m_vertexCount; }

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer{};
        u32 m_vertexSize{ 0 };
        u32 m_vertexCount{ 0 };
    };
}
