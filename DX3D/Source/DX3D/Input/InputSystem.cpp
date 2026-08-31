#include <DX3D/Input/InputSystem.h>
#include <cstring>

dx3d::InputSystem::InputSystem(const BaseDesc& desc) : Base(desc)
{
    std::memset(m_keys, 0, sizeof(m_keys));
    std::memset(m_prevKeys, 0, sizeof(m_prevKeys));
    std::memset(m_mouseButtons, 0, sizeof(m_mouseButtons));
}

void dx3d::InputSystem::update()
{
    std::memcpy(m_prevKeys, m_keys, sizeof(m_keys));
    m_mouseDelta = { m_currentMousePos.x - m_prevMousePos.x, m_currentMousePos.y - m_prevMousePos.y };
    m_prevMousePos = m_currentMousePos;
}

bool dx3d::InputSystem::isKeyDown(int keyCode) const
{
    if (keyCode >= 0 && keyCode < 256)
        return m_keys[keyCode];
    return false;
}

bool dx3d::InputSystem::isKeyUp(int keyCode) const
{
    if (keyCode >= 0 && keyCode < 256)
        return !m_keys[keyCode];
    return true;
}

bool dx3d::InputSystem::isMouseButtonDown(int button) const
{
    if (button >= 0 && button < 3)
        return m_mouseButtons[button];
    return false;
}

dx3d::Point dx3d::InputSystem::getMousePosition() const
{
    return m_currentMousePos;
}

dx3d::Point dx3d::InputSystem::getMouseDelta() const
{
    return m_mouseDelta;
}

void dx3d::InputSystem::onKeyDown(int keyCode)
{
    if (keyCode >= 0 && keyCode < 256)
        m_keys[keyCode] = true;
}

void dx3d::InputSystem::onKeyUp(int keyCode)
{
    if (keyCode >= 0 && keyCode < 256)
        m_keys[keyCode] = false;
}

void dx3d::InputSystem::onMouseMove(i32 x, i32 y)
{
    if (m_firstMouseMove)
    {
        m_prevMousePos = { x, y };
        m_firstMouseMove = false;
    }
    m_currentMousePos = { x, y };
}

void dx3d::InputSystem::onMouseDown(int button)
{
    if (button >= 0 && button < 3)
        m_mouseButtons[button] = true;
}

void dx3d::InputSystem::onMouseUp(int button)
{
    if (button >= 0 && button < 3)
        m_mouseButtons[button] = false;
}