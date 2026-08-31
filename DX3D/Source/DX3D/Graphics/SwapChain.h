#pragma once
#include <DX3D/Graphics/GraphicsResource.h>
#include <DX3D/Core/Base.h>
#include <DX3D/Math/Rect.h>
#include <d3d11.h>

namespace dx3d
{
    class SwapChain final : public GraphicsResource
    {
    public:
        explicit SwapChain(const SwapChainDesc& desc, const GraphicsResourceDesc& gDesc);
        Rect getSize() const noexcept;
        bool present(bool vsync = false);

    private:
        void reloadBuffers();

    private:
        Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain{};
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv{};
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_dsv{};
        Rect m_size{};
        friend class DeviceContext;
    };
}

