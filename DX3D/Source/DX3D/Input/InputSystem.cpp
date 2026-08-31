#include <DX3D/Input/InputSystem.h>
#include <cstring>

namespace
{
    dx3d::InputSystem* g_globalInput = nullptr;
}

void dx3d::SetGlobalInputSystem(InputSystem* input)
{
    g_globalInput = input;
}

dx3d::InputSystem* dx3d::GetGlobalInputSystem()
{
    return g_globalInput;
}

dx3d::InputSystem::InputSystem(const BaseDesc& desc) : Base(desc)
{
    std::memset(m_keysStates, 0, sizeof(m_keysStates));
    std::memset(m_oldKeysStates, 0, sizeof(m_oldKeysStates));
}

void dx3d::InputSystem::update()
{
    std::memcpy(m_oldKeysStates, m_keysStates, sizeof(m_keysStates));
    GetKeyboardState(m_keysStates);

    POINT p;
    GetCursorPos(&p);

    if (m_firstMouseMove)
    {
        m_oldMousePos = { static_cast<i32>(p.x), static_cast<i32>(p.y) };
        m_firstMouseMove = false;
    }

    m_currentMousePos = { static_cast<i32>(p.x), static_cast<i32>(p.y) };
    m_mouseDelta = { m_currentMousePos.x - m_oldMousePos.x, m_currentMousePos.y - m_oldMousePos.y };
    m_oldMousePos = m_currentMousePos;
}

void dx3d::InputSystem::setCursorPosition(const Point& point)
{
    SetCursorPos(point.x, point.y);
    m_oldMousePos = point;
}

bool dx3d::InputSystem::isKeyDown(i32 keyCode) const
{
    if (keyCode < 0 || keyCode >= 256) return false;
    return (m_keysStates[keyCode] & 0x80) != 0;
}

bool dx3d::InputSystem::isKeyUp(i32 keyCode) const
{
    if (keyCode < 0 || keyCode >= 256) return false;
    return (m_keysStates[keyCode] & 0x80) == 0;
}

bool dx3d::InputSystem::isMouseButtonDown(i32 button) const
{
    switch (button)
    {
        case 0: return (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
        case 1: return (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
        case 2: return (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;
        default: return false;
    }
}
