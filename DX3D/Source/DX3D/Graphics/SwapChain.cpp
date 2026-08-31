#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/GraphicsLogUtils.h>

dx3d::SwapChain::SwapChain(const SwapChainDesc& desc) :
    GraphicsResource(desc.gResource),
    m_size(desc.size)
{
    Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
    DX3DGraphicsLogThrowOnFail(m_device.QueryInterface(__uuidof(IDXGIDevice), &dxgiDevice), "Query IDXGIDevice failed");

    Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
    DX3DGraphicsLogThrowOnFail(dxgiDevice->GetAdapter(&adapter), "GetAdapter failed");

    Microsoft::WRL::ComPtr<IDXGIFactory> factory;
    DX3DGraphicsLogThrowOnFail(adapter->GetParent(__uuidof(IDXGIFactory), &factory), "GetParent factory failed");

    DXGI_SWAP_CHAIN_DESC scDesc{};
    scDesc.BufferCount = 2;
    scDesc.BufferDesc.Width = desc.size.width;
    scDesc.BufferDesc.Height = desc.size.height;
    scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scDesc.BufferDesc.RefreshRate.Numerator = 60;
    scDesc.BufferDesc.RefreshRate.Denominator = 1;
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.OutputWindow = desc.hwnd;
    scDesc.SampleDesc.Count = 1;
    scDesc.SampleDesc.Quality = 0;
    scDesc.Windowed = TRUE;
    scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    DX3DGraphicsLogThrowOnFail(factory->CreateSwapChain(&m_device, &scDesc, &m_swapChain), "CreateSwapChain failed");

    createBuffers(desc.size);
}

void dx3d::SwapChain::createBuffers(const Rect& size)
{
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    DX3DGraphicsLogThrowOnFail(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer), "Get back buffer failed");
    DX3DGraphicsLogThrowOnFail(m_device.CreateRenderTargetView(backBuffer.Get(), nullptr, &m_rtv), "Create RTV failed");

    D3D11_TEXTURE2D_DESC dsDesc{};
    dsDesc.Width = size.width;
    dsDesc.Height = size.height;
    dsDesc.MipLevels = 1;
    dsDesc.ArraySize = 1;
    dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsDesc.SampleDesc.Count = 1;
    dsDesc.SampleDesc.Quality = 0;
    dsDesc.Usage = D3D11_USAGE_DEFAULT;
    dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthBuffer;
    DX3DGraphicsLogThrowOnFail(m_device.CreateTexture2D(&dsDesc, nullptr, &depthBuffer), "Create depth buffer failed");
    DX3DGraphicsLogThrowOnFail(m_device.CreateDepthStencilView(depthBuffer.Get(), nullptr, &m_dsv), "Create DSV failed");
}

void dx3d::SwapChain::clear(DeviceContext& context, f32 r, f32 g, f32 b, f32 a)
{
    f32 color[4] = { r, g, b, a };
    context.getD3DContext()->ClearRenderTargetView(m_rtv.Get(), color);
    context.getD3DContext()->ClearDepthStencilView(m_dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    ID3D11RenderTargetView* rtv = m_rtv.Get();
    context.getD3DContext()->OMSetRenderTargets(1, &rtv, m_dsv.Get());
}

void dx3d::SwapChain::present(bool vsync)
{
    m_swapChain->Present(vsync ? 1 : 0, 0);
}
