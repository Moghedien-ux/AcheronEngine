#pragma once
#include <DX3D/Core/Base.h>
#include <DX3D/Core/Core.h>
#include <Windows.h>

namespace dx3d
{
    class InputSystem final : public Base
    {
    public:
        explicit InputSystem(const BaseDesc& desc);
        virtual ~InputSystem() override = default;

        void update();
        void setCursorPosition(const Point& point);

        bool isKeyDown(i32 keyCode) const;
        bool isKeyUp(i32 keyCode) const;

        bool isMouseButtonDown(i32 button) const; // 0: Left, 1: Right, 2: Middle
        Point getMousePosition() const { return m_currentMousePos; }
        Point getMouseDelta() const { return m_mouseDelta; }

    private:
        u8 m_keysStates[256]{ 0 };
        u8 m_oldKeysStates[256]{ 0 };

        Point m_currentMousePos{ 0, 0 };
        Point m_oldMousePos{ 0, 0 };
        Point m_mouseDelta{ 0, 0 };
        bool m_firstMouseMove{ true };
    };

    void SetGlobalInputSystem(InputSystem* input);
    InputSystem* GetGlobalInputSystem();
}
