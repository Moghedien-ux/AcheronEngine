#pragma once
#include <DX3D/Graphics/GraphicsResource.h>

namespace dx3d
{
    struct ConstantBufferDesc
    {
        const void* bufferData{};
        ui32 bufferSize{};
    };

    class ConstantBuffer final : public GraphicsResource
    {
    public:
        explicit ConstantBuffer(const ConstantBufferDesc& desc, const GraphicsResourceDesc& gDesc);
        void update(const void* bufferData, ui32 bufferSize);

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer{};
        ui32 m_size{};

        friend class DeviceContext;
    };
}



