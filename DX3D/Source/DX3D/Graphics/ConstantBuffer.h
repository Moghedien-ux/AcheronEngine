#pragma once
#include <DX3D/Graphics/GraphicsResource.h>
#include <wrl.h>

namespace dx3d
{
    class DeviceContext;

    struct ConstantBufferDesc
    {
        GraphicsResourceDesc gResource;
        u32 bufferSize{ 0 };
    };

    class ConstantBuffer final : public GraphicsResource
    {
    public:
        explicit ConstantBuffer(const ConstantBufferDesc& desc);
        virtual ~ConstantBuffer() override = default;

        ID3D11Buffer* get() const { return m_buffer.Get(); }
        void update(DeviceContext& context, const void* data);

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer{};
        u32 m_bufferSize{ 0 };
    };
}
