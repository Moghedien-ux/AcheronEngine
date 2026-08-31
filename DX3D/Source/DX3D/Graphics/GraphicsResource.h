#pragma once
#include <DX3D/Core/Base.h>
#include <d3d11.h>

namespace dx3d
{
    struct GraphicsResourceDesc
    {
        BaseDesc base;
        ID3D11Device& device;
    };

    class GraphicsResource : public Base
    {
    public:
        explicit GraphicsResource(const GraphicsResourceDesc& desc) :
            Base(desc.base),
            m_device(desc.device)
        {}

        virtual ~GraphicsResource() override = default;

    protected:
        ID3D11Device& m_device;
    };
}
