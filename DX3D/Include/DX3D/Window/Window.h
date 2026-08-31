#pragma once
#include <DX3D/Core/Base.h>
#include <DX3D/Core/Core.h>
#include <Windows.h>

namespace dx3d
{
    struct WindowDesc
    {
        BaseDesc base;
        Rect size{ 1280, 720 };
        const wchar_t* title{ L"DX3D Game Engine (DirectX 11)" };
    };

    class Window : public Base
    {
    public:
        explicit Window(const WindowDesc& desc);
        virtual ~Window() override;

        HWND getHandle() const { return m_hwnd; }
        Rect getSize() const { return m_size; }
        bool isAlive() const { return m_hwnd != nullptr; }

    private:
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    protected:
        HWND m_hwnd{ nullptr };
        Rect m_size{ 1280, 720 };
    };
}
