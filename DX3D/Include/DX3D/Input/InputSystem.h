#pragma once
#include <DX3D/Core/Base.h>
#include <DX3D/Core/Core.h>
#include <unordered_map>
#include <Windows.h>

namespace dx3d
{
    struct Point
    {
        i32 x{ 0 }, y{ 0 };
    };

    class InputSystem final : public Base
    {
    public:
        explicit InputSystem(const BaseDesc& desc);
        ~InputSystem() = default;

        void update();

        bool isKeyDown(int keyCode) const;
        bool isKeyUp(int keyCode) const;

        bool isMouseButtonDown(int button) const; // 0 = Left, 1 = Right, 2 = Middle
        Point getMousePosition() const;
        Point getMouseDelta() const;

        // Called by Window message loop
        void onKeyDown(int keyCode);
        void onKeyUp(int keyCode);
        void onMouseMove(i32 x, i32 y);
        void onMouseDown(int button);
        void onMouseUp(int button);

    private:
        bool m_keys[256]{ false };
        bool m_prevKeys[256]{ false };
        bool m_mouseButtons[3]{ false };
        Point m_currentMousePos{};
        Point m_prevMousePos{};
        Point m_mouseDelta{};
        bool m_firstMouseMove{ true };
    };
}