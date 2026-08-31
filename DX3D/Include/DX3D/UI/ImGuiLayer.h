#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Core/Base.h>
#include <d3d11.h>
#include <Windows.h>

namespace dx3d
{
    class ImGuiLayer final : public Base
    {
    public:
        ImGuiLayer(const BaseDesc& desc, HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context);
        ~ImGuiLayer();

        void beginFrame();
        void endFrame();

        // Forward Windows messages to ImGui
        static bool handleWin32Message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    private:
        void applyUnrealEngineTheme();

    private:
        HWND m_hwnd{};
    };
}
