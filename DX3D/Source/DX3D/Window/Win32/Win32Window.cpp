#include <DX3D/Window/Window.h>
#include <DX3D/UI/ImGuiLayer.h>

LRESULT CALLBACK dx3d::Window::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// Let ImGui handle input first
	if (ImGuiLayer::handleWin32Message(hwnd, msg, wparam, lparam))
		return true;

	switch (msg)
	{
		case WM_CLOSE:
			DestroyWindow(hwnd);
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

dx3d::Window::Window(const WindowDesc& desc) :
	Base(desc.base),
	m_size(desc.size)
{
	HINSTANCE hInstance = GetModuleHandle(nullptr);

	WNDCLASSEX wc{};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = L"DX3DWindowClass";

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, desc.size.width, desc.size.height };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	m_hwnd = CreateWindowEx(
		0,
		L"DX3DWindowClass",
		desc.title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, hInstance, nullptr
	);

	if (!m_hwnd)
	{
		throw DX3DException("Failed to create Win32 window.");
	}

	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);
}

dx3d::Window::~Window()
{
	if (m_hwnd)
	{
		DestroyWindow(m_hwnd);
		m_hwnd = nullptr;
	}
}
