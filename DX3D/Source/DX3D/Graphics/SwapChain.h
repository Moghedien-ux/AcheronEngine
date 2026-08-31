#pragma once
#include <DX3D/Graphics/GraphicsResource.h>
#include <dxgi.h>
#include <wrl.h>

namespace dx3d
{
    class DeviceContext;

    struct SwapChainDesc
    {
        GraphicsResourceDesc gResource;
        HWND hwnd{ nullptr };
        Rect size{ 1280, 720 };
    };

    class SwapChain final : public GraphicsResource
    {
    public:
        explicit SwapChain(const SwapChainDesc& desc);
        virtual ~SwapChain() override = default;

        void clear(DeviceContext& context, f32 r, f32 g, f32 b, f32 a);
        void present(bool vsync = true);

        ID3D11RenderTargetView* getRenderTargetView() const { return m_rtv.Get(); }
        ID3D11DepthStencilView* getDepthStencilView() const { return m_dsv.Get(); }
        Rect getSize() const { return m_size; }

    private:
        void createBuffers(const Rect& size);

    private:
        Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain{};
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv{};
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_dsv{};
        Rect m_size{ 1280, 720 };
    };
}
