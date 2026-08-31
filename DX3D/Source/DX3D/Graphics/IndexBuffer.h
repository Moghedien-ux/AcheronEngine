#pragma once
#include <DX3D/Graphics/GraphicsResource.h>
#include <wrl.h>

namespace dx3d
{
    struct IndexBufferDesc
    {
        GraphicsResourceDesc gResource;
        const void* dataList{ nullptr };
        u32 indexCount{ 0 };
    };

    class IndexBuffer final : public GraphicsResource
    {
    public:
        explicit IndexBuffer(const IndexBufferDesc& desc);
        virtual ~IndexBuffer() override = default;

        ID3D11Buffer* get() const { return m_buffer.Get(); }
        u32 getIndexCount() const { return m_indexCount; }

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer{};
        u32 m_indexCount{ 0 };
    };
}
