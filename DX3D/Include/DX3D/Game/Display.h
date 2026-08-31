#pragma once
#include <DX3D/Window/Window.h>
#include <DX3D/Graphics/SwapChain.h>

namespace dx3d
{
    class GraphicsDevice;

    struct DisplayDesc
    {
        WindowDesc window;
        GraphicsDevice& device;
    };

    class Display final : public Window
    {
    public:
        explicit Display(const DisplayDesc& desc);
        virtual ~Display() override = default;

        SwapChain& getSwapChain() { return *m_swapChain; }

    private:
        std::unique_ptr<SwapChain> m_swapChain{};
    };
}
