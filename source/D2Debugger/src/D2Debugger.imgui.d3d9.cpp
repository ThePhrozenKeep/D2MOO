// This is basically a modified copy of the Dear ImGui standalone example application for DirectX 9
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#include "IconsFontAwesome6.h"
#include "fa-solid-900.cpp" // NOLINT

#include "D2Debugger.h"

// Data
struct D2DebuggerData
{
    LPDIRECT3D9              pD3D = nullptr;
    LPDIRECT3DDEVICE9        pd3dDevice = nullptr;
    D3DPRESENT_PARAMETERS    d3dpp = {};
    HWND                     hWindow = nullptr;
    WNDCLASSEXW              windowClassEx;
    bool                     bShowDemo = true;
} gD2DebuggerData;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND GetGameWindow()
{
    HWND hGameWindow = nullptr;
    EnumWindows([](HWND hWnd, LPARAM lParam) -> BOOL
        {
            DWORD dwProcessId = 0x0;
            GetWindowThreadProcessId(hWnd, &dwProcessId);
            // Only consider the current process windows.
            if (GetCurrentProcessId() == dwProcessId)
            {
                char buf[2048];
                // There are many windows that may be created due to compatibility layers...
                // So sadly we need to filter by name too.
                // An alternative would be to get the window handle directly from D2Client.dll
                if (GetWindowTextA(hWnd, buf, _countof(buf)) && strstr(buf, "Diablo"))
                {
                    *(HWND*)lParam = hWnd;
                    SetLastError(ERROR_SUCCESS);
                    // Stop iteration here.
                    return FALSE;
                }
            }
            return TRUE;
        }
    , (LPARAM)&hGameWindow);
    return hGameWindow;
}

void GetWindowPositionFromGameWindow(int& x, int& y)
{
    const HWND hGameWindow = GetGameWindow();
    if (!hGameWindow)
    {
        x = CW_USEDEFAULT;
        return;
    }
    MONITORINFO monitorInfo = { 0 };
    monitorInfo.cbSize = sizeof(MONITORINFO);
    GetMonitorInfoA(MonitorFromWindow(hGameWindow, MONITOR_DEFAULTTOPRIMARY), &monitorInfo);

    RECT windowRect;
    GetWindowRect(hGameWindow, &windowRect);

    if (windowRect.left == monitorInfo.rcMonitor.left
        && windowRect.right == monitorInfo.rcMonitor.right
        && windowRect.top == monitorInfo.rcMonitor.top
        && windowRect.bottom == monitorInfo.rcMonitor.bottom)
    {
        // Fullscreen, same position as game window
        x = windowRect.left;
        y = windowRect.top;
    }
    else
    {
        // Windowed, on the right of the game window
        x = windowRect.right + 5;
        y = windowRect.top;
    }
}

bool IsCursorVisible()
{
    CURSORINFO ci = { sizeof(CURSORINFO) };
    if (GetCursorInfo(&ci))
        return ci.flags & CURSOR_SHOWING;
    return false;
}

D2DEBUGGER_DLL_DECL
int D2DebuggerInit()
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    gD2DebuggerData.windowClassEx = { sizeof(WNDCLASSEXW), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"D2Debugger", nullptr };
    ::RegisterClassExW(&gD2DebuggerData.windowClassEx);
    int x = 0, y = 0;
    GetWindowPositionFromGameWindow(x, y);
    gD2DebuggerData.hWindow = ::CreateWindowW(gD2DebuggerData.windowClassEx.lpszClassName, L"D2Debugger", WS_OVERLAPPEDWINDOW, x, y, 1000, 800, nullptr, nullptr, gD2DebuggerData.windowClassEx.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(gD2DebuggerData.hWindow))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(gD2DebuggerData.windowClassEx.lpszClassName, gD2DebuggerData.windowClassEx.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(gD2DebuggerData.hWindow, SW_SHOWDEFAULT);
    ::UpdateWindow(gD2DebuggerData.hWindow);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(gD2DebuggerData.hWindow);
    ImGui_ImplDX9_Init(gD2DebuggerData.pd3dDevice);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    float baseFontSize = 13.0f; // 13.0f is the size of the default font. Change to the font size you use.
    float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

    // merge in icons from Font Awesome
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.GlyphMinAdvanceX = iconFontSize;
    ImFont* fontAwesome = io.Fonts->AddFontFromMemoryCompressedTTF(
        faSolid900_compressed_compressed_data, faSolid900_compressed_compressed_size, 
        iconFontSize, &icons_config, icons_ranges
    );
    IM_ASSERT(fontAwesome != nullptr);
    return 0;
}

