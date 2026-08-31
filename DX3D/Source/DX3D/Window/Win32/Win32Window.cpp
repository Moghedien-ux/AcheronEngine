#include "DX3D/Window/Window.h"
#include <DX3D/Input/InputSystem.h>
#include <DX3D/Core/Core.h>
#include <Windows.h>
#include <windowsx.h>

static dx3d::InputSystem* g_inputSystem = nullptr;

namespace dx3d
{
	void SetGlobalInputSystem(InputSystem* input)
	{
		g_inputSystem = input;
	}
}

static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	case WM_KEYDOWN:
	{
		if (g_inputSystem) g_inputSystem->onKeyDown(static_cast<int>(wparam));
		return 0;
	}

	case WM_KEYUP:
	{
		if (g_inputSystem) g_inputSystem->onKeyUp(static_cast<int>(wparam));
		return 0;
	}

	case WM_MOUSEMOVE:
	{
		if (g_inputSystem)
		{
			dx3d::i32 x = GET_X_LPARAM(lparam);
			dx3d::i32 y = GET_Y_LPARAM(lparam);
			g_inputSystem->onMouseMove(x, y);
		}
		return 0;
	}

	case WM_LBUTTONDOWN:
	{
		if (g_inputSystem) g_inputSystem->onMouseDown(0);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		if (g_inputSystem) g_inputSystem->onMouseUp(0);
		return 0;
	}

	case WM_RBUTTONDOWN:
	{
		if (g_inputSystem) g_inputSystem->onMouseDown(1);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		if (g_inputSystem) g_inputSystem->onMouseUp(1);
		return 0;
	}

	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
}

dx3d::Window::Window(const WindowDesc& desc) : Base(desc.base), m_size(desc.size)
{
	auto registerWindowClassFunction = []()
		{
			WNDCLASSEX wc{};
			wc.cbSize = sizeof(WNDCLASSEX);
			wc.lpszClassName = "DX3DWindow";
			wc.lpfnWndProc = &WindowProcedure;
			return RegisterClassEx(&wc);
		};

	static const auto windowClassId = std::invoke(registerWindowClassFunction);

	if (!windowClassId)
		DX3DLogThrowError("RegisterClassEx failed.");

	RECT rc{ 0, 0, m_size.width, m_size.height };
	AdjustWindowRect(&rc, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, false);

	m_handle = CreateWindowEx(NULL, MAKEINTATOM(windowClassId), "DX3D Game Engine - 3D Cube & Camera",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT,
		rc.right - rc.left, rc.bottom - rc.top,
		NULL, NULL, NULL, NULL);

	if (!m_handle)
		DX3DLogThrowError("CreateWindowEx failed.");

	ShowWindow(static_cast<HWND>(m_handle), SW_SHOW);
}

dx3d::Window::~Window()
{
	DestroyWindow(static_cast<HWND>(m_handle));
}