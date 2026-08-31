#include <DX3D/UI/ImGuiLayer.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool dx3d::ImGuiLayer::handleWin32Message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
        return true;

    if (ImGui::GetCurrentContext() == nullptr)
        return false;

    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse && (msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MOUSEWHEEL))
        return true;

    if (io.WantCaptureKeyboard && (msg == WM_KEYDOWN || msg == WM_KEYUP || msg == WM_CHAR))
        return true;

    return false;
}

dx3d::ImGuiLayer::ImGuiLayer(const BaseDesc& desc, HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context) :
    Base(desc), m_hwnd(hwnd)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    applyUnrealEngineTheme();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(device, context);

    DX3DLogInfo("ImGuiLayer Initialized with Unreal Engine 5 Theme.");
}

dx3d::ImGuiLayer::~ImGuiLayer()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void dx3d::ImGuiLayer::beginFrame()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void dx3d::ImGuiLayer::endFrame()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void dx3d::ImGuiLayer::applyUnrealEngineTheme()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // Unreal Engine 5 Slate Palette
    colors[ImGuiCol_Text]                  = ImVec4(0.92f, 0.93f, 0.94f, 1.00f);
    colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.52f, 0.54f, 1.00f);
    colors[ImGuiCol_WindowBg]              = ImVec4(0.12f, 0.13f, 0.15f, 0.94f);
    colors[ImGuiCol_ChildBg]               = ImVec4(0.10f, 0.11f, 0.12f, 1.00f);
    colors[ImGuiCol_PopupBg]               = ImVec4(0.14f, 0.15f, 0.17f, 0.96f);
    colors[ImGuiCol_Border]                = ImVec4(0.24f, 0.26f, 0.28f, 0.60f);
    colors[ImGuiCol_FrameBg]               = ImVec4(0.18f, 0.19f, 0.22f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.24f, 0.26f, 0.30f, 1.00f);
    colors[ImGuiCol_FrameBgActive]         = ImVec4(0.28f, 0.30f, 0.35f, 1.00f);
    colors[ImGuiCol_TitleBg]               = ImVec4(0.09f, 0.10f, 0.11f, 1.00f);
    colors[ImGuiCol_TitleBgActive]         = ImVec4(0.15f, 0.16f, 0.18f, 1.00f);
    colors[ImGuiCol_Button]                = ImVec4(0.20f, 0.22f, 0.26f, 1.00f);
    colors[ImGuiCol_ButtonHovered]         = ImVec4(0.28f, 0.45f, 0.70f, 1.00f); // Unreal Blue Accent
    colors[ImGuiCol_ButtonActive]          = ImVec4(0.18f, 0.38f, 0.62f, 1.00f);
    colors[ImGuiCol_Header]                = ImVec4(0.22f, 0.24f, 0.28f, 1.00f);
    colors[ImGuiCol_HeaderHovered]         = ImVec4(0.28f, 0.32f, 0.38f, 1.00f);
    colors[ImGuiCol_HeaderActive]          = ImVec4(0.35f, 0.40f, 0.48f, 1.00f);
    colors[ImGuiCol_SliderGrab]            = ImVec4(0.85f, 0.50f, 0.15f, 1.00f); // Unreal Orange Accent
    colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.95f, 0.60f, 0.20f, 1.00f);
    colors[ImGuiCol_CheckMark]             = ImVec4(0.85f, 0.50f, 0.15f, 1.00f);

    style.WindowRounding    = 6.0f;
    style.FrameRounding     = 4.0f;
    style.GrabRounding      = 4.0f;
    style.PopupRounding     = 4.0f;
    style.FramePadding      = ImVec2(6, 4);
    style.ItemSpacing       = ImVec2(8, 6);
}