D2DEBUGGER_DLL_DECL
void D2DebuggerDestroy()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(gD2DebuggerData.hWindow);
    ::UnregisterClassW(gD2DebuggerData.windowClassEx.lpszClassName, gD2DebuggerData.windowClassEx.hInstance);
}

// returns true if should quit
D2DEBUGGER_DLL_DECL
bool D2DebuggerNewFrame()
{
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    // Poll and handle messages (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    while (::PeekMessageW(&msg, nullptr, 0U, 0U, PM_REMOVE))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessageW(&msg);

        if (msg.message == WM_QUIT)
        {
            return true;
        }
    }

    // Some rendering backends seem to force hide all cursors of the application.
    // So detect this case and ask ImGui to render it instead.
    if (!IsCursorVisible())
    {
        auto& io = ImGui::GetIO();
        io.MouseDrawCursor = true;
    }
    // Start the Dear ImGui frame
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (gD2DebuggerData.bShowDemo)
        ImGui::ShowDemoWindow(&gD2DebuggerData.bShowDemo);

    return false;
}

D2DEBUGGER_DLL_DECL
void D2DebuggerEndFrame(bool VSyncNextFrame)
{
    // Rendering
    ImGui::EndFrame();
    gD2DebuggerData.pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    gD2DebuggerData.pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    gD2DebuggerData.pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
    
    const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * 255.0f), (int)(clear_color.y * 255.0f), (int)(clear_color.z * 255.0f), (int)(clear_color.w * 255.0f));
    gD2DebuggerData.pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
    if (gD2DebuggerData.pd3dDevice->BeginScene() >= 0)
    {
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        gD2DebuggerData.pd3dDevice->EndScene();
    }
    HRESULT result = gD2DebuggerData.pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);

    // Handle loss of D3D9 device
    const UINT NextFramePresentationInterval = VSyncNextFrame ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
    if ((NextFramePresentationInterval != gD2DebuggerData.d3dpp.PresentationInterval) ||
        (result == D3DERR_DEVICELOST && gD2DebuggerData.pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET))
    {
        gD2DebuggerData.d3dpp.PresentationInterval = NextFramePresentationInterval;
        ResetDevice();
    }
}



// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    gD2DebuggerData.pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (gD2DebuggerData.pD3D == nullptr)
        return false;

    // Create the D3DDevice
    ZeroMemory(&gD2DebuggerData.d3dpp, sizeof(gD2DebuggerData.d3dpp));
    gD2DebuggerData.d3dpp.Windowed = TRUE;
    gD2DebuggerData.d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    gD2DebuggerData.d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    gD2DebuggerData.d3dpp.EnableAutoDepthStencil = TRUE;
    gD2DebuggerData.d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    //gD2DebuggerData.d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    gD2DebuggerData.d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (gD2DebuggerData.pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &gD2DebuggerData.d3dpp, &gD2DebuggerData.pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (gD2DebuggerData.pd3dDevice) { gD2DebuggerData.pd3dDevice->Release(); gD2DebuggerData.pd3dDevice = nullptr; }
    if (gD2DebuggerData.pD3D) { gD2DebuggerData.pD3D->Release(); gD2DebuggerData.pD3D = nullptr; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = gD2DebuggerData.pd3dDevice->Reset(&gD2DebuggerData.d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (gD2DebuggerData.pd3dDevice != nullptr && wParam != SIZE_MINIMIZED)
        {
            gD2DebuggerData.d3dpp.BackBufferWidth = LOWORD(lParam);
            gD2DebuggerData.d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
