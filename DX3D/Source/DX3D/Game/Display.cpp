#include <DX3D/Game/Display.h>
#include <DX3D/Graphics/GraphicsDevice.h>

dx3d::Display::Display(const DisplayDesc& desc) :
    Window(desc.window)
{
    m_swapChain = std::make_unique<SwapChain>(
        SwapChainDesc{
            { desc.window.base, desc.device.getD3DDevice() },
            m_hwnd,
            m_size
        }
    );
}
