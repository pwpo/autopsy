#include <dwmapi.h>
#include <cstdio>
#include <chrono>
#include <thread>
#include <d3d11.h>
#include <wincodec.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <cmath>
#include <cfloat>
#include <cstring>

#include "ui/graphic.h"
#include "global.h"

#include <imgui/misc/imgui_freetype.h>
#include "imgui/imgui_internal.h"

#include "ui/font/regular.h"
#include "ui/font/bold.h"
#include "feature/esp.h"
#include "feature/ball.h"
#include <ImGui/addons/Colors/Colors.h>
#include "feature/silent.h"
#include "config.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND Hwnd, UINT Msg, WPARAM WParam, LPARAM LParam);

LRESULT CALLBACK wndproc(HWND Hwnd, UINT Msg, WPARAM WParam, LPARAM LParam)
{
    if (ImGui_ImplWin32_WndProcHandler(Hwnd, Msg, WParam, LParam))
        return true;

    switch (Msg)
    {
    case WM_SYSCOMMAND:
        if ((WParam & 0xfff0) == SC_KEYMENU) return 0;
        break;
    case WM_SYSKEYDOWN:
        if (WParam == VK_F4) { DestroyWindow(Hwnd); return 0; }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CLOSE:
        return 0;
    }
    return DefWindowProcA(Hwnd, Msg, WParam, LParam);
}


graphic::graphic() { Detail = std::make_unique<detail>(); }
graphic::~graphic() { dropimgui(); dropwindow(); dropdevice(); }

bool graphic::window()
{
    Detail->WindowClass.cbSize = sizeof(Detail->WindowClass);
    Detail->WindowClass.style = CS_CLASSDC;
    Detail->WindowClass.lpszClassName = "autopsy.lol";
    Detail->WindowClass.hInstance = GetModuleHandleA(0);
    Detail->WindowClass.lpfnWndProc = wndproc;
    RegisterClassExA(&Detail->WindowClass);

    Detail->Window = CreateWindowExA(
        WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW,
        Detail->WindowClass.lpszClassName, "autopsy.lol", WS_POPUP,
        0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        0, 0, Detail->WindowClass.hInstance, 0);

    if (!Detail->Window) return false;

    SetLayeredWindowAttributes(Detail->Window, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);

    RECT ClientArea{}, WindowArea{};
    GetClientRect(Detail->Window, &ClientArea);
    GetWindowRect(Detail->Window, &WindowArea);
    POINT Diff{};
    ClientToScreen(Detail->Window, &Diff);

    MARGINS Margins{
        WindowArea.left + (Diff.x - WindowArea.left),
        WindowArea.top + (Diff.y - WindowArea.top),
        WindowArea.right, WindowArea.bottom
    };
    DwmExtendFrameIntoClientArea(Detail->Window, &Margins);
    ShowWindow(Detail->Window, SW_SHOW);
    UpdateWindow(Detail->Window);
    return true;
}

bool graphic::device()
{
    DXGI_SWAP_CHAIN_DESC sd{};
    sd.BufferCount = 2;
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.OutputWindow = Detail->Window;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Windowed = 1;
    sd.Flags = 0;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    D3D_FEATURE_LEVEL fl;
    const D3D_FEATURE_LEVEL fll[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
    HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        fll, 2, D3D11_SDK_VERSION, &sd, &Detail->SwapChain, &Detail->Device, &fl, &Detail->DeviceContext);

    if (hr == DXGI_ERROR_UNSUPPORTED)
        hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, 0,
            fll, 2, D3D11_SDK_VERSION, &sd, &Detail->SwapChain, &Detail->Device, &fl, &Detail->DeviceContext);

    if (FAILED(hr) || !Detail->SwapChain || !Detail->Device || !Detail->DeviceContext)
    {
        MessageBoxA(nullptr, "This software can not run on your computer.", "Critical Problem", MB_ICONERROR | MB_OK);
        return false;
    }

    ID3D11Texture2D* bb = nullptr;
    if (FAILED(Detail->SwapChain->GetBuffer(0, IID_PPV_ARGS(&bb))) || !bb)
        return false;

    if (bb) {
        HRESULT rtv = Detail->Device->CreateRenderTargetView(bb, nullptr, &Detail->GraphicsTargetView);
        bb->Release();
        return SUCCEEDED(rtv) && Detail->GraphicsTargetView;
    }
    return false;
}

bool graphic::imgui()
{
    ImGui::CreateContext();

    float dpiScale = ImGui_ImplWin32_GetDpiScaleForMonitor(
        ::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

    ImGuiIO& IO = ImGui::GetIO();
    IO.IniFilename = nullptr;

    const unsigned int ftFlags =
        ImGuiFreeTypeLoaderFlags_LoadColor |
        ImGuiFreeTypeLoaderFlags_LightHinting;
    IO.Fonts->SetFontLoader(ImGuiFreeType::GetFontLoader());
    IO.Fonts->FontLoaderFlags = ftFlags;

    ImFontConfig cfg{};
    cfg.PixelSnapH = false;
    cfg.OversampleH = 4;
    cfg.OversampleV = 4;
    cfg.RasterizerMultiply = 1.03f;
    cfg.FontLoaderFlags = 0;
    cfg.FontDataOwnedByAtlas = false;

    const float sz = 14.0f * dpiScale;
    const char* fontPath = "C:\\Windows\\Fonts\\segoeui.ttf";
    const char* fontSemi = "C:\\Windows\\Fonts\\seguisb.ttf";
    const char* fontBold = "C:\\Windows\\Fonts\\segoeuib.ttf";
    const char* fontBlack = "C:\\Windows\\Fonts\\seguibl.ttf";
    if (GetFileAttributesA(fontPath) != INVALID_FILE_ATTRIBUTES)
    {
        UiFont = IO.Fonts->AddFontFromFileTTF(fontPath, sz, &cfg);
        Tahoma_BoldXP = IO.Fonts->AddFontFromFileTTF(
            GetFileAttributesA(fontSemi) != INVALID_FILE_ATTRIBUTES ? fontSemi : fontPath,
            15.0f * dpiScale, &cfg);
        TitleFont = IO.Fonts->AddFontFromFileTTF(
            GetFileAttributesA(fontSemi) != INVALID_FILE_ATTRIBUTES ? fontSemi : fontPath,
            16.0f * dpiScale, &cfg);
    }
    else
    {
        UiFont = IO.Fonts->AddFontFromMemoryTTF(
            font_regular, (int)font_regular_len, sz, &cfg);
        Tahoma_BoldXP = IO.Fonts->AddFontFromMemoryTTF(
            font_bold, (int)font_bold_len, 15.0f * dpiScale, &cfg);
        TitleFont = IO.Fonts->AddFontFromMemoryTTF(
            font_bold, (int)font_bold_len, 16.0f * dpiScale, &cfg);
    }

    ImFontConfig logoCfg = cfg;
    logoCfg.RasterizerMultiply = 1.05f;
    const char* logoPath = GetFileAttributesA(fontBlack) != INVALID_FILE_ATTRIBUTES ? fontBlack : fontBold;
    const char* logoFallback = GetFileAttributesA(fontSemi) != INVALID_FILE_ATTRIBUTES ? fontSemi : fontPath;
    if (GetFileAttributesA(logoPath) != INVALID_FILE_ATTRIBUTES)
        LogoFont = IO.Fonts->AddFontFromFileTTF(logoPath, 21.0f * dpiScale, &logoCfg);
    else if (GetFileAttributesA(logoFallback) != INVALID_FILE_ATTRIBUTES)
        LogoFont = IO.Fonts->AddFontFromFileTTF(logoFallback, 20.0f * dpiScale, &logoCfg);
    else
        LogoFont = Tahoma_BoldXP;

    ImGui::StyleColorsDark();
    ImGuiStyle& S = ImGui::GetStyle();
    S.WindowRounding = 14.f;
    S.ChildRounding = 11.f;
    S.FrameRounding = 7.f;
    S.PopupRounding = 10.f;
    S.ScrollbarRounding = 8.f;
    S.GrabRounding = 6.f;
    S.ItemSpacing = { 8.f, 7.f };
    S.FramePadding = { 8.f, 5.f };
    S.WindowPadding = { 0.f, 0.f };
    S.PopupBorderSize = 1.f;
    S.FrameBorderSize = 0.f;
    S.ScrollbarSize = 4.f;

    if (!ImGui_ImplWin32_Init(Detail->Window))                return false;
    if (!Detail->Device || !Detail->DeviceContext)            return false;
    if (!ImGui_ImplDX11_Init(Detail->Device, Detail->DeviceContext)) return false;
    return true;
}

void graphic::dropdevice()
{
    if (Detail->GraphicsTargetView) Detail->GraphicsTargetView->Release();
    if (Detail->SwapChain)          Detail->SwapChain->Release();
    if (Detail->DeviceContext)      Detail->DeviceContext->Release();
    if (Detail->Device)             Detail->Device->Release();
    Detail->GraphicsTargetView = nullptr;
    Detail->SwapChain = nullptr;
    Detail->DeviceContext = nullptr;
    Detail->Device = nullptr;
}
void graphic::dropwindow()
{
    if (Detail->Window)
    {
        DestroyWindow(Detail->Window);
        Detail->Window = nullptr;
    }
    if (Detail->WindowClass.lpszClassName)
        UnregisterClassA(Detail->WindowClass.lpszClassName, Detail->WindowClass.hInstance);
}
void graphic::dropimgui()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

static int menukey(ImGuiKey key)
{
    if (key >= ImGuiKey_0 && key <= ImGuiKey_9)
        return '0' + (key - ImGuiKey_0);
    if (key >= ImGuiKey_A && key <= ImGuiKey_Z)
        return 'A' + (key - ImGuiKey_A);
    if (key >= ImGuiKey_F1 && key <= ImGuiKey_F24)
        return VK_F1 + (key - ImGuiKey_F1);
    if (key >= ImGuiKey_Keypad0 && key <= ImGuiKey_Keypad9)
        return VK_NUMPAD0 + (key - ImGuiKey_Keypad0);

    switch (key)
    {
    case ImGuiKey_Tab: return VK_TAB;
    case ImGuiKey_LeftArrow: return VK_LEFT;
    case ImGuiKey_RightArrow: return VK_RIGHT;
    case ImGuiKey_UpArrow: return VK_UP;
    case ImGuiKey_DownArrow: return VK_DOWN;
    case ImGuiKey_PageUp: return VK_PRIOR;
    case ImGuiKey_PageDown: return VK_NEXT;
    case ImGuiKey_Home: return VK_HOME;
    case ImGuiKey_End: return VK_END;
    case ImGuiKey_Insert: return VK_INSERT;
    case ImGuiKey_Delete: return VK_DELETE;
    case ImGuiKey_Backspace: return VK_BACK;
    case ImGuiKey_Space: return VK_SPACE;
    case ImGuiKey_Enter: return VK_RETURN;
    case ImGuiKey_Escape: return VK_ESCAPE;
    case ImGuiKey_LeftCtrl: return VK_LCONTROL;
    case ImGuiKey_LeftShift: return VK_LSHIFT;
    case ImGuiKey_LeftAlt: return VK_LMENU;
    case ImGuiKey_RightCtrl: return VK_RCONTROL;
    case ImGuiKey_RightShift: return VK_RSHIFT;
    case ImGuiKey_RightAlt: return VK_RMENU;
    case ImGuiKey_Menu: return VK_APPS;
    case ImGuiKey_Apostrophe: return VK_OEM_7;
    case ImGuiKey_Comma: return VK_OEM_COMMA;
    case ImGuiKey_Minus: return VK_OEM_MINUS;
    case ImGuiKey_Period: return VK_OEM_PERIOD;
    case ImGuiKey_Slash: return VK_OEM_2;
    case ImGuiKey_Semicolon: return VK_OEM_1;
    case ImGuiKey_Equal: return VK_OEM_PLUS;
    case ImGuiKey_LeftBracket: return VK_OEM_4;
    case ImGuiKey_Backslash: return VK_OEM_5;
    case ImGuiKey_RightBracket: return VK_OEM_6;
    case ImGuiKey_GraveAccent: return VK_OEM_3;
    case ImGuiKey_CapsLock: return VK_CAPITAL;
    case ImGuiKey_ScrollLock: return VK_SCROLL;
    case ImGuiKey_NumLock: return VK_NUMLOCK;
    case ImGuiKey_PrintScreen: return VK_SNAPSHOT;
    case ImGuiKey_Pause: return VK_PAUSE;
    case ImGuiKey_KeypadDecimal: return VK_DECIMAL;
    case ImGuiKey_KeypadDivide: return VK_DIVIDE;
    case ImGuiKey_KeypadMultiply: return VK_MULTIPLY;
    case ImGuiKey_KeypadSubtract: return VK_SUBTRACT;
    case ImGuiKey_KeypadAdd: return VK_ADD;
    case ImGuiKey_KeypadEnter: return VK_RETURN;
    case ImGuiKey_MouseLeft: return VK_LBUTTON;
    case ImGuiKey_MouseRight: return VK_RBUTTON;
    case ImGuiKey_MouseMiddle: return VK_MBUTTON;
    case ImGuiKey_MouseX1: return VK_XBUTTON1;
    case ImGuiKey_MouseX2: return VK_XBUTTON2;
    default: return 0;
    }
}

void graphic::begin()
{
    MSG Msg;
    while (PeekMessage(&Msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (Msg.message == WM_QUIT)
            ExitProcess(0);
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    static bool lastStreamproof = !global::setting::Streamproof;
    if (lastStreamproof != global::setting::Streamproof)
    {
        SetWindowDisplayAffinity(Detail->Window,
            global::setting::Streamproof ? WDA_EXCLUDEFROMCAPTURE : WDA_NONE);
        lastStreamproof = global::setting::Streamproof;
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    {
        HWND _roblox = FindWindowA(0, "Roblox");
        HWND _fg = GetForegroundWindow();
        static double lastToggle = -1.0;
        const double now = ImGui::GetTime();
        const int menuVk = menukey(global::setting::Menu_Key);
        if (menuVk != 0 && (GetAsyncKeyState(menuVk) & 1) && (_fg == _roblox || _fg == Detail->Window) && now - lastToggle >= .18)
        {
            lastToggle = now;
            Running = !Running;
            LONG exStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_LAYERED;
            if (!Running) exStyle |= WS_EX_TRANSPARENT;
            SetWindowLong(Detail->Window, GWL_EXSTYLE, exStyle);
        }
    }
}

void graphic::end()
{
    if (!Detail->DeviceContext || !Detail->GraphicsTargetView || !Detail->SwapChain)
        return;

    ImGui::Render();
    const float cc[4]{ 0.f, 0.f, 0.f, 0.f };
    Detail->DeviceContext->OMSetRenderTargets(1, &Detail->GraphicsTargetView, nullptr);
    Detail->DeviceContext->ClearRenderTargetView(Detail->GraphicsTargetView, cc);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    const int mode = ImClamp(global::setting::Performance_Mode, 0, 2);
    if (mode == 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(2));

    const UINT syncInterval = mode == 2 ? 0u : 1u;
    const HRESULT hr = Detail->SwapChain->Present(syncInterval, 0);
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        ExitProcess(0);
}

static void cursor()
{
    if (!SilentAimInstance.Address) return;
    if (!drive->read<bool>(SilentAimInstance.Address + offset::gui::Visible)) return;
    POINT pt;
    if (!GetCursorPos(&pt)) return;

    const bool rmbHeld = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
    const float gap = rmbHeld ? 4.f : 10.f;
    ImDrawList* dl = ImGui::GetBackgroundDrawList();
    const ImU32 col = IM_COL32(255, 255, 255, 255);
    const float dot = 4.f, lw = 2.f, ll = 10.f;
    const ImVec2 c = { (float)pt.x, (float)pt.y };

    dl->AddRectFilled(c - ImVec2(dot * .5f, dot * .5f), c + ImVec2(dot * .5f, dot * .5f), col);
    dl->AddRectFilled({ c.x - lw * .5f, c.y - gap - ll }, { c.x + lw * .5f, c.y - gap }, col);
    dl->AddRectFilled({ c.x - lw * .5f, c.y + gap }, { c.x + lw * .5f, c.y + gap + ll }, col);
    dl->AddRectFilled({ c.x - gap - ll, c.y - lw * .5f }, { c.x - gap,    c.y + lw * .5f }, col);
    dl->AddRectFilled({ c.x + gap,    c.y - lw * .5f }, { c.x + gap + ll, c.y + lw * .5f }, col);
}

static const char* pcuser()
{
    static std::string name = []()
        {
            char buffer[257]{};
            DWORD len = GetEnvironmentVariableA("USERNAME", buffer, (DWORD)sizeof(buffer));
            if (!len || len >= sizeof(buffer))
                return std::string("Windows");
            return std::string(buffer, buffer + len);
        }();
    return name.c_str();
}

static bool copytext(const std::string& text)
{
    if (text.empty() || !OpenClipboard(nullptr))
        return false;

    EmptyClipboard();

    HGLOBAL memory = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
    if (!memory)
    {
        CloseClipboard();
        return false;
    }

    void* data = GlobalLock(memory);
    if (!data)
    {
        GlobalFree(memory);
        CloseClipboard();
        return false;
    }

    std::memcpy(data, text.c_str(), text.size() + 1);
    GlobalUnlock(memory);

    if (!SetClipboardData(CF_TEXT, memory))
    {
        GlobalFree(memory);
        CloseClipboard();
        return false;
    }

    CloseClipboard();
    return true;
}




namespace color
{
    static ImU32 Win = IM_COL32(3, 6, 10, 255);
    static ImU32 Surface = IM_COL32(6, 11, 18, 255);
    static ImU32 SurfaceLift = IM_COL32(8, 18, 29, 255);
    static ImU32 card = IM_COL32(8, 14, 22, 255);
    static ImU32 CardHov = IM_COL32(11, 28, 42, 255);
    static ImU32 Border = IM_COL32(24, 48, 65, 255);
    static ImU32 BorderDim = IM_COL32(12, 26, 38, 255);
    static ImU32 Divider = IM_COL32(17, 34, 48, 255);
    static ImU32 Accent = IM_COL32(0, 174, 255, 255);
    static ImU32 AccentDim = IM_COL32(2, 57, 86, 255);
    static ImU32 AccentHov = IM_COL32(56, 203, 255, 255);
    static ImU32 AccentFg = IM_COL32(210, 247, 255, 255);
    static ImU32 Purple = IM_COL32(100, 117, 255, 255);
    static ImU32 PurpleDim = IM_COL32(18, 29, 67, 255);
    static ImU32 Glow = IM_COL32(0, 174, 255, 72);
    static ImU32 GlowPurple = IM_COL32(100, 117, 255, 50);
    static ImU32 text = IM_COL32(232, 242, 249, 255);
    static ImU32 TextMid = IM_COL32(146, 166, 178, 255);
    static ImU32 TextDim = IM_COL32(78, 99, 112, 255);
    static ImU32 White = IM_COL32(255, 255, 255, 255);
    static ImU32 Danger = IM_COL32(255, 80, 104, 255);
    static ImU32 DangerDim = IM_COL32(63, 18, 28, 255);
    static ImU32 DangerBrd = IM_COL32(135, 35, 55, 255);
    static ImU32 Track = IM_COL32(18, 31, 42, 255);
    static ImU32 Transp = IM_COL32(0, 0, 0, 0);

    static ImU32 lerp(ImU32 a, ImU32 b, float t);
    static ImU32 alpha(ImU32 c, float a);

    static ImU32 fromfloat(const float col[4], float alphaMul = 1.f)
    {
        return ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], col[3] * alphaMul));
    }

    static void refresh()
    {
        Win = fromfloat(global::setting::color::Window);
        Surface = lerp(Win, IM_COL32(255, 255, 255, 255), .025f);
        SurfaceLift = lerp(Win, fromfloat(global::setting::color::Accent), .09f);
        card = fromfloat(global::setting::color::card);
        CardHov = lerp(card, fromfloat(global::setting::color::Accent), .18f);
        Accent = fromfloat(global::setting::color::Accent);
        Purple = fromfloat(global::setting::color::Accent2);
        AccentDim = alpha(Accent, .34f);
        AccentHov = lerp(Accent, White, .22f);
        AccentFg = lerp(Accent, White, .78f);
        PurpleDim = alpha(Purple, .34f);
        Glow = alpha(Accent, .28f);
        GlowPurple = alpha(Purple, .22f);
        text = fromfloat(global::setting::color::text);
        TextMid = lerp(text, Win, .38f);
        TextDim = lerp(text, Win, .62f);
        Border = lerp(card, Accent, .28f);
        BorderDim = lerp(Win, Accent, .14f);
        Divider = lerp(Win, Accent, .18f);
        Track = lerp(Win, text, .09f);
    }

    static ImU32 lerp(ImU32 a, ImU32 b, float t)
    {
        auto ch = [](ImU32 c, int s) { return (int)((c >> s) & 0xFF); };
        auto mix = [&](int s) { return (int)(ch(a, s) * (1.f - t) + ch(b, s) * t); };
        return IM_COL32(mix(0), mix(8), mix(16), mix(24));
    }

    static ImU32 alpha(ImU32 c, float a)
    {
        const int alpha = (int)(((c >> 24) & 0xFF) * ImClamp(a, 0.f, 1.f));
        return (c & 0x00FFFFFF) | ((ImU32)alpha << 24);
    }
}

namespace fx
{
    static std::unordered_map<ImGuiID, float> s_t;

    static float saturate(float v)
    {
        return ImClamp(v, 0.f, 1.f);
    }

    static float easeoutcubic(float t)
    {
        t = 1.f - saturate(t);
        return 1.f - t * t * t;
    }

    static float easeoutquint(float t)
    {
        t = 1.f - saturate(t);
        return 1.f - t * t * t * t * t;
    }

    static float easeinoutcubic(float t)
    {
        t = saturate(t);
        return t < .5f ? 4.f * t * t * t : 1.f - powf(-2.f * t + 2.f, 3.f) * .5f;
    }

    static float damp(float value, float target, float speed)
    {
        const float dt = ImClamp(ImGui::GetIO().DeltaTime, 0.f, 1.f / 30.f);
        const float k = 1.f - expf(-speed * dt);
        const float v = ImLerp(value, target, k);
        return fabsf(v - target) < .0007f ? target : v;
    }

    static float anim(ImGuiID id, bool on, float speed = 12.f)
    {
        float& v = s_t[id];
        v = damp(v, on ? 1.f : 0.f, speed);
        return v;
    }

    static float togglet(ImGuiID id, bool on)
    {
        return easeinoutcubic(anim(id, on, 9.5f));
    }
}

namespace icon
{
    static void crosshair(ImDrawList* dl, ImVec2 c, float sz, ImU32 col)
    {
        const float r = sz * .38f, gap = sz * .13f, ll = sz * .26f, lw = 1.5f;
        dl->AddCircle(c, r, col, 32, lw);
        dl->AddCircleFilled(c, 2.0f, col);
        dl->AddLine({ c.x,       c.y - r - gap }, { c.x,       c.y - r - gap - ll }, col, lw);
        dl->AddLine({ c.x,       c.y + r + gap }, { c.x,       c.y + r + gap + ll }, col, lw);
        dl->AddLine({ c.x - r - gap, c.y }, { c.x - r - gap - ll, c.y }, col, lw);
        dl->AddLine({ c.x + r + gap, c.y }, { c.x + r + gap + ll, c.y }, col, lw);
    }

    static void eye(ImDrawList* dl, ImVec2 c, float sz, ImU32 col)
    {
        const float rx = sz * .44f, ry = sz * .22f, lw = 1.5f;
        const ImVec2 L = { c.x - rx,c.y }, R = { c.x + rx,c.y };
        const ImVec2 T = { c.x,c.y - ry }, B = { c.x,c.y + ry };
        dl->AddBezierCubic(L, { L.x + rx * .65f,T.y - ry * .3f }, { R.x - rx * .65f,T.y - ry * .3f }, R, col, lw);
        dl->AddBezierCubic(L, { L.x + rx * .65f,B.y + ry * .3f }, { R.x - rx * .65f,B.y + ry * .3f }, R, col, lw);
        dl->AddCircleFilled(c, sz * .13f, col);
    }

    static void globe(ImDrawList* dl, ImVec2 c, float sz, ImU32 col)
    {
        const float r = sz * .40f, cx = sz * .22f, lw = 1.5f;
        dl->AddCircle(c, r, col, 32, lw);
        dl->AddLine({ c.x - r, c.y }, { c.x + r, c.y }, col, lw);
        dl->AddBezierCubic({ c.x,c.y - r }, { c.x + cx,c.y - r * .5f }, { c.x + cx,c.y + r * .5f }, { c.x,c.y + r }, col, lw);
        dl->AddBezierCubic({ c.x,c.y - r }, { c.x - cx,c.y - r * .5f }, { c.x - cx,c.y + r * .5f }, { c.x,c.y + r }, col, lw);
    }

    static void layer(ImDrawList* dl, ImVec2 c, float sz, ImU32 col)
    {
        const float hw = sz * .35f, hh = sz * .07f, gap = sz * .17f;
        for (int i = -1; i <= 1; i++)
            dl->AddRectFilled({ c.x - hw, c.y + i * gap - hh }, { c.x + hw, c.y + i * gap + hh }, col, 1.f);
    }

    static void gear(ImDrawList* dl, ImVec2 c, float sz, ImU32 col)
    {
        const float ri = sz * .28f, ro = sz * .42f;
        dl->AddCircle(c, ri, col, 24, 1.5f);
        for (int i = 0; i < 6; i++)
        {
            const float a = (float)i / 6.f * 6.2832f;
            const float a1 = a - .24f, a2 = a + .24f;
            dl->AddQuadFilled(
                { c.x + cosf(a1) * ri, c.y + sinf(a1) * ri },
                { c.x + cosf(a2) * ri, c.y + sinf(a2) * ri },
                { c.x + cosf(a2) * ro, c.y + sinf(a2) * ro },
                { c.x + cosf(a1) * ro, c.y + sinf(a1) * ro }, col);
        }
    }

    static void diamond(ImDrawList* dl, ImVec2 c, float sz, ImU32 col)
    {
        const float r = sz * .42f;
        dl->AddQuad({ c.x, c.y - r }, { c.x + r, c.y }, { c.x, c.y + r }, { c.x - r, c.y }, col, 1.6f);
        dl->AddLine({ c.x - r * .45f, c.y }, { c.x + r * .45f, c.y }, col, 1.3f);
        dl->AddCircleFilled(c, sz * .08f, col, 12);
    }
}

namespace side
{
    static constexpr float kW = 178.f;
    static constexpr float kCompactW = 58.f;
    static constexpr float kTabH = 42.f;
    static constexpr float kLogoH = 84.f;

    using IconFn = void(*)(ImDrawList*, ImVec2, float, ImU32);

    static bool tab(ImDrawList* dl, ImVec2 wp, float startY, float railW, bool compact,
        int idx, int cur, IconFn icon, const char* label)
    {
        const bool   active = (idx == cur);
        const float tabH = kTabH - 6.f;

        const float tabPad = compact ? 4.f : 14.f;
        const float tabW = railW - (tabPad * 2.f);
        const float tabX = wp.x + tabPad;
        const float tabY = wp.y + startY + idx * kTabH;

        const ImVec2 tMin = { tabX, tabY };
        const ImVec2 tMax = tMin + ImVec2(tabW, tabH);
        const ImVec2 center = compact
            ? ImVec2(tMin.x + tabW * .5f, tMin.y + tabH * .5f)
            : ImVec2(tMin.x + 18.f, tMin.y + tabH * .5f);

        ImGui::SetCursorScreenPos(tMin);
        ImGui::PushID(idx);
        ImGui::InvisibleButton("##tab", { tabW, tabH });

        const bool clicked = ImGui::IsItemClicked();
        const bool hov = ImGui::IsItemHovered();
        const ImGuiID id = ImGui::GetItemID();
        ImGui::PopID();

        const float t = fx::easeoutcubic(fx::anim(id, active || hov, 9.f));
        const float at = fx::easeoutcubic(fx::anim(id ^ 0x8A13C4u, active, 10.f));
        const ImU32 hoverFill = color::lerp(color::Transp, IM_COL32(11, 28, 42, 126), t);
        const ImU32 activeFill = IM_COL32(0, 78, 116, 178);
        dl->AddRectFilled(tMin, tMax, color::lerp(hoverFill, activeFill, at), 9.f);

        if (at > .01f)
        {
            dl->AddRectFilled(tMin + ImVec2(1.f, 1.f), tMax - ImVec2(1.f, 1.f),
                IM_COL32(0, 174, 255, (int)(at * 12.f)), 8.f);
            dl->AddRect(tMin, tMax, IM_COL32(0, 174, 255, (int)(at * 96.f)), 9.f, 0, 1.f);
        }

        const ImU32 baseCol = hov ? color::text : color::TextMid;
        ImU32 iconCol = color::lerp(baseCol, color::AccentFg, at);
        icon(dl, center + ImVec2(at * 1.5f, 0.f), 18.f + at, iconCol);
        if (!compact)
            dl->AddText({ tMin.x + 42.f, tMin.y + (tabH - ImGui::GetFontSize()) * .5f }, iconCol, label);

        return clicked;
    }
}

namespace ui
{
    static constexpr float kTW = 34.f;
    static constexpr float kTH = 18.f;
    static constexpr float kTrkH = 4.f;
    static constexpr float kColW = 15.f;
    static constexpr float kColH = 14.f;

    static void labelsection(const char* text)
    {
        ImDrawList* dl = ImGui::GetWindowDrawList();
        const ImVec2 p = ImGui::GetCursorScreenPos();

        dl->AddRectFilled(p, { p.x + 3.f, p.y + ImGui::GetFontSize() + 1.f }, color::Accent, 1.5f);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10.f);
        ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::TextMid), "%s", text);

        ImGui::PushStyleColor(ImGuiCol_Separator, ImGui::ColorConvertU32ToFloat4(color::Divider));
        ImGui::Separator();
        ImGui::PopStyleColor();
        ImGui::Dummy({ 0.f, 4.f });
    }

    static void gap(float px = 4.f) { ImGui::Dummy({ 0.f, px }); }

    static bool toggle(const char* label, bool* v)
    {
        ImGui::PushID(label);
        const ImVec2   p = ImGui::GetCursorScreenPos();
        ImDrawList* dl = ImGui::GetWindowDrawList();

        ImGui::InvisibleButton("##t", { kTW, kTH });
        const bool clicked = ImGui::IsItemClicked();
        const bool hov = ImGui::IsItemHovered();
        if (clicked) *v = !*v;

        const float t = fx::togglet(ImGui::GetItemID(), *v);
        const ImU32 trk = color::lerp(color::Track, color::AccentDim, t);

        dl->AddRectFilled(p, p + ImVec2(kTW, kTH), trk, kTH * .5f);
        dl->AddRectFilledMultiColor(p + ImVec2(1.f, 1.f), p + ImVec2(kTW - 1.f, kTH - 1.f),
            color::lerp(IM_COL32(255, 255, 255, 0), color::Glow, t),
            color::lerp(IM_COL32(255, 255, 255, 0), color::Glow, t),
            color::lerp(IM_COL32(255, 255, 255, 0), color::GlowPurple, t),
            color::lerp(IM_COL32(255, 255, 255, 0), color::GlowPurple, t));
        if (t > .05f)
            dl->AddRect(p, p + ImVec2(kTW, kTH),
                IM_COL32(0, 174, 255, (int)(t * 135.f)), kTH * .5f, 0, 1.f);
        const float tx = p.x + 2.f + t * (kTW - kTH);
        const float tr = (kTH - 4.f) * .5f;
        const float cy = p.y + kTH * .5f;
        dl->AddCircleFilled({ tx + tr, cy }, tr,
            hov ? color::White : IM_COL32(224, 224, 232, 255), 18);

        ImGui::SameLine(0.f, 8.f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (kTH - ImGui::GetFontSize()) * .5f);
        ImGui::TextColored(
            ImGui::ColorConvertU32ToFloat4(*v ? color::text : color::TextMid),
            "%s", label);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - (kTH - ImGui::GetFontSize()) * .5f);

        ImGui::PopID();
        return clicked;
    }

    static bool togglecolor(const char* tLabel, bool* v,
        const char* cLabel, float col[4])
    {
        const bool changed = toggle(tLabel, v);
        ImGui::SameLine(ImGui::GetContentRegionMax().x - kColW);
        ImGui::PushID(cLabel);
        {
            const ImVec2 p = ImGui::GetCursorScreenPos();
            ImDrawList* dl = ImGui::GetWindowDrawList();
            const ImU32  sw = IM_COL32((int)(col[0] * 255), (int)(col[1] * 255),
                (int)(col[2] * 255), (int)(col[3] * 255));
            dl->AddRectFilled(p, p + ImVec2(kColW, kColH), sw, 2.f);
            dl->AddRect(p, p + ImVec2(kColW, kColH), *v ? color::Accent : color::Border, 2.f);
            ImGui::InvisibleButton("##s", { kColW, kColH });
            if (ImGui::IsItemClicked()) ImGui::OpenPopup("##cp");
            ImGui::SetNextWindowPos({ p.x + kColW + 4.f, p.y - 4.f });
            ImGui::PushStyleColor(ImGuiCol_PopupBg, ImGui::ColorConvertU32ToFloat4(color::card));
            ImGui::PushStyleColor(ImGuiCol_Border, ImGui::ColorConvertU32ToFloat4(color::Border));
            if (ImGui::BeginPopup("##cp")) {
                ImGui::ColorPicker4("##pk", col,
                    ImGuiColorEditFlags_NoSidePreview |
                    ImGuiColorEditFlags_AlphaBar |
                    ImGuiColorEditFlags_PickerHueBar);
                ImGui::EndPopup();
            }
            ImGui::PopStyleColor(2);
        }
        ImGui::PopID();
        return changed;
    }

    static void color4(const char* label, float col[4])
    {
        ImGui::PushID(label);
        const ImVec2 p = ImGui::GetCursorScreenPos();
        ImDrawList* dl = ImGui::GetWindowDrawList();
        const ImU32  sw = IM_COL32((int)(col[0] * 255), (int)(col[1] * 255),
            (int)(col[2] * 255), (int)(col[3] * 255));
        dl->AddRectFilled(p, p + ImVec2(kColW, kColH), sw, 2.f);
        dl->AddRect(p, p + ImVec2(kColW, kColH), color::Border, 2.f);
        ImGui::InvisibleButton("##sw", { kColW, kColH });
        if (ImGui::IsItemClicked()) ImGui::OpenPopup("##cpe");
        ImGui::SetNextWindowPos({ p.x + kColW + 4.f, p.y - 4.f });
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImGui::ColorConvertU32ToFloat4(color::card));
        ImGui::PushStyleColor(ImGuiCol_Border, ImGui::ColorConvertU32ToFloat4(color::Border));
        if (ImGui::BeginPopup("##cpe")) {
            ImGui::ColorPicker4("##pk", col,
                ImGuiColorEditFlags_NoSidePreview |
                ImGuiColorEditFlags_AlphaBar |
                ImGuiColorEditFlags_PickerHueBar);
            ImGui::EndPopup();
        }
        ImGui::PopStyleColor(2);
        ImGui::PopID();
    }

    static void dualcolor(const char* la, float* ca, const char* lb, float* cb)
    {
        ImGui::SameLine(ImGui::GetContentRegionMax().x - kColW * 2.f - 4.f);
        color4(la, ca);
        ImGui::SameLine(0.f, 4.f);
        color4(lb, cb);
    }

    static void tricolor(const char* la, float* ca, const char* lb, float* cb, const char* lc, float* cc)
    {
        ImGui::SameLine(ImGui::GetContentRegionMax().x - kColW * 3.f - 8.f);
        color4(la, ca); ImGui::SameLine(0.f, 4.f);
        color4(lb, cb); ImGui::SameLine(0.f, 4.f);
        color4(lc, cc);
    }

    static bool sliderfloat(const char* label, float* v, float mn, float mx)
    {
        ImGui::PushID(label);
        ImDrawList* dl = ImGui::GetWindowDrawList();
        const float avail = ImGui::GetContentRegionAvail().x;

        ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::TextMid), "%s", label);
        char buf[32]; snprintf(buf, sizeof(buf), "%.2f", *v);
        ImGui::SameLine(avail - ImGui::CalcTextSize(buf).x);
        ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::AccentFg), "%s", buf);

        const ImVec2 p = ImGui::GetCursorScreenPos();
        const float  w = ImGui::GetContentRegionAvail().x;
        const float  cy = p.y + 11.f;

        ImGui::InvisibleButton("##sl", { w, 22.f });
        const bool active = ImGui::IsItemActive();
        const bool hov = ImGui::IsItemHovered();
        if (active)
        {
            float rel = (ImGui::GetIO().MousePos.x - p.x) / w;
            *v = mn + ImClamp(rel, 0.f, 1.f) * (mx - mn);
        }
        const float t = mx == mn ? 0.f : ImClamp((*v - mn) / (mx - mn), 0.f, 1.f);

        dl->AddRectFilled({ p.x, cy - kTrkH * .5f }, { p.x + w, cy + kTrkH * .5f }, color::Track, kTrkH);
        dl->AddRectFilled({ p.x, cy - kTrkH * .5f }, { p.x + w * t, cy + kTrkH * .5f },
            active ? color::AccentHov : color::Accent, kTrkH);
        const float hh = active ? 7.5f : (hov ? 6.5f : 5.5f);
        const float hx = p.x + w * t;
        dl->AddCircleFilled({ hx, cy }, hh, active ? color::AccentHov : color::Accent, 22);

        ImGui::PopID();
        return ImGui::IsItemDeactivatedAfterEdit();
    }

    static bool sliderint(const char* label, int* v, int mn, int mx)
    {
        float fv = (float)*v;
        bool  r = sliderfloat(label, &fv, (float)mn, (float)mx);
        *v = (int)roundf(fv);
        return r;
    }

    static bool combo(const char* label, int* idx, const std::vector<const char*>& items)
    {
        ImGui::PushID(label);
        ImDrawList* dl = ImGui::GetWindowDrawList();
        const float avail = ImGui::GetContentRegionAvail().x;

        ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::TextMid), "%s", label);

        const ImVec2 p = ImGui::GetCursorScreenPos();
        const float  w = avail, h = 23.f;

        ImGui::InvisibleButton("##cb", { w, h });
        const bool hov = ImGui::IsItemHovered();
        const bool clicked = ImGui::IsItemClicked();

        dl->AddRectFilled(p, p + ImVec2(w, h), hov ? color::CardHov : color::SurfaceLift, 6.f);
        dl->AddRect(p, p + ImVec2(w, h), hov ? color::AccentDim : color::Border, 6.f, 0, 1.f);

        const char* cur = (*idx >= 0 && *idx < (int)items.size()) ? items[*idx] : "---";
        dl->AddText({ p.x + 7.f, p.y + (h - ImGui::GetFontSize()) * .5f }, color::text, cur);

        const float cx = p.x + w - 13.f, cy = p.y + h * .5f;
        dl->AddTriangleFilled({ cx - 4.f,cy - 2.f }, { cx + 4.f,cy - 2.f }, { cx,cy + 3.f }, color::TextMid);

        bool changed = false;
        if (clicked) ImGui::OpenPopup("##cop");
        ImGui::SetNextWindowPos({ p.x, p.y + h + 2.f });
        ImGui::SetNextWindowSize({ w, 0.f });
        ImGui::SetNextWindowSizeConstraints({ w,0.f }, { w,160.f });
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImGui::ColorConvertU32ToFloat4(color::card));
        ImGui::PushStyleColor(ImGuiCol_Border, ImGui::ColorConvertU32ToFloat4(color::Border));
        if (ImGui::BeginPopup("##cop"))
        {
            for (int i = 0; i < (int)items.size(); i++)
            {
                const bool sel = (i == *idx);
                ImGui::PushStyleColor(ImGuiCol_Header,
                    ImGui::ColorConvertU32ToFloat4(sel ? color::AccentDim : color::Transp));
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered,
                    ImGui::ColorConvertU32ToFloat4(color::AccentDim));
                ImGui::PushStyleColor(ImGuiCol_Text,
                    ImGui::ColorConvertU32ToFloat4(sel ? color::AccentFg : color::text));
                if (ImGui::Selectable(items[i], sel))
                {
                    *idx = i;
                    changed = true;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::PopStyleColor(3);
            }
            ImGui::EndPopup();
        }
        ImGui::PopStyleColor(2);
        ImGui::PopID();
        return changed;
    }

    static float bindwidth(ImGuiKey key, ImKeyBindMode mode)
    {
        const char* kn = ImGui::GetKeyName(key);
        if (!kn || !*kn) kn = "NONE";
        const char* mn = (mode == ImKeyBindMode_Hold) ? "HOLD" : "TOGG";
        const float kw = ImMax(ImGui::CalcTextSize(kn).x + 14.f, 38.f);
        const float mw = ImGui::CalcTextSize(mn).x + 10.f;
        return kw + 3.f + mw;
    }
    static void bind(const char* id, ImGuiKey* key, ImKeyBindMode* mode)
    {
        static bool    s_listen = false;
        static ImGuiID s_ownerId = 0;
        ImGui::PushID(id);
        const ImGuiID self = ImGui::GetID("##kb");

        ImDrawList* dl = ImGui::GetWindowDrawList();
        const float h = 18.f;
        const char* kn = ImGui::GetKeyName(*key);
        if (!kn || !*kn) kn = "NONE";
        const char* mn = (*mode == ImKeyBindMode_Hold) ? "HOLD" : "TOGG";
        const float kw = ImMax(ImGui::CalcTextSize(kn).x + 14.f, 38.f);
        const float mw = ImGui::CalcTextSize(mn).x + 10.f;
        const bool  listening = s_listen && (s_ownerId == self);

        const ImVec2 kp = ImGui::GetCursorScreenPos();
        ImGui::InvisibleButton("##k", { kw, h });
        if (ImGui::IsItemClicked()) { s_listen = true; s_ownerId = self; }
        if (listening) {
            for (int k = ImGuiKey_Tab; k < ImGuiKey_COUNT; k++)
                if (ImGui::IsKeyPressed((ImGuiKey)k, false))
                {
                    *key = (ImGuiKey)k;
                    s_listen = false;
                    break;
                }
        }
        dl->AddRectFilled(kp, kp + ImVec2(kw, h), listening ? color::AccentDim : color::SurfaceLift, 5.f);
        dl->AddRect(kp, kp + ImVec2(kw, h), listening ? color::Accent : color::Border, 5.f, 0, 1.f);
        dl->AddText({ kp.x + (kw - ImGui::CalcTextSize(kn).x) * .5f,
                     kp.y + (h - ImGui::GetFontSize()) * .5f },
            listening ? color::AccentFg : color::text, kn);

        ImGui::SameLine(0.f, 3.f);

        const ImVec2 mp = ImGui::GetCursorScreenPos();
        ImGui::InvisibleButton("##m", { mw, h });
        const bool mhov = ImGui::IsItemHovered();
        if (ImGui::IsItemClicked())
            *mode = (*mode == ImKeyBindMode_Hold) ? ImKeyBindMode_Toggle : ImKeyBindMode_Hold;
        dl->AddRectFilled(mp, mp + ImVec2(mw, h), mhov ? color::CardHov : color::SurfaceLift, 5.f);
        dl->AddRect(mp, mp + ImVec2(mw, h), color::Border, 5.f, 0, 1.f);
        dl->AddText({ mp.x + (mw - ImGui::CalcTextSize(mn).x) * .5f,
                     mp.y + (h - ImGui::GetFontSize()) * .5f }, color::TextMid, mn);

        ImGui::PopID();
    }

    static float keywidth(ImGuiKey key)
    {
        const char* kn = ImGui::GetKeyName(key);
        if (!kn || !*kn) kn = "NONE";
        return ImMax(ImGui::CalcTextSize(kn).x + 18.f, 48.f);
    }

    static void keyselect(const char* id, ImGuiKey* key)
    {
        static bool    s_listen = false;
        static ImGuiID s_ownerId = 0;
        ImGui::PushID(id);
        const ImGuiID self = ImGui::GetID("##ks");

        ImDrawList* dl = ImGui::GetWindowDrawList();
        const float h = 18.f;
        const char* kn = ImGui::GetKeyName(*key);
        if (!kn || !*kn) kn = "NONE";
        const float w = keywidth(*key);
        const bool listening = s_listen && (s_ownerId == self);

        const ImVec2 p = ImGui::GetCursorScreenPos();
        ImGui::InvisibleButton("##key", { w, h });
        if (ImGui::IsItemClicked()) { s_listen = true; s_ownerId = self; }
        if (listening) {
            for (int k = ImGuiKey_Tab; k < ImGuiKey_COUNT; k++)
                if (ImGui::IsKeyPressed((ImGuiKey)k, false))
                {
                    *key = (ImGuiKey)k;
                    s_listen = false;
                    break;
                }
        }

        const char* text = listening ? "..." : kn;
        const float tw = ImGui::CalcTextSize(text).x;
        dl->AddRectFilled(p, p + ImVec2(w, h), listening ? color::AccentDim : color::SurfaceLift, 5.f);
        dl->AddRect(p, p + ImVec2(w, h), listening ? color::Accent : color::Border, 5.f, 0, 1.f);
        dl->AddText({ p.x + (w - tw) * .5f, p.y + (h - ImGui::GetFontSize()) * .5f },
            listening ? color::AccentFg : color::text, text);

        ImGui::PopID();
    }

    static bool button(const char* label, ImU32 bg, ImU32 brd, ImU32 txt,
        float w = -1.f, float h = 22.f)
    {
        ImDrawList* dl = ImGui::GetWindowDrawList();
        const float W = (w < 0.f) ? ImGui::GetContentRegionAvail().x : w;
        const ImVec2 p = ImGui::GetCursorScreenPos();
        ImGui::InvisibleButton(label, { W,h });
        const bool hov = ImGui::IsItemHovered();
        const bool act = ImGui::IsItemActive();
        const ImU32 fill = act ? color::lerp(bg, color::White, .06f)
            : (hov ? color::lerp(bg, color::White, .10f) : bg);
        dl->AddRectFilled(p, p + ImVec2(W, h), fill, 5.f);
        dl->AddRect(p, p + ImVec2(W, h), hov ? color::lerp(brd, color::Accent, .35f) : brd, 5.f, 0, 1.f);
        const float tw = ImGui::CalcTextSize(label).x;
        dl->AddText({ p.x + (W - tw) * .5f, p.y + (h - ImGui::GetFontSize()) * .5f }, txt, label);
        return ImGui::IsItemClicked();
    }

    struct card {
        static bool begin(const char* id, ImVec2 size, const char* title = nullptr)
        {
            const ImVec2 p = ImGui::GetCursorScreenPos();
            ImDrawList* dl = ImGui::GetWindowDrawList();
            if (size.x > 0.f && size.y > 0.f)
            {
                dl->AddRectFilled(p + ImVec2(0.f, 4.f), p + size + ImVec2(0.f, 4.f),
                    IM_COL32(0, 0, 0, 46), 12.f);
                dl->AddRectFilled(p, p + size, IM_COL32(7, 13, 21, 235), 12.f);
            }
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::ColorConvertU32ToFloat4(color::card));
            ImGui::PushStyleColor(ImGuiCol_Border, ImGui::ColorConvertU32ToFloat4(color::Border));
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 12.f);
            ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(13.f, 12.f));
            const bool open = ImGui::BeginChild(id, size, true, ImGuiWindowFlags_None);
            if (open && title) { labelsection(title); gap(2.f); }
            return open;
        }
        static void end()
        {
            ImGui::EndChild();
            ImGui::PopStyleVar(3);
            ImGui::PopStyleColor(2);
        }
    };

}



namespace hud
{
    static ImU32 accent(float alpha = 1.f) { return color::fromfloat(global::overlay::color::Accent, alpha); }
    static ImU32 accent2(float alpha = 1.f) { return color::fromfloat(global::overlay::color::Accent2, alpha); }
    static ImU32 panelcolor(float alpha = 1.f) { return color::fromfloat(global::overlay::color::panel, alpha); }
    static ImU32 textcolor(float alpha = 1.f) { return color::fromfloat(global::overlay::color::text, alpha); }

    static void clamppanel(ImVec2& pos, ImVec2 size)
    {
        const ImVec2 display = ImGui::GetIO().DisplaySize;
        const float pad = 8.f;
        pos.x = ImClamp(pos.x, pad, ImMax(pad, display.x - size.x - pad));
        pos.y = ImClamp(pos.y, pad, ImMax(pad, display.y - size.y - pad));
    }

    static const char* keyname(ImGuiKey key)
    {
        const char* name = ImGui::GetKeyName(key);
        return name && *name ? name : "NONE";
    }

    static const char* modename(ImKeyBindMode mode)
    {
        return mode == ImKeyBindMode_Hold ? "hold" : "toggle";
    }

    static int virtualkey(ImGuiKey key)
    {
        if (key >= ImGuiKey_0 && key <= ImGuiKey_9)
            return '0' + (key - ImGuiKey_0);
        if (key >= ImGuiKey_A && key <= ImGuiKey_Z)
            return 'A' + (key - ImGuiKey_A);
        if (key >= ImGuiKey_F1 && key <= ImGuiKey_F24)
            return VK_F1 + (key - ImGuiKey_F1);
        if (key >= ImGuiKey_Keypad0 && key <= ImGuiKey_Keypad9)
            return VK_NUMPAD0 + (key - ImGuiKey_Keypad0);

        switch (key)
        {
        case ImGuiKey_Tab: return VK_TAB;
        case ImGuiKey_LeftArrow: return VK_LEFT;
        case ImGuiKey_RightArrow: return VK_RIGHT;
        case ImGuiKey_UpArrow: return VK_UP;
        case ImGuiKey_DownArrow: return VK_DOWN;
        case ImGuiKey_PageUp: return VK_PRIOR;
        case ImGuiKey_PageDown: return VK_NEXT;
        case ImGuiKey_Home: return VK_HOME;
        case ImGuiKey_End: return VK_END;
        case ImGuiKey_Insert: return VK_INSERT;
        case ImGuiKey_Delete: return VK_DELETE;
        case ImGuiKey_Backspace: return VK_BACK;
        case ImGuiKey_Space: return VK_SPACE;
        case ImGuiKey_Enter: return VK_RETURN;
        case ImGuiKey_Escape: return VK_ESCAPE;
        case ImGuiKey_LeftCtrl: return VK_LCONTROL;
        case ImGuiKey_LeftShift: return VK_LSHIFT;
        case ImGuiKey_LeftAlt: return VK_LMENU;
        case ImGuiKey_RightCtrl: return VK_RCONTROL;
        case ImGuiKey_RightShift: return VK_RSHIFT;
        case ImGuiKey_RightAlt: return VK_RMENU;
        case ImGuiKey_Menu: return VK_APPS;
        case ImGuiKey_Apostrophe: return VK_OEM_7;
        case ImGuiKey_Comma: return VK_OEM_COMMA;
        case ImGuiKey_Minus: return VK_OEM_MINUS;
        case ImGuiKey_Period: return VK_OEM_PERIOD;
        case ImGuiKey_Slash: return VK_OEM_2;
        case ImGuiKey_Semicolon: return VK_OEM_1;
        case ImGuiKey_Equal: return VK_OEM_PLUS;
        case ImGuiKey_LeftBracket: return VK_OEM_4;
        case ImGuiKey_Backslash: return VK_OEM_5;
        case ImGuiKey_RightBracket: return VK_OEM_6;
        case ImGuiKey_GraveAccent: return VK_OEM_3;
        case ImGuiKey_CapsLock: return VK_CAPITAL;
        case ImGuiKey_ScrollLock: return VK_SCROLL;
        case ImGuiKey_NumLock: return VK_NUMLOCK;
        case ImGuiKey_PrintScreen: return VK_SNAPSHOT;
        case ImGuiKey_Pause: return VK_PAUSE;
        case ImGuiKey_KeypadDecimal: return VK_DECIMAL;
        case ImGuiKey_KeypadDivide: return VK_DIVIDE;
        case ImGuiKey_KeypadMultiply: return VK_MULTIPLY;
        case ImGuiKey_KeypadSubtract: return VK_SUBTRACT;
        case ImGuiKey_KeypadAdd: return VK_ADD;
        case ImGuiKey_KeypadEnter: return VK_RETURN;
        case ImGuiKey_AppBack: return VK_BROWSER_BACK;
        case ImGuiKey_AppForward: return VK_BROWSER_FORWARD;
        case ImGuiKey_Oem102: return VK_OEM_102;
        case ImGuiKey_MouseLeft: return VK_LBUTTON;
        case ImGuiKey_MouseRight: return VK_RBUTTON;
        case ImGuiKey_MouseMiddle: return VK_MBUTTON;
        case ImGuiKey_MouseX1: return VK_XBUTTON1;
        case ImGuiKey_MouseX2: return VK_XBUTTON2;
        default: return 0;
        }
    }

    static bool asynckey(ImGuiKey key)
    {
        const int vk = virtualkey(key);
        return vk != 0 && (GetAsyncKeyState(vk) & 0x8000);
    }

    static bool keylive(bool enabled, ImGuiKey key, ImKeyBindMode mode)
    {
        if (!enabled)
            return false;
        if (mode == ImKeyBindMode_Hold)
            return asynckey(key);
        return true;
    }

    static int hotkeycount()
    {
        int count = 0;
        if (global::overlay::Hotkey_Aimbot) ++count;
        if (global::overlay::Hotkey_Silent) ++count;
        if (global::overlay::Hotkey_Fly) ++count;
        if (global::overlay::Hotkey_BladeBallSpam) ++count;
        if (global::overlay::Hotkey_Walkspeed) ++count;
        if (global::overlay::Hotkey_HitboxExpander) ++count;
        return count;
    }

    static void panelbase(ImDrawList* dl, ImVec2 p, ImVec2 s, bool hovered, bool active)
    {
        const float r = 11.f;
        const float t = hovered || active ? 1.f : 0.f;
        for (int i = 0; i < 5; i++)
        {
            const float spread = 9.f + i * 7.f;
            const int a = (int)((30.f - i * 4.8f) + t * 8.f);
            dl->AddRectFilled(p - ImVec2(spread * .45f, spread * .25f) + ImVec2(0.f, 9.f + i * 2.f),
                p + s + ImVec2(spread * .45f, spread * .62f),
                IM_COL32(0, 0, 0, a), r + spread);
        }

        dl->AddRectFilledMultiColorRounded(p - ImVec2(15.f, 10.f),
            p + s + ImVec2(15.f, 18.f),
            accent((8.f + t * 5.f) / 255.f),
            accent2((8.f + t * 5.f) / 255.f),
            IM_COL32(0, 0, 0, 0),
            IM_COL32(0, 0, 0, 0), r + 15.f);

        dl->AddRectFilled(p, p + s, panelcolor(), r);
        dl->AddRectFilledMultiColorRounded(p + ImVec2(1.f, 1.f), p + s - ImVec2(1.f, 1.f),
            IM_COL32(255, 255, 255, 10), accent(10.f / 255.f),
            IM_COL32(0, 0, 0, 70), accent2(10.f / 255.f), r - 1.f);
        dl->AddRect(p, p + s,
            color::lerp(accent(.42f), accent(.82f), t),
            r, 0, 1.f);
    }

    template <typename DrawFn>
    static void panel(const char* id, ImVec2& pos, ImVec2 size, bool movable, DrawFn draw)
    {
        clamppanel(pos, size);

        constexpr float drawPad = 30.f;
        ImGui::SetNextWindowPos(pos - ImVec2(drawPad, drawPad), ImGuiCond_Always);
        ImGui::SetNextWindowSize(size + ImVec2(drawPad * 2.f, drawPad * 2.f), ImGuiCond_Always);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.f, 0.f, 0.f, 0.f));

        ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoScrollWithMouse |
            ImGuiWindowFlags_NoFocusOnAppearing |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoBringToFrontOnFocus;

        if (!movable)
            flags |= ImGuiWindowFlags_NoInputs;

        if (ImGui::Begin(id, nullptr, flags))
        {
            ImDrawList* dl = ImGui::GetWindowDrawList();
            const ImVec2 p = ImGui::GetWindowPos() + ImVec2(drawPad, drawPad);
            ImGui::SetCursorScreenPos(p);
            ImGui::InvisibleButton("##drag", size);
            const bool hovered = movable && ImGui::IsItemHovered();
            const bool active = movable && ImGui::IsItemActive();

            if (active && ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.f))
            {
                pos += ImGui::GetIO().MouseDelta;
                clamppanel(pos, size);
            }

            draw(dl, p, size, hovered, active);
        }

        ImGui::End();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar(2);
    }

    static void watermark(ImDrawList* dl, ImVec2 p, ImVec2 s, bool hovered, bool active)
    {
        panelbase(dl, p, s, hovered, active);

        ImFont* logo = TitleFont ? TitleFont : (Tahoma_BoldXP ? Tahoma_BoldXP : ImGui::GetFont());
        const float logoSize = logo->LegacySize;
        const ImVec2 text = p + ImVec2(15.f, 7.f);
        const ImVec2 nameSize = logo->CalcTextSizeA(logoSize, FLT_MAX, 0.f, "AUTOPSY");
        char fps[32]{};
        std::snprintf(fps, sizeof(fps), "%.0f fps", ImGui::GetIO().Framerate);

        dl->AddRectFilled(p + ImVec2(7.f, 9.f), p + ImVec2(10.f, s.y - 9.f), accent(), 2.f);
        dl->AddText(logo, logoSize, text + ImVec2(1.f, 1.f), IM_COL32(0, 0, 0, 180), "AUTOPSY");
        dl->AddText(logo, logoSize, text, textcolor(), "AUTOPSY");
        dl->AddText(text + ImVec2(nameSize.x + 8.f, logoSize * .28f), accent(), "BETA");
        dl->AddText(p + ImVec2(16.f, 25.f), color::alpha(textcolor(), .52f), pcuser());

        const ImVec2 fpsSize = ImGui::CalcTextSize(fps);
        const ImVec2 pillMin = ImVec2(p.x + s.x - fpsSize.x - 28.f, p.y + 10.f);
        const ImVec2 pillMax = ImVec2(p.x + s.x - 10.f, p.y + 29.f);
        dl->AddRectFilled(pillMin, pillMax, IM_COL32(8, 18, 29, 185), 6.f);
        dl->AddRect(pillMin, pillMax, IM_COL32(0, 174, 255, 64), 6.f);
        dl->AddText(ImVec2(pillMin.x + 9.f, pillMin.y + 2.f), accent2(), fps);
    }

    static void hotkeyrow(ImDrawList* dl, ImVec2 p, float w, const char* label, ImGuiKey key, ImKeyBindMode mode, bool live)
    {
        ImGui::PushID(label);
        const ImGuiID id = ImGui::GetID("##hotkeyrow");
        ImGui::PopID();

        const float t = fx::easeoutcubic(fx::anim(id, live, 10.f));
        const ImVec2 mn = p;
        const ImVec2 mx = p + ImVec2(w, 24.f);
        const ImU32 bg = color::lerp(IM_COL32(6, 14, 23, 88), IM_COL32(0, 78, 116, 112), t);
        const ImU32 brd = color::lerp(IM_COL32(25, 48, 64, 92), IM_COL32(0, 174, 255, 132), t);

        dl->AddRectFilled(mn, mx, bg, 7.f);
        dl->AddRect(mn, mx, brd, 7.f, 0, 1.f);
        dl->AddRectFilled(mn + ImVec2(7.f, 7.f), mn + ImVec2(10.f, 17.f),
            color::lerp(color::TextDim, color::Accent, t), 2.f);
        dl->AddText(mn + ImVec2(17.f, 5.f),
            color::lerp(color::TextMid, color::White, t), label);

        const char* keyName = keyname(key);
        const char* modeName = modename(mode);
        const ImVec2 modeSize = ImGui::CalcTextSize(modeName);
        const ImVec2 keySize = ImGui::CalcTextSize(keyName);
        const float modeW = ImMax(48.f, modeSize.x + 18.f);
        const float keyW = ImMax(42.f, keySize.x + 18.f);
        const ImVec2 modeMin = ImVec2(mx.x - modeW - 8.f, mn.y + 4.f);
        const ImVec2 keyMin = ImVec2(modeMin.x - keyW - 5.f, mn.y + 4.f);

        dl->AddRectFilled(keyMin, keyMin + ImVec2(keyW, 16.f),
            color::lerp(IM_COL32(4, 12, 20, 170), IM_COL32(0, 70, 104, 205), t), 5.f);
        dl->AddRect(keyMin, keyMin + ImVec2(keyW, 16.f), color::lerp(color::Border, color::Accent, t * .8f), 5.f);
        dl->AddText(keyMin + ImVec2((keyW - keySize.x) * .5f, 1.f), color::lerp(color::text, color::AccentFg, t), keyName);

        dl->AddRectFilled(modeMin, modeMin + ImVec2(modeW, 16.f),
            color::lerp(IM_COL32(4, 12, 20, 150), IM_COL32(6, 28, 42, 190), t), 5.f);
        dl->AddRect(modeMin, modeMin + ImVec2(modeW, 16.f), color::lerp(color::BorderDim, color::AccentDim, t), 5.f);
        dl->AddText(modeMin + ImVec2((modeW - modeSize.x) * .5f, 1.f), color::lerp(color::TextMid, color::AccentFg, t), modeName);
    }

    static void hotkey(ImDrawList* dl, ImVec2 p, ImVec2 s, bool hovered, bool active)
    {
        panelbase(dl, p, s, hovered, active);

        ImFont* title = TitleFont ? TitleFont : (Tahoma_BoldXP ? Tahoma_BoldXP : ImGui::GetFont());
        const float titleSize = title->LegacySize;
        dl->AddText(title, titleSize, p + ImVec2(14.f, 11.f), textcolor(), "HOTKEYS");
        dl->AddLine(p + ImVec2(14.f, 36.f), p + ImVec2(s.x - 14.f, 36.f), color::Divider, 1.f);

        const float rowW = s.x - 28.f;
        float y = p.y + 47.f;
        auto row = [&](const char* label, ImGuiKey key, ImKeyBindMode mode, bool live)
            {
                hotkeyrow(dl, ImVec2(p.x + 14.f, y), rowW, label, key, mode, live);
                y += 30.f;
            };

        if (global::overlay::Hotkey_Aimbot)
            row("Aimbot", global::aim::Aimbot_Key, global::aim::Aimbot_Mode,
                keylive(global::aim::Enabled, global::aim::Aimbot_Key, global::aim::Aimbot_Mode));
        if (global::overlay::Hotkey_Silent)
            row("Silent", global::silent::Silent_Key, global::silent::Silent_Mode,
                keylive(global::silent::Enabled, global::silent::Silent_Key, global::silent::Silent_Mode));
        if (global::overlay::Hotkey_Fly)
            row("Fly", global::misc::Fly_Key, global::misc::Fly_Mode,
                keylive(global::misc::fly, global::misc::Fly_Key, global::misc::Fly_Mode));
        if (global::overlay::Hotkey_BladeBallSpam)
            row("Blade Spam", global::ball::SpamParry_Key, global::ball::SpamParry_Mode,
                keylive(global::ball::SpamParry, global::ball::SpamParry_Key, global::ball::SpamParry_Mode));
        if (global::overlay::Hotkey_Walkspeed)
            row("Walkspeed", ImGuiKey_None, ImKeyBindMode_Toggle, global::misc::walkspeed);
        if (global::overlay::Hotkey_HitboxExpander)
            row("Hitbox", ImGuiKey_None, ImKeyBindMode_Toggle, global::misc::hitbox);

        if (y == p.y + 47.f)
            dl->AddText(p + ImVec2(14.f, 50.f), color::alpha(textcolor(), .45f), "No hotkeys selected");
    }

    static float dot(const sdk::vector3& a, const sdk::vector3& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static bool normalize(sdk::vector3& v)
    {
        const float mag = v.magnitude();
        if (mag < .001f || std::isnan(mag))
            return false;

        v = v / mag;
        return true;
    }

    static bool partpose(const sdk::instance& inst, sdk::vector3& out, sdk::matrix3& rot)
    {
        if (!inst.Address)
            return false;

        sdk::part part(inst.Address);
        sdk::part primitive = part.primitive();
        if (!primitive.Address)
            return false;

        out = primitive.position();
        rot = primitive.rotation();
        return !(std::isnan(out.x) || std::isnan(out.y) || std::isnan(out.z));
    }

    static bool partposition(const sdk::instance& inst, sdk::vector3& out)
    {
        sdk::matrix3 ignored{};
        return partpose(inst, out, ignored);
    }

    static bool playerposition(const sdk::player& player, sdk::vector3& out)
    {
        if (partposition(player.HumanoidRootPart, out))
            return true;
        if (partposition(player.LowerTorso, out))
            return true;
        if (partposition(player.Torso, out))
            return true;
        return partposition(player.Head, out);
    }

    static bool aimray(const sdk::player& player, sdk::vector3& origin, sdk::vector3& direction)
    {
        sdk::matrix3 rotation{};
        if (!partpose(player.Head, origin, rotation) &&
            !partpose(player.HumanoidRootPart, origin, rotation) &&
            !partpose(player.Torso, origin, rotation))
            return false;

        direction = rotation * sdk::vector3{ 0.f, 0.f, -1.f };
        return normalize(direction);
    }

    static bool aiminglocal(const sdk::player& player, const sdk::vector3& localPos)
    {
        sdk::vector3 origin{};
        sdk::vector3 direction{};
        if (!aimray(player, origin, direction))
            return false;

        const sdk::vector3 toLocal = localPos - origin;
        const float along = dot(toLocal, direction);
        const float maxLength = ImClamp(global::overlay::AimView_MaxLength, 50.f, 1000.f);
        if (along < 0.f || along > maxLength)
            return false;

        const sdk::vector3 closest = origin + direction * along;
        const float miss = closest.distance(localPos);
        const float tolerance = ImClamp(toLocal.magnitude() * .018f, 4.0f, 10.0f);
        return miss <= tolerance;
    }

    static ImVec2 radardelta(const sdk::vector3& local, const sdk::vector3& target)
    {
        const sdk::vector3 delta = target - local;
        float x = delta.x;
        float y = delta.z;

        if (global::overlay::Radar_Rotate && global::camera.Address)
        {
            const sdk::matrix3 camera = global::camera.rotation();
            const sdk::vector3 forward{ -camera.data[2], -camera.data[5], -camera.data[8] };
            const float yaw = atan2f(forward.x, forward.z);
            const float c = cosf(-yaw);
            const float s = sinf(-yaw);
            const float rx = x * c - y * s;
            const float ry = x * s + y * c;
            x = rx;
            y = ry;
        }

        const float zoom = ImMax(.05f, global::overlay::Radar_Zoom);
        return ImVec2(x * zoom, y * zoom);
    }

    static void radarblip(ImDrawList* dl, ImVec2 center, ImVec2 delta, float limit, bool circle, ImU32 color, float radius)
    {
        if (circle)
        {
            const float len = sqrtf(delta.x * delta.x + delta.y * delta.y);
            if (len > limit && len > .001f)
                delta = ImVec2(delta.x / len * limit, delta.y / len * limit);
        }
        else
        {
            delta.x = ImClamp(delta.x, -limit, limit);
            delta.y = ImClamp(delta.y, -limit, limit);
        }

        const ImVec2 point = center + delta;
        dl->AddCircleFilled(point + ImVec2(0.f, 1.f), radius + 1.6f, IM_COL32(0, 0, 0, 130), 16);
        dl->AddCircleFilled(point, radius, color, 16);
        dl->AddCircle(point, radius + 2.2f, color::alpha(color, .36f), 16, 1.f);
    }

    static void radar(ImDrawList* dl, ImVec2 p, ImVec2 s, bool hovered, bool active)
    {
        panelbase(dl, p, s, hovered, active);

        const bool circle = global::overlay::Radar_Shape == 0;
        const ImVec2 center = p + s * .5f;
        const float radius = ImMin(s.x, s.y) * .5f - 18.f;
        const ImVec2 radarMin = center - ImVec2(radius, radius);
        const ImVec2 radarMax = center + ImVec2(radius, radius);

        if (circle)
        {
            dl->AddCircleFilled(center, radius, IM_COL32(2, 9, 15, 126), 96);
            dl->AddCircle(center, radius, IM_COL32(0, 174, 255, 132), 96, 1.4f);
            dl->AddCircle(center, radius * .66f, color::Divider, 96, 1.f);
            dl->AddCircle(center, radius * .33f, color::Divider, 96, 1.f);
        }
        else
        {
            dl->AddRectFilled(radarMin, radarMax, IM_COL32(2, 9, 15, 126), 5.f);
            dl->AddRect(radarMin, radarMax, IM_COL32(0, 174, 255, 132), 5.f, 0, 1.4f);
            dl->AddRect(center - ImVec2(radius * .66f, radius * .66f),
                center + ImVec2(radius * .66f, radius * .66f), color::Divider, 3.f);
            dl->AddRect(center - ImVec2(radius * .33f, radius * .33f),
                center + ImVec2(radius * .33f, radius * .33f), color::Divider, 3.f);
        }

        dl->AddLine(ImVec2(center.x - radius, center.y), ImVec2(center.x + radius, center.y), color::alpha(color::Accent, .28f), 1.f);
        dl->AddLine(ImVec2(center.x, center.y - radius), ImVec2(center.x, center.y + radius), color::alpha(color::Accent, .28f), 1.f);

        sdk::vector3 localPos{};
        if (playerposition(global::LocalPlayer, localPos))
        {
            for (const auto& player : global::Player_Cache)
            {
                if (!player.character.Address)
                    continue;
                if (global::LocalPlayer.character.Address &&
                    player.character.Address == global::LocalPlayer.character.Address)
                    continue;

                sdk::vector3 playerPos{};
                if (!playerposition(player, playerPos))
                    continue;

                const float dist = localPos.distance(playerPos);
                const float fade = 1.f - ImClamp(dist / 900.f, 0.f, .55f);
                radarblip(dl, center, radardelta(localPos, playerPos), radius - 8.f, circle,
                    color::alpha(color::AccentFg, fade), 3.6f);
            }
        }

        const ImVec2 tri[3] = {
            center + ImVec2(0.f, -7.f),
            center + ImVec2(5.5f, 6.f),
            center + ImVec2(-5.5f, 6.f)
        };
        dl->AddTriangleFilled(tri[0] + ImVec2(0.f, 1.f), tri[1] + ImVec2(0.f, 1.f), tri[2] + ImVec2(0.f, 1.f), IM_COL32(0, 0, 0, 150));
        dl->AddTriangleFilled(tri[0], tri[1], tri[2], textcolor());

        ImFont* title = Tahoma_BoldXP ? Tahoma_BoldXP : ImGui::GetFont();
        dl->AddText(title, title->LegacySize, p + ImVec2(14.f, 10.f), textcolor(), "RADAR");
        char zoomText[32]{};
        std::snprintf(zoomText, sizeof(zoomText), "%.2fx", global::overlay::Radar_Zoom);
        const ImVec2 zSize = ImGui::CalcTextSize(zoomText);
        dl->AddText(ImVec2(p.x + s.x - zSize.x - 14.f, p.y + 11.f), accent2(), zoomText);
    }

    struct aimthreat
    {
        int Count = 0;
        std::string name;
    };

    static aimthreat getthreat()
    {
        aimthreat threat{};

        sdk::vector3 localPos{};
        if (!playerposition(global::LocalPlayer, localPos))
            return threat;

        for (const auto& player : global::Player_Cache)
        {
            if (!player.character.Address)
                continue;
            if (global::LocalPlayer.character.Address &&
                player.character.Address == global::LocalPlayer.character.Address)
                continue;

            if (!aiminglocal(player, localPos))
                continue;

            ++threat.Count;
            if (threat.name.empty())
                threat.name = !player.Display_Name.empty() ? player.Display_Name : player.name;
        }

        return threat;
    }

    static void aimwarning(ImDrawList* dl)
    {
        const aimthreat threat = getthreat();
        if (threat.Count <= 0)
            return;

        const ImVec2 display = ImGui::GetIO().DisplaySize;
        const float pulse = (sinf((float)ImGui::GetTime() * 6.0f) + 1.f) * .5f;
        const char* title = "AIM WARNING";
        char detail[96]{};
        if (threat.Count == 1)
            std::snprintf(detail, sizeof(detail), "%s is aiming at you", threat.name.c_str());
        else
            std::snprintf(detail, sizeof(detail), "%dx players aiming at you", threat.Count);

        ImFont* font = Tahoma_BoldXP ? Tahoma_BoldXP : ImGui::GetFont();
        const float fontSize = font->LegacySize;
        const ImVec2 titleSize = font->CalcTextSizeA(fontSize, FLT_MAX, 0.f, title);
        const ImVec2 detailSize = ImGui::CalcTextSize(detail);
        const float width = ImClamp(ImMax(titleSize.x, detailSize.x) + 74.f, 258.f, 420.f);
        const ImVec2 warnMin(display.x * .5f - width * .5f, 72.f);
        const ImVec2 warnMax(warnMin.x + width, warnMin.y + 58.f);

        for (int i = 0; i < 4; ++i)
        {
            const float spread = 5.f + i * 5.f;
            dl->AddRectFilled(warnMin - ImVec2(spread, spread * .45f) + ImVec2(0.f, 5.f + i),
                warnMax + ImVec2(spread, spread * .7f),
                IM_COL32(0, 0, 0, 42 - i * 7), 10.f + spread);
        }

        dl->AddRectFilled(warnMin, warnMax, IM_COL32(23, 6, 12, 222), 9.f);
        dl->AddRectFilledMultiColorRounded(warnMin + ImVec2(1.f, 1.f), warnMax - ImVec2(1.f, 1.f),
            IM_COL32(255, 80, 104, 40 + (int)(pulse * 34.f)),
            IM_COL32(100, 117, 255, 24),
            IM_COL32(0, 0, 0, 82),
            IM_COL32(255, 80, 104, 18), 9.f);
        dl->AddRect(warnMin, warnMax, IM_COL32(255, 80, 104, 170 + (int)(pulse * 60.f)), 9.f, 0, 1.35f);

        const ImVec2 icon(warnMin.x + 22.f, warnMin.y + 29.f);
        dl->AddTriangleFilled(icon + ImVec2(0.f, -12.f), icon + ImVec2(12.f, 10.f), icon + ImVec2(-12.f, 10.f),
            IM_COL32(255, 80, 104, 235));
        dl->AddText(font, fontSize, ImVec2(icon.x - 3.f, icon.y - 8.f), IM_COL32(23, 6, 12, 255), "!");

        dl->AddText(font, fontSize, ImVec2(warnMin.x + 48.f, warnMin.y + 9.f), IM_COL32(255, 238, 241, 255), title);
        dl->AddText(ImVec2(warnMin.x + 48.f, warnMin.y + 33.f), IM_COL32(255, 178, 190, 255), detail);
    }

    static void render(bool movable)
    {
        if (global::overlay::AimWarning)
            aimwarning(ImGui::GetBackgroundDrawList());

        if (global::overlay::watermark)
            panel("##autopsy_watermark", global::overlay::Watermark_Pos, ImVec2(258.f, 42.f), movable, watermark);
        if (global::overlay::hotkey)
        {
            const int rows = hotkeycount();
            panel("##autopsy_hotkeys", global::overlay::Hotkeys_Pos,
                ImVec2(258.f, 52.f + ImMax(1, rows) * 30.f), movable, hotkey);
        }
        if (global::overlay::radar)
        {
            const float radarSize = ImClamp(global::overlay::Radar_Size, 130.f, 280.f);
            panel("##autopsy_radar", global::overlay::Radar_Pos, ImVec2(radarSize, radarSize), movable, radar);
        }
    }
}


static void dissolve(ImDrawList* dl, ImVec2 mn, ImVec2 mx, float t, float time)
{
    const float a = sinf(fx::saturate(t) * 3.14159265f);
    if (a <= .01f)
        return;

    const float w = mx.x - mn.x;
    const float h = mx.y - mn.y;
    for (int i = 0; i < 30; i++)
    {
        const float seed = (float)i * 12.9898f;
        const float u = fmodf(fabsf(sinf(seed) * 43758.5453f), 1.f);
        const float v = fmodf(fabsf(sinf(seed + 41.371f) * 24634.6345f), 1.f);
        const float drift = sinf(time * 2.1f + seed) * 5.f * (1.f - fabsf(t - .5f) * 1.55f);
        const float size = 1.0f + v * 1.8f;
        ImVec2 p;

        if ((i & 3) == 0)
            p = { mn.x + u * w, mn.y - 11.f + drift };
        else if ((i & 3) == 1)
            p = { mn.x + u * w, mx.y + 8.f + drift };
        else if ((i & 3) == 2)
            p = { mn.x - 10.f + drift, mn.y + u * h };
        else
            p = { mx.x + 8.f + drift, mn.y + u * h };

        const int alpha = (int)(a * (14.f + v * 28.f));
        const ImU32 col = (i % 5) == 0 ? IM_COL32(100, 117, 255, alpha) : IM_COL32(0, 174, 255, alpha);
        dl->AddRectFilled(p, p + ImVec2(size, size), col, 1.5f);
    }
}

static bool focusroblox()
{
    HWND roblox = FindWindowA(nullptr, "Roblox");
    if (!roblox || !IsWindow(roblox))
        return false;

    if (GetForegroundWindow() == roblox)
        return true;

    if (IsIconic(roblox))
        ShowWindow(roblox, SW_RESTORE);

    const DWORD currentThread = GetCurrentThreadId();
    const DWORD targetThread = GetWindowThreadProcessId(roblox, nullptr);
    HWND foreground = GetForegroundWindow();
    const DWORD foregroundThread = foreground ? GetWindowThreadProcessId(foreground, nullptr) : 0;

    const bool attachForeground = foregroundThread != 0 && foregroundThread != currentThread;
    const bool attachTarget = targetThread != 0 && targetThread != currentThread && targetThread != foregroundThread;

    if (attachForeground)
        AttachThreadInput(currentThread, foregroundThread, TRUE);
    if (attachTarget)
        AttachThreadInput(currentThread, targetThread, TRUE);

    BringWindowToTop(roblox);
    const bool focused = SetForegroundWindow(roblox) != FALSE;

    if (attachTarget)
        AttachThreadInput(currentThread, targetThread, FALSE);
    if (attachForeground)
        AttachThreadInput(currentThread, foregroundThread, FALSE);

    return focused || GetForegroundWindow() == roblox;
}

static void keepfocus(float elapsed)
{
    static double lastFocusAttempt = -1.0;
    const double now = ImGui::GetTime();
    if (elapsed > 4.0f || (lastFocusAttempt >= 0.0 && now - lastFocusAttempt < .35))
        return;

    lastFocusAttempt = now;
    focusroblox();
}

static void welcome(bool menuOpen)
{
    static double startTime = -1.0;
    if (startTime < 0.0)
        startTime = ImGui::GetTime();

    const float elapsed = (float)(ImGui::GetTime() - startTime);
    if (elapsed > 4.85f)
        return;

    if (!menuOpen)
        keepfocus(elapsed);

    const float inT = fx::easeoutcubic(ImClamp(elapsed / .55f, 0.f, 1.f));
    const float outT = elapsed <= 4.0f ? 1.f : 1.f - fx::easeoutcubic(ImClamp((elapsed - 4.0f) / .85f, 0.f, 1.f));
    const float alpha = inT * outT;
    if (alpha <= .01f)
        return;

    ImDrawList* dl = ImGui::GetForegroundDrawList();
    const ImVec2 display = ImGui::GetIO().DisplaySize;
    const ImVec2 center = display * .5f;
    const float pulse = (sinf(elapsed * 4.2f) + 1.f) * .5f;

    dl->AddRectFilled({ 0.f, 0.f }, display, IM_COL32(2, 5, 9, (int)(232.f * alpha)));
    dl->AddRectFilledMultiColor({ 0.f, 0.f }, display,
        IM_COL32(0, 174, 255, (int)(18.f * alpha)),
        IM_COL32(100, 117, 255, (int)(14.f * alpha)),
        IM_COL32(0, 0, 0, 0),
        IM_COL32(0, 0, 0, 0));

    for (int i = 0; i < 5; ++i)
    {
        const float r = 54.f + i * 34.f + pulse * 12.f;
        dl->AddCircle(center, r, IM_COL32(0, 174, 255, (int)((42.f - i * 6.f) * alpha)), 96, 1.2f);
    }

    ImFont* logo = LogoFont ? LogoFont : (TitleFont ? TitleFont : ImGui::GetFont());
    const float logoSize = logo->LegacySize * 1.65f;
    const char* title = "AUTOPSY";
    const ImVec2 titleSize = logo->CalcTextSizeA(logoSize, FLT_MAX, 0.f, title);
    const ImVec2 titlePos(center.x - titleSize.x * .5f, center.y - 34.f);

    dl->AddText(logo, logoSize, titlePos + ImVec2(0.f, 3.f), IM_COL32(0, 0, 0, (int)(190.f * alpha)), title);
    dl->AddText(logo, logoSize, titlePos, IM_COL32(242, 250, 255, (int)(255.f * alpha)), title);
    dl->AddText(titlePos + ImVec2(titleSize.x + 8.f, 6.f), IM_COL32(0, 174, 255, (int)(245.f * alpha)), "BETA");

    const char* sub = "loading overlay";
    const ImVec2 subSize = ImGui::CalcTextSize(sub);
    dl->AddText(ImVec2(center.x - subSize.x * .5f, center.y + 14.f), IM_COL32(146, 166, 178, (int)(230.f * alpha)), sub);

    const float barW = ImMin(360.f, display.x * .42f);
    const float progress = ImClamp(elapsed / 4.0f, 0.f, 1.f);
    const ImVec2 barMin(center.x - barW * .5f, center.y + 48.f);
    const ImVec2 barMax(center.x + barW * .5f, center.y + 54.f);
    dl->AddRectFilled(barMin, barMax, IM_COL32(12, 26, 38, (int)(220.f * alpha)), 3.f);
    dl->AddRectFilled(barMin, ImVec2(barMin.x + barW * progress, barMax.y), IM_COL32(0, 174, 255, (int)(255.f * alpha)), 3.f);
    dl->AddRect(barMin, barMax, IM_COL32(210, 247, 255, (int)(82.f * alpha)), 3.f);
}

void graphic::menu()
{
    color::refresh();

    static int Section = 0;
    static int LastSection = 0;
    static float SectionAlpha = 1.f;
    static int HeaderPrevSection = 0;
    static float HeaderAlpha = 1.f;
    static float MenuT = 0.f;
    static bool MenuPosReady = false;
    static ImVec2 MenuPos = {};

    ImGuiIO& IO = ImGui::GetIO();

    const bool compact = global::setting::Compact_UI;

    const float kWinW = compact ? 674.f : 760.f;
    const float kWinH = compact ? 512.f : 548.f;
    constexpr float kR = 14.f;
    const float sbW = compact ? side::kCompactW : side::kW;
    const float kHeaderH = compact ? 48.f : 58.f;

    MenuT = fx::damp(MenuT, Running ? 1.f : 0.f, Running ? 8.5f : 7.2f);
    if (!Running && MenuT <= .01f)
        return;
    const float MenuEase = fx::easeoutquint(MenuT);
    const float MenuAlpha = fx::easeoutcubic(MenuT);
    const float BackdropT = fx::easeinoutcubic(MenuT);
    const ImVec2 MenuOffset = { 0.f, (1.f - MenuEase) * 18.f };

    if (!MenuPosReady)
    {
        MenuPos = IO.DisplaySize / 2.f;
        MenuPosReady = true;
    }

    const float minX = kWinW * .5f;
    const float maxX = IO.DisplaySize.x - kWinW * .5f;
    const float minY = kWinH * .5f;
    const float maxY = IO.DisplaySize.y - kWinH * .5f;
    MenuPos.x = maxX > minX ? ImClamp(MenuPos.x, minX, maxX) : IO.DisplaySize.x * .5f;
    MenuPos.y = maxY > minY ? ImClamp(MenuPos.y, minY, maxY) : IO.DisplaySize.y * .5f;

    const ImVec2 menuMin = MenuPos + MenuOffset - ImVec2(kWinW, kWinH) * .5f;
    const ImVec2 menuMax = menuMin + ImVec2(kWinW, kWinH);
    ImDrawList* BDL = ImGui::GetBackgroundDrawList();
    BDL->AddRectFilled({ 0.f, 0.f }, IO.DisplaySize, IM_COL32(2, 4, 8, (int)(BackdropT * 18.f)));
    for (int i = 0; i < 5; i++)
    {
        const float spread = 18.f + i * 11.f;
        const int alpha = (int)((12.f - i * 1.85f) * BackdropT);
        BDL->AddRectFilled(menuMin - ImVec2(spread, spread * .55f),
            menuMax + ImVec2(spread, spread), IM_COL32(7, 13, 21, alpha), kR + spread);
    }
    BDL->AddRectFilledMultiColorRounded(menuMin - ImVec2(28.f, 20.f),
        menuMax + ImVec2(28.f, 32.f),
        IM_COL32(0, 174, 255, (int)(BackdropT * 5.f)),
        IM_COL32(100, 117, 255, (int)(BackdropT * 5.f)),
        IM_COL32(0, 0, 0, 0),
        IM_COL32(0, 0, 0, 0), kR + 26.f);
    dissolve(BDL, menuMin, menuMax, MenuT, (float)ImGui::GetTime());

    ImGui::SetNextWindowSize({ kWinW, kWinH }, ImGuiCond_Always);
    ImGui::SetNextWindowPos(MenuPos + MenuOffset, ImGuiCond_Always, { 0.5f, 0.5f });

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, MenuAlpha);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImGui::ColorConvertU32ToFloat4(color::Win));

    const bool open = ImGui::Begin("##autopsy.lol", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoBackground);

    ImGui::PopStyleColor();
    ImGui::PopStyleVar(3);
    if (!open) { ImGui::End(); return; }

    ImVec2 WP = ImGui::GetWindowPos();
    const ImVec2 WS = ImGui::GetWindowSize();
    ImDrawList* DL = ImGui::GetWindowDrawList();

    const float time = (float)ImGui::GetTime();
    const float glow = (sinf(time * 2.3f) + 1.f) * .5f;

    auto dragMenu = [&]()
        {
            if (Running && ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.f))
            {
                MenuPos += IO.MouseDelta;
                MenuPos.x = maxX > minX ? ImClamp(MenuPos.x, minX, maxX) : IO.DisplaySize.x * .5f;
                MenuPos.y = maxY > minY ? ImClamp(MenuPos.y, minY, maxY) : IO.DisplaySize.y * .5f;
                ImGui::SetWindowPos(MenuPos + MenuOffset - WS * .5f, ImGuiCond_Always);
                WP = ImGui::GetWindowPos();
            }
        };

    ImGui::SetCursorScreenPos(WP);
    ImGui::InvisibleButton("##drag_logo", { sbW, compact ? 8.f : side::kLogoH });
    dragMenu();
    if (compact)
    {
        const float dragTop = 8.f + side::kTabH * 6.f + 10.f;
        if (dragTop < WS.y)
        {
            ImGui::SetCursorScreenPos({ WP.x, WP.y + dragTop });
            ImGui::InvisibleButton("##drag_sidebar_bottom", { sbW, WS.y - dragTop });
            dragMenu();
        }
    }
    ImGui::SetCursorScreenPos({ WP.x + sbW, WP.y });
    ImGui::InvisibleButton("##drag_header", { WS.x - sbW, kHeaderH });
    dragMenu();

    DL->PushClipRect(WP - ImVec2(40.f, 40.f), WP + WS + ImVec2(40.f, 40.f), false);
    DL->AddRectFilledMultiColorRounded(WP - ImVec2(12.f, 2.f) + ImVec2(0.f, 20.f),
        WP + WS + ImVec2(12.f, 34.f),
        IM_COL32(0, 0, 0, 0), IM_COL32(0, 0, 0, 0),
        IM_COL32(0, 0, 0, (int)(MenuEase * 24.f)), IM_COL32(0, 0, 0, (int)(MenuEase * 24.f)), kR + 18.f);
    DL->AddRectFilledMultiColorRounded(WP - ImVec2(5.f, 1.f) + ImVec2(0.f, 10.f),
        WP + WS + ImVec2(5.f, 18.f),
        IM_COL32(0, 0, 0, (int)(MenuEase * 8.f)), IM_COL32(0, 0, 0, (int)(MenuEase * 8.f)),
        IM_COL32(0, 0, 0, (int)(MenuEase * 48.f)), IM_COL32(0, 0, 0, (int)(MenuEase * 48.f)), kR + 8.f);
    DL->AddRectFilled(WP + ImVec2(0.f, 5.f), WP + WS + ImVec2(0.f, 5.f),
        IM_COL32(0, 0, 0, (int)(MenuEase * 72.f)), kR + 2.f);
    DL->AddRectFilled(WP, WP + WS, IM_COL32(3, 6, 10, 58), kR);
    DL->AddRectFilled(WP + ImVec2(1.f, 1.f), WP + WS - ImVec2(1.f, 1.f),
        IM_COL32(0, 174, 255, 7), kR - 1.f);
    DL->AddRect(WP, WP + WS, color::Border, kR, 0, 1.f);
    DL->AddRect(WP + ImVec2(1.f, 1.f), WP + WS - ImVec2(1.f, 1.f),
        color::lerp(IM_COL32(0, 174, 255, 28), IM_COL32(100, 117, 255, 24), glow), kR, 0, 1.f);

    const ImVec2 sideMin = WP + ImVec2(1.f, 1.f);
    const ImVec2 sideMax = { WP.x + sbW, WP.y + WS.y - 1.f };
    DL->AddRectFilled(sideMin, sideMax, IM_COL32(4, 10, 17, 118), kR, ImDrawFlags_RoundCornersLeft);
    DL->AddRectFilledMultiColorRounded(sideMin, sideMax,
        IM_COL32(255, 255, 255, 10), IM_COL32(255, 255, 255, 2),
        IM_COL32(0, 0, 0, 68), IM_COL32(0, 174, 255, 12), kR, ImDrawFlags_RoundCornersLeft);
    DL->AddRectFilledMultiColorRounded(sideMin + ImVec2(1.f, 1.f),
        ImVec2(sideMax.x - 1.f, sideMin.y + 92.f),
        IM_COL32(255, 255, 255, 8), IM_COL32(0, 174, 255, 5),
        IM_COL32(255, 255, 255, 1), IM_COL32(0, 0, 0, 0), kR - 1.f, ImDrawFlags_RoundCornersTopLeft);
    DL->AddRect(sideMin, sideMax, IM_COL32(255, 255, 255, 18), kR, ImDrawFlags_RoundCornersLeft, 1.f);
    DL->AddLine({ WP.x + sbW, WP.y + 10.f }, { WP.x + sbW, WP.y + WS.y - 10.f }, IM_COL32(96, 210, 255, 42), 1.f);
    DL->PopClipRect();

    if (!compact)
    {
        const ImVec2 lp = { WP.x + 30.f, WP.y + 25.f };
        ImFont* logo = LogoFont ? LogoFont : (Tahoma_BoldXP ? Tahoma_BoldXP : ImGui::GetFont());
        const float logoSize = logo->LegacySize * 1.12f;
        const ImVec2 aw = logo->CalcTextSizeA(logoSize, FLT_MAX, 0.f, "AUTOPSY");
        DL->AddRectFilled(lp + ImVec2(-8.f, 3.f), lp + ImVec2(-5.f, 25.f), color::Accent, 2.f);
        DL->AddText(logo, logoSize, lp + ImVec2(1.f, 2.f), IM_COL32(0, 0, 0, 190), "AUTOPSY");
        DL->AddText(logo, logoSize, lp, color::White, "AUTOPSY");
        DL->AddText(lp + ImVec2(aw.x + 7.f, 6.f), color::Accent, "BETA");
        DL->AddRectFilled({ WP.x + 20.f, WP.y + side::kLogoH - 13.f },
            { WP.x + sbW - 20.f, WP.y + side::kLogoH - 12.f }, color::Divider, 1.f);
    }

    using Fn = void(*)(ImDrawList*, ImVec2, float, ImU32);
    constexpr Fn icons[] = { icon::crosshair, icon::eye, icon::globe, icon::layer, icon::diamond, icon::gear };
    constexpr const char* labels[] = { "Aimbot", "Visuals", "World", "Miscellaneous", "Blade Ball", "Settings" };
    constexpr int kTabs = 6;

    const float tabY = compact ? 14.f : side::kLogoH + 28.f;

    if (!compact)
        DL->AddText({ WP.x + 20.f, WP.y + side::kLogoH + 7.f }, color::TextDim, "MAIN");

    for (int i = 0; i < kTabs; i++)
        if (side::tab(DL, WP, tabY, sbW, compact, i, Section, icons[i], labels[i]))
            Section = i;

    if (!compact)
    {
        DL->AddRectFilled({ WP.x + 20.f, WP.y + WS.y - 66.f },
            { WP.x + sbW - 20.f, WP.y + WS.y - 65.f }, color::Divider, 1.f);
        DL->AddText({ WP.x + 20.f, WP.y + WS.y - 49.f }, color::TextDim, "Signed in");
        DL->AddText({ WP.x + 20.f, WP.y + WS.y - 30.f }, color::text, pcuser());
    }

    if (LastSection != Section)
    {
        HeaderPrevSection = LastSection;
        LastSection = Section;
        SectionAlpha = 0.f;
        HeaderAlpha = 0.f;
    }
    SectionAlpha = fx::damp(SectionAlpha, 1.f, 8.5f);
    const float SectionEase = fx::easeoutcubic(SectionAlpha);
    HeaderAlpha = fx::damp(HeaderAlpha, 1.f, 9.5f);
    const float HeaderEase = fx::easeoutcubic(HeaderAlpha);

    const float contentX = WP.x + sbW + 1.f;
    const float contentW = WS.x - sbW - 1.f;
    const float bodyY = WP.y + kHeaderH + 1.f;
    const float bodyH = WS.y - kHeaderH - 1.f;

    DL->AddRectFilled({ contentX, WP.y + 1.f }, { contentX + contentW - 1.f, WP.y + kHeaderH }, color::Surface, kR, ImDrawFlags_RoundCornersTopRight);
    DL->AddRectFilled({ contentX, WP.y + 1.f }, { contentX + contentW - 1.f, WP.y + kHeaderH },
        IM_COL32(0, 174, 255, 5), kR, ImDrawFlags_RoundCornersTopRight);
    DL->AddLine({ contentX, WP.y + kHeaderH }, { contentX + contentW, WP.y + kHeaderH }, color::Divider, 1.f);

    constexpr const char* kTabTitles[] = { "AIMBOT","VISUALS","WORLD","MISC","BLADE BALL","SETTINGS" };
    {
        const ImVec2 tp = { contentX + 18.f, WP.y + 12.f };
        ImFont* titleFont = TitleFont ? TitleFont : (Tahoma_BoldXP ? Tahoma_BoldXP : ImGui::GetFont());
        const float titleSize = titleFont->LegacySize;
        auto drawTitle = [&](int idx, float a, ImVec2 off)
            {
                const ImVec2 p = tp + off;
                DL->AddText(titleFont, titleSize, p + ImVec2(0.f, 1.f), color::alpha(IM_COL32(0, 0, 0, 180), a), kTabTitles[idx]);
                DL->AddText(titleFont, titleSize, p, color::alpha(color::Accent, a), kTabTitles[idx]);
                DL->AddText({ p.x, p.y + 24.f }, color::alpha(color::TextDim, a), "settings");
            };
        if (HeaderEase < .995f)
            drawTitle(HeaderPrevSection, 1.f - HeaderEase, { -HeaderEase * 14.f, -HeaderEase * 4.f });
        drawTitle(Section, HeaderEase, { (1.f - HeaderEase) * 18.f, (1.f - HeaderEase) * 5.f });
        const char* brand = "autopsy.lol";
        const float bw = ImGui::CalcTextSize(brand).x;
        const ImVec2 bp = { contentX + contentW - bw - 22.f, WP.y + 20.f };
        DL->AddText(bp, color::White, brand);
        DL->AddCircleFilled({ bp.x - 12.f, bp.y + 7.f }, 3.f, color::Accent, 16);
    }

    DL->AddRectFilled({ contentX, bodyY }, { contentX + contentW - 1.f, WP.y + WS.y - 1.f }, IM_COL32(5, 9, 15, 255), kR, ImDrawFlags_RoundCornersBottomRight);

    constexpr float kPad = 12.f;
    ImGui::SetCursorScreenPos({ contentX + kPad + (1.f - SectionEase) * 18.f, bodyY + kPad });
    ImGui::PushClipRect({ contentX, bodyY }, { contentX + contentW, WP.y + WS.y }, true);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, SectionEase);
    ImGui::BeginGroup();

    const float bInW = contentW - kPad * 2.f;
    const float bInH = bodyH - kPad * 2.f;
    const float halfW = (bInW - 8.f) * .5f;
    const float halfH = (bInH - 8.f) * .5f;


    if (Section == 0)
    {
        if (ui::card::begin("##abt", { halfW, halfH }, "AIMBOT"))
        {
            ui::toggle("Enabled", &global::aim::Enabled);
            ImGui::SameLine(ImGui::GetContentRegionMax().x - ui::bindwidth(global::aim::Aimbot_Key, global::aim::Aimbot_Mode) - 4.f);
            ui::bind("##abk", &global::aim::Aimbot_Key, &global::aim::Aimbot_Mode);

            ui::gap(2.f);
            ui::toggle("Sticky Aim", &global::aim::AimbotSticky);
            ui::toggle("Knocked Check", &global::aim::KnockedCheck);
            ui::sliderint("Hit Chance", &global::aim::HitChance, 0, 100);
            ui::gap(4.f);

            ui::combo("Type", &global::aim::Aimbot_type, { "Mouse","Camera" });
            ui::gap(2.f);
            ui::combo("HitPart", &global::aim::HitPart, { "Head","Torso","LowerTorso" });
            ui::gap(6.f);

            ui::toggle("Prediction", &global::aim::Prediction);
            if (global::aim::Prediction) {
                ui::toggle("Auto Prediction", &global::aim::AutoPrediction);
                if (!global::aim::AutoPrediction) {
                    ui::sliderfloat("Pred X", &global::aim::PredictionX, 0.f, .5f);
                    ui::sliderfloat("Pred Y", &global::aim::PredictionY, 0.f, .5f);
                    ui::sliderfloat("Pred Z", &global::aim::PredictionZ, 0.f, .5f);
                }
            }
            ui::gap(4.f);

            if (global::aim::Aimbot_type == 0) {
                ui::sliderfloat("Smooth X", &global::aim::mouse::Smoothing_X, 0.f, 12.f);
                ui::sliderfloat("Smooth Y", &global::aim::mouse::Smoothing_Y, 0.f, 12.f);
                ui::sliderfloat("Sensitivity", &global::aim::mouse::Mouse_Sensitivty, 0.f, 5.f);
            }
            else {
                ui::sliderfloat("Smooth X", &global::aim::camera::Smoothing_X, 0.f, 12.f);
                ui::sliderfloat("Smooth Y", &global::aim::camera::Smoothing_Y, 0.f, 12.f);
            }
        }
        ui::card::end();

        ImGui::SameLine(0.f, 6.f);

        if (ui::card::begin("##sil", { halfW, halfH }, "SILENT AIM"))
        {
            ui::toggle("Enabled", &global::silent::Enabled);
            ImGui::SameLine(ImGui::GetContentRegionMax().x - ui::bindwidth(global::silent::Silent_Key, global::silent::Silent_Mode) - 4.f);
            ui::bind("##sik", &global::silent::Silent_Key, &global::silent::Silent_Mode);

            ui::gap(2.f);
            ui::toggle("Sticky Aim", &global::silent::StickyAim);
            ui::toggle("Spoof Mouse", &global::silent::SpoofMouse);
            ui::toggle("Knocked Check", &global::silent::KnockedCheck);
            ui::gap(4.f);

            ui::combo("Hit Part", &global::silent::AimPart, { "Head","Torso","LowerTorso" });
            ui::gap(4.f);
            ui::toggle("Prediction", &global::silent::Prediction);
            if (global::silent::Prediction) {
                ui::toggle("Auto Prediction", &global::silent::AutoPrediction);
                if (!global::silent::AutoPrediction) {
                    ui::sliderfloat("Pred X", &global::silent::PredictionX, 0.f, .5f);
                    ui::sliderfloat("Pred Y", &global::silent::PredictionY, 0.f, .5f);
                    ui::sliderfloat("Pred Z", &global::silent::PredictionZ, 0.f, .5f);
                }
            }
        }
        ui::card::end();

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.f);
        const float r2H = halfH - 6.f;

        if (ui::card::begin("##abfov", { halfW, r2H }, "AIMBOT FOV"))
        {
            if (global::aim::Enabled)
            {
                ui::togglecolor("Draw FOV", &global::aim::DrawFov,
                    "##abfc", global::aim::FovColor);
                if (global::aim::DrawFov) {
                    ui::sliderfloat("Radius", &global::aim::FovSize, 1.f, 500.f);
                    ui::toggle("Spin", &global::aim::FovSpin);
                    if (global::aim::FovSpin)
                        ui::sliderint("Spin Speed", &global::aim::FovSpinSpeed, 1, 5);
                    ui::toggle("Constrain to FOV", &global::aim::useFov);
                }
            }
            else {
                ImGui::Dummy({ 0.f, 4.f });
                ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::TextDim),
                    "Enable aimbot first");
            }
        }
        ui::card::end();

        ImGui::SameLine(0.f, 6.f);

        if (ui::card::begin("##sifov", { halfW, r2H }, "SILENT FOV"))
        {
            if (global::silent::Enabled)
            {
                ui::togglecolor("Draw FOV", &global::silent::DrawFov,
                    "##sifc", global::silent::FovColor);
                ui::gap(2.f);
                ui::toggle("Gun-Based FOV", &global::silent::GunBasedFov);
                if (global::silent::GunBasedFov) {
                    ui::sliderfloat("Default", &global::silent::fov, 0.f, 300.f);
                    ui::sliderfloat("Dbl Barrel", &global::silent::FovDoubleBarrel, 0.f, 300.f);
                    ui::sliderfloat("Tactical", &global::silent::FovTacticalShotgun, 0.f, 300.f);
                    ui::sliderfloat("Revolver", &global::silent::FovRevolver, 0.f, 300.f);
                }
                else {
                    ui::sliderfloat("Static FOV", &global::silent::fov, 0.f, 500.f);
                }
                ui::toggle("Spin FOV", &global::silent::FovSpin);
                if (global::silent::FovSpin)
                    ui::sliderint("Spin Speed", &global::silent::FovSpinSpeed, 1, 5);
                ui::toggle("Constrain to FOV", &global::silent::UseFov);
            }
            else {
                ImGui::Dummy({ 0.f, 4.f });
                ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::TextDim),
                    "Enable silent aim first");
            }
        }
        ui::card::end();
    }


    if (Section == 1)
    {
        if (ui::card::begin("##esp", { halfW, bInH }, "ESP TOGGLES"))
        {
            ui::toggle("Master Enable", &global::esp::Enabled);
            ui::gap(4.f);

            ui::labelsection("BOX");
            ui::togglecolor("Box", &global::esp::Box, "##bxc", global::esp::color::Box);
            if (global::esp::Box) {
                ui::combo("Style", &global::esp::Box_Type, { "Bounding","Corner" });
                ui::gap(2.f);
                ui::toggle("Box Fill", &global::esp::Box_Fill);
                if (global::esp::Box_Fill) {
                    ui::dualcolor("##bft", global::esp::color::BoxFill_Top,
                        "##bfb", global::esp::color::BoxFill_Bottom);
                    ui::toggle("Gradient", &global::esp::Box_Fill_Gradient);
                    if (global::esp::Box_Fill_Gradient) {
                        ui::toggle("Rotation", &global::esp::Box_Fill_Gradient_Rotate);
                        if (global::esp::Box_Fill_Gradient_Rotate) {
                            ui::combo("Rotation Type", &global::esp::Box_Fill_Type,
                                { "Side","Bottom","Spin" });
                            ui::sliderint("Speed", &global::esp::BoxFillSpeed, 1, 5);
                        }
                    }
                }
            }
            ui::gap(4.f);

            ui::labelsection("HEALTH");
            ui::togglecolor("Health Bar", &global::esp::Healthbar,
                "##hbc", global::esp::color::Healthbar);
            if (global::esp::Healthbar) {
                ui::combo("Bar Style", &global::esp::Healthbar_Type, { "Static","Gradient" });
                if (global::esp::Healthbar_Type == 1) {
                    ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::TextMid),
                        "Gradient (top \xE2\x80\x94 mid \xE2\x80\x94 bottom)");
                    ui::tricolor("##hbt", global::esp::color::Healthbar_Top,
                        "##hbm", global::esp::color::Healthbar_Middle,
                        "##hbb", global::esp::color::Healthbar_Bottom);
                }
                ui::sliderint("Bar Gap", &global::esp::gap, 1, 5);
                ui::sliderint("Bar Thickness", &global::esp::Thickness, 1, 5);
            }
            ui::togglecolor("Health Text", &global::esp::Health,
                "##htc", global::esp::color::Health);
            ui::gap(4.f);

            ui::labelsection("LABELS");
            ui::togglecolor("Name", &global::esp::name, "##nc", global::esp::color::name);
            if (global::esp::name)
                ui::combo("Name Format", &global::esp::Name_Type,
                    { "Name","Display Name","Name & Display" });
            ui::togglecolor("Distance", &global::esp::Distance, "##dc", global::esp::color::Distance);
            ui::togglecolor("Rig Type", &global::esp::Rig_Type, "##rc", global::esp::color::Rig_Type);
            ui::togglecolor("Tool", &global::esp::tool, "##tc", global::esp::color::tool);
            ui::gap(4.f);

            ui::labelsection("3D");
            ui::togglecolor("Skeleton", &global::esp::Skeleton, "##skc", global::esp::color::Skeleton);
            ui::togglecolor("Trails", &global::esp::Trails, "##trc", global::esp::color::Trails);
            ui::togglecolor("Chinese Hats", &global::esp::Chinese_Hat, "##chhatc", global::esp::color::hat);
            ui::togglecolor("Aim Lines", &global::esp::aimline, "##aimlc", global::esp::color::aimline);
            ui::toggle("Chams", &global::esp::Chams);
            if (global::esp::Chams) {
                ui::dualcolor("##chc", global::esp::color::Chams,
                    "##choc", global::esp::color::ChamsOutline);
                ui::toggle("Fade", &global::esp::ChamsFade);
                if (global::esp::ChamsFade)
                    ui::sliderint("Fade Speed", &global::esp::ChamsFadeSpeed, 1, 5);
            }
        }
        ui::card::end();

        ImGui::SameLine(0.f, 6.f);

        if (ui::card::begin("##vopt", { halfW, bInH }, "OPTIONS"))
        {
            ui::toggle("Exclude Team", &global::setting::Team_Check);
            ui::toggle("Exclude Client", &global::setting::Client_Check);
            ui::gap(4.f);
            ui::labelsection("RENDERING");
            ui::sliderfloat("Render Distance", &global::esp::Render_Distance, 0.f, 500.f);
        }
        ui::card::end();
    }


    if (Section == 2)
    {
        if (ui::card::begin("##wld", { bInW, bInH }, "WORLD MANIPULATION"))
        {
            ui::toggle("Skybox Changer", &global::world::Skybox);
            if (global::world::Skybox) {
                ui::combo("Preset", &global::world::Skybox_Type, {
                    "Autopsy","Space","Pink Sky","Minecraft","Night Cloudy",
                    "Sparkling Night","Winterness","Dark Crimson","Nebula","Tropical","Green Sky" });
                ui::toggle("Rotation", &global::world::Rotate);
                if (global::world::Rotate)
                    ui::sliderfloat("Rotate Speed", &global::world::Skybox_Rotate_Speed, 0.f, 5.f);
            }
            ui::gap(4.f);

            ui::labelsection("LIGHTING");
            ui::togglecolor("Atmosphere", &global::world::Ambience,
                "##atmc", global::world::color::Ambience);
            ui::toggle("Fog", &global::world::Fog);
            if (global::world::Fog) {
                ImGui::SameLine(ImGui::GetContentRegionMax().x - ui::kColW);
                ui::color4("##fogc", global::world::color::Fog);
                ui::sliderfloat("Fog Distance", &global::world::Fog_Distance, 0.f, 1000.f);
            }
            ui::toggle("Brightness", &global::world::Brightness);
            if (global::world::Brightness)
                ui::sliderfloat("Brightness", &global::world::BrightnessI, 0.f, 10.f);
            ui::toggle("Exposure", &global::world::Exposure);
            if (global::world::Exposure)
                ui::sliderfloat("Exposure", &global::world::ExposureI, -3.f, 3.f);
            ui::gap(4.f);

            ui::labelsection("CAMERA");
            ui::toggle("Custom FOV", &global::world::FOV);
            if (global::world::FOV)
                ui::sliderfloat("FOV", &global::world::FOV_Distance, 70.f, 120.f);
        }
        ui::card::end();
    }


    if (Section == 3)
    {
        if (ui::card::begin("##msc", { halfW, bInH }, "EXPLOITS"))
        {
            ui::toggle("Fly", &global::misc::fly);
            ImGui::SameLine(ImGui::GetContentRegionMax().x - ui::bindwidth(global::misc::Fly_Key, global::misc::Fly_Mode) - 4.f);
            ui::bind("##flyk", &global::misc::Fly_Key, &global::misc::Fly_Mode);
            if (global::misc::fly)
                ui::sliderfloat("Fly Speed", &global::misc::Fly_Speed, 0.f, 200.f);
            ui::gap(8.f);
            ui::toggle("Walkspeed", &global::misc::walkspeed);
            ui::sliderfloat("Walkspeed Speed", &global::misc::Walkspeed_Speed, 1.f, 500.f);
            ui::toggle("Hitbox Expander", &global::misc::hitbox);
            ui::sliderfloat("Hitbox X", &global::misc::Hitbox_Size_X, 1.f, 75.f);
            ui::sliderfloat("Hitbox Y", &global::misc::Hitbox_Size_Y, 1.f, 75.f);
            ui::sliderfloat("Hitbox Z", &global::misc::Hitbox_Size_Z, 1.f, 75.f);
        }
        ui::card::end();

        ImGui::SameLine(0.f, 6.f);

        if (ui::card::begin("##ovr", { halfW, bInH }, "HUD"))
        {
            ui::toggle("Watermark", &global::overlay::watermark);
            ui::toggle("Hotkeys", &global::overlay::hotkey);
            if (global::overlay::hotkey)
            {
                ui::gap(3.f);
                int selectedHotkeys = 0;
                if (global::overlay::Hotkey_Aimbot) ++selectedHotkeys;
                if (global::overlay::Hotkey_Silent) ++selectedHotkeys;
                if (global::overlay::Hotkey_Fly) ++selectedHotkeys;
                if (global::overlay::Hotkey_BladeBallSpam) ++selectedHotkeys;
                if (global::overlay::Hotkey_Walkspeed) ++selectedHotkeys;
                if (global::overlay::Hotkey_HitboxExpander) ++selectedHotkeys;
                char preview[32]{};
                std::snprintf(preview, sizeof(preview), "%d selected", selectedHotkeys);
                ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::TextMid), "Hotkey Entries");
                ImGui::SameLine(ImGui::GetContentRegionMax().x - 150.f);
                ImGui::SetNextItemWidth(146.f);
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::ColorConvertU32ToFloat4(color::SurfaceLift));
                ImGui::PushStyleColor(ImGuiCol_Border, ImGui::ColorConvertU32ToFloat4(color::Border));
                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(color::text));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.f);
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
                if (ImGui::BeginCombo("##hotkey_entries", preview))
                {
                    ImGui::Selectable("Aimbot", &global::overlay::Hotkey_Aimbot, ImGuiSelectableFlags_DontClosePopups);
                    ImGui::Selectable("Silent", &global::overlay::Hotkey_Silent, ImGuiSelectableFlags_DontClosePopups);
                    ImGui::Selectable("Fly", &global::overlay::Hotkey_Fly, ImGuiSelectableFlags_DontClosePopups);
                    ImGui::Selectable("Blade Spam", &global::overlay::Hotkey_BladeBallSpam, ImGuiSelectableFlags_DontClosePopups);
                    ImGui::Selectable("Walkspeed", &global::overlay::Hotkey_Walkspeed, ImGuiSelectableFlags_DontClosePopups);
                    ImGui::Selectable("Hitbox", &global::overlay::Hotkey_HitboxExpander, ImGuiSelectableFlags_DontClosePopups);
                    ImGui::EndCombo();
                }
                ImGui::PopStyleVar(2);
                ImGui::PopStyleColor(3);
            }
            ui::toggle("Radar", &global::overlay::radar);
            ui::toggle("Aim Warning", &global::overlay::AimWarning);
            if (global::overlay::AimWarning || global::esp::aimline)
                ui::sliderfloat("AimView Length", &global::overlay::AimView_MaxLength, 50.f, 1000.f);
            ui::gap(4.f);
            ui::labelsection("HUD COLORS");
            ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::TextMid), "Accent");
            ImGui::SameLine(ImGui::GetContentRegionMax().x - ui::kColW);
            ui::color4("##hudacc", global::overlay::color::Accent);
            ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::TextMid), "Accent 2");
            ImGui::SameLine(ImGui::GetContentRegionMax().x - ui::kColW);
            ui::color4("##hudacc2", global::overlay::color::Accent2);
            ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::TextMid), "Panel");
            ImGui::SameLine(ImGui::GetContentRegionMax().x - ui::kColW);
            ui::color4("##hudpanel", global::overlay::color::panel);
            ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::TextMid), "Text");
            ImGui::SameLine(ImGui::GetContentRegionMax().x - ui::kColW);
            ui::color4("##hudtext", global::overlay::color::text);

            if (global::overlay::radar)
            {
                ui::gap(4.f);
                ui::labelsection("RADAR");
                ui::combo("Shape", &global::overlay::Radar_Shape, { "Circle","Square" });
                ui::toggle("Rotate With Camera", &global::overlay::Radar_Rotate);
                ui::sliderfloat("Zoom", &global::overlay::Radar_Zoom, .25f, 4.f);
                ui::sliderfloat("Size", &global::overlay::Radar_Size, 130.f, 280.f);
            }
        }
        ui::card::end();
    }


    if (Section == 4)
    {
        const bool bladeBallActive = global::GameID == global::ball::PlaceId;
        if (ui::card::begin("##bbmain", { halfW, bInH }, "BLADE BALL"))
        {
            if (!bladeBallActive)
            {
                ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::Accent),
                    "User is not playing Blade Ball.");
                ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::TextDim),
                    "Start Blade Ball to enable these features.");
                ui::gap(8.f);
                ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::TextDim),
                    "Features locked");
                ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::TextDim),
                    "Required place: 13772394625");
            }
            else
            {
                ui::togglecolor("Ball ESP", &global::ball::BallEsp,
                    "##bbespcol", global::ball::BallEspColor);
                ui::togglecolor("Parry Range", &global::ball::DrawParryRange,
                    "##bbrangecol", global::ball::ParryRangeColor);
                ui::gap(4.f);
                ui::toggle("Auto Parry", &global::ball::AutoParry);
                ui::toggle("Clash Mode", &global::ball::ClashMode);
                ui::toggle("Press F", &global::ball::pressf);
                ui::toggle("Predict Timing", &global::ball::PredictTiming);
                ui::toggle("Auto Range", &global::ball::AutoRange);
                ui::toggle("Auto Timing", &global::ball::AutoTiming);
                ui::toggle("Dynamic Timing", &global::ball::DynamicTiming);
                ui::sliderfloat("Parry Distance", &global::ball::ParryDistance, 5.f, 120.f);
                ui::sliderfloat("Distance Buffer", &global::ball::DistanceBuffer, 0.f, 100.f);
                ui::sliderfloat("Timing MS", &global::ball::ParryTimingMs, 35.f, 260.f);
                ui::sliderfloat("Cooldown MS", &global::ball::ParryCooldownMs, 20.f, 220.f);
                ui::sliderfloat("Min Speed", &global::ball::MinBallSpeed, 1.f, 80.f);
            }
        }
        ui::card::end();

        ImGui::SameLine(0.f, 6.f);

        if (ui::card::begin("##bbinfo", { halfW, bInH }, "OPTIONS"))
        {
            if (!bladeBallActive)
            {
                ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::TextDim),
                    "Blade Ball options are disabled.");
                ui::gap(4.f);
                ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::TextDim),
                    "Join Blade Ball and the controls will unlock.");
            }
            else
            {
                ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::TextDim),
                    "Ball ESP draws each detected ball.");
                ui::gap(4.f);
                ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::TextDim),
                    "Auto parry triggers when a targeted ball enters range.");
                ui::gap(8.f);
                ui::toggle("Spam Parry", &global::ball::SpamParry);
                ImGui::SameLine(ImGui::GetContentRegionMax().x - ui::bindwidth(global::ball::SpamParry_Key, global::ball::SpamParry_Mode) - 4.f);
                ui::bind("##bbspamk", &global::ball::SpamParry_Key, &global::ball::SpamParry_Mode);
            }
        }
        ui::card::end();
    }

    if (Section == 5)
    {
        static char           cfgName[128] = "";
        static char           cfgImport[131072] = "";
        static std::string    cfgNotice;
        static float          cfgNoticeUntil = 0.f;
        static std::vector<std::string> cfgList;
        static bool           cfgRefreshed = false;
        if (!cfgRefreshed) { config::refresh(cfgList); cfgRefreshed = true; }

        if (ui::card::begin("##cfg", { halfW, bInH }, "CONFIGS"))
        {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::ColorConvertU32ToFloat4(color::Win));
            ImGui::PushStyleColor(ImGuiCol_Border, ImGui::ColorConvertU32ToFloat4(color::Border));
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 3.f);
            ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.f);
            ImGui::BeginChild("##cl", { ImGui::GetContentRegionAvail().x, 108.f }, true);
            for (auto& cfg : cfgList)
            {
                const bool sel = (strcmp(cfgName, cfg.c_str()) == 0);
                ImGui::PushStyleColor(ImGuiCol_Header,
                    ImGui::ColorConvertU32ToFloat4(color::AccentDim));
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered,
                    ImGui::ColorConvertU32ToFloat4(color::AccentDim));
                ImGui::PushStyleColor(ImGuiCol_HeaderActive,
                    ImGui::ColorConvertU32ToFloat4(color::Accent));
                ImGui::PushStyleColor(ImGuiCol_Text,
                    ImGui::ColorConvertU32ToFloat4(sel ? color::AccentFg : color::TextMid));
                if (ImGui::Selectable(cfg.c_str(), sel))
                    strcpy_s(cfgName, sizeof(cfgName), cfg.c_str());
                ImGui::PopStyleColor(4);
            }
            ImGui::EndChild();
            ImGui::PopStyleVar(2);
            ImGui::PopStyleColor(2);

            ui::gap(4.f);

            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::ColorConvertU32ToFloat4(color::Win));
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImGui::ColorConvertU32ToFloat4(color::CardHov));
            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImGui::ColorConvertU32ToFloat4(color::CardHov));
            ImGui::PushStyleColor(ImGuiCol_Border, ImGui::ColorConvertU32ToFloat4(color::Border));
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(color::text));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.f);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##cn", cfgName, sizeof(cfgName));
            ImGui::PopStyleVar(2);
            ImGui::PopStyleColor(5);

            ui::gap(4.f);

            const float bw = (ImGui::GetContentRegionAvail().x - 8.f) / 3.f;
            if (ui::button("Load", color::card, color::Border, color::text, bw)) {
                if (cfgName[0]) config::load(cfgName);
            }
            ImGui::SameLine(0.f, 4.f);
            if (ui::button("Save", color::AccentDim, color::Accent, color::AccentFg, bw)) {
                if (cfgName[0]) { config::save(cfgName); config::refresh(cfgList); }
            }
            ImGui::SameLine(0.f, 4.f);
            if (ui::button("Delete", color::DangerDim, color::DangerBrd, color::Danger, bw)) {
                if (cfgName[0]) {
                    config::remove(cfgName);
                    config::refresh(cfgList);
                    cfgName[0] = '\0';
                }
            }

            ui::gap(8.f);
            ui::labelsection("SHARE");

            const float shareW = (ImGui::GetContentRegionAvail().x - 4.f) / 2.f;
            if (ui::button("Export", color::AccentDim, color::Accent, color::AccentFg, shareW))
            {
                if (cfgName[0])
                {
                    config::save(cfgName);
                    config::refresh(cfgList);

                    std::string encoded;
                    if (config::export64(cfgName, encoded))
                    {
                        strncpy_s(cfgImport, sizeof(cfgImport), encoded.c_str(), _TRUNCATE);
                        cfgNotice = copytext(encoded)
                            ? "Config copied to clipboard"
                            : "Config exported, clipboard failed";
                        cfgNoticeUntil = (float)ImGui::GetTime() + 2.8f;
                    }
                    else
                    {
                        cfgNotice = "Config export failed";
                        cfgNoticeUntil = (float)ImGui::GetTime() + 2.8f;
                    }
                }
                else
                {
                    cfgNotice = "Enter config name first";
                    cfgNoticeUntil = (float)ImGui::GetTime() + 2.8f;
                }
            }
            ImGui::SameLine(0.f, 4.f);
            if (ui::button("Import", color::card, color::Border, color::text, shareW))
            {
                ImGui::OpenPopup("##import_config_popup");
            }

            if (!cfgNotice.empty() && ImGui::GetTime() < cfgNoticeUntil)
            {
                ui::gap(4.f);
                ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::Accent), "%s", cfgNotice.c_str());
            }

            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowSize(ImVec2(460.f, 318.f), ImGuiCond_Appearing);
            ImGui::SetNextWindowPos(viewport->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            ImGui::PushStyleColor(ImGuiCol_PopupBg, ImGui::ColorConvertU32ToFloat4(color::Surface));
            ImGui::PushStyleColor(ImGuiCol_Border, ImGui::ColorConvertU32ToFloat4(color::Border));
            ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(2.f / 255.f, 7.f / 255.f, 11.f / 255.f, 0.78f));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6.f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(14.f, 14.f));
            if (ImGui::BeginPopupModal("##import_config_popup", nullptr,
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings))
            {
                ImDrawList* modalDraw = ImGui::GetWindowDrawList();
                ImVec2 modalPos = ImGui::GetWindowPos();
                ImVec2 modalSize = ImGui::GetWindowSize();
                modalDraw->AddRectFilled(modalPos, modalPos + modalSize, IM_COL32(3, 6, 10, 238), 6.f);
                modalDraw->AddRectFilled(modalPos, modalPos + ImVec2(modalSize.x, 44.f), color::card, 6.f, ImDrawFlags_RoundCornersTop);
                modalDraw->AddRectFilled(modalPos + ImVec2(0.f, 43.f), modalPos + ImVec2(modalSize.x, 44.f), color::Border);
                modalDraw->AddRectFilled(modalPos + ImVec2(14.f, 15.f), modalPos + ImVec2(17.f, 29.f), color::Accent, 2.f);
                ImGui::SetCursorPos(ImVec2(25.f, 13.f));
                ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::text), "IMPORT CONFIG");
                ImGui::SetCursorPos(ImVec2(14.f, 58.f));
                ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::TextMid), "Paste config base64 here");
                ui::gap(8.f);

                const float popupWidth = ImGui::GetContentRegionAvail().x;
                if (ui::button("Paste", color::card, color::Border, color::text, 96.f))
                {
                    const char* clip = ImGui::GetClipboardText();
                    if (clip)
                        strncpy_s(cfgImport, sizeof(cfgImport), clip, _TRUNCATE);
                }

                ui::gap(8.f);
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::ColorConvertU32ToFloat4(color::Win));
                ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImGui::ColorConvertU32ToFloat4(color::CardHov));
                ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImGui::ColorConvertU32ToFloat4(color::CardHov));
                ImGui::PushStyleColor(ImGuiCol_Border, ImGui::ColorConvertU32ToFloat4(color::Border));
                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(color::text));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.f);
                ImGui::InputTextMultiline("##cfgimport", cfgImport, sizeof(cfgImport),
                    ImVec2(popupWidth, 116.f));
                ImGui::PopStyleVar(2);
                ImGui::PopStyleColor(5);

                ui::gap(10.f);
                const float modalW = (ImGui::GetContentRegionAvail().x - 6.f) / 2.f;
                if (ui::button("Import Base64", color::AccentDim, color::Accent, color::AccentFg, modalW))
                {
                    if (cfgName[0] && cfgImport[0] && config::import64(cfgName, cfgImport))
                    {
                        config::refresh(cfgList);
                        config::load(cfgName);
                        cfgNotice = "Config imported";
                        ImGui::CloseCurrentPopup();
                    }
                    else
                    {
                        cfgNotice = cfgName[0] ? "Config import failed" : "Enter config name first";
                    }
                    cfgNoticeUntil = (float)ImGui::GetTime() + 2.8f;
                }
                ImGui::SameLine(0.f, 6.f);
                if (ui::button("Cancel", color::card, color::Border, color::text, modalW))
                    ImGui::CloseCurrentPopup();

                ImGui::EndPopup();
            }
            ImGui::PopStyleVar(3);
            ImGui::PopStyleColor(3);
        }
        ui::card::end();

        ImGui::SameLine(0.f, 6.f);

        if (ui::card::begin("##gen", { halfW, bInH }, "GENERAL"))
        {
            ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::TextMid), "Menu Key");
            ImGui::SameLine(ImGui::GetContentRegionMax().x - ui::keywidth(global::setting::Menu_Key) - 4.f);
            ui::keyselect("##menukey", &global::setting::Menu_Key);
            ui::gap(4.f);
            ui::toggle("Compact UI", &global::setting::Compact_UI);
            ui::gap(2.f);
            ui::toggle("Streamproof", &global::setting::Streamproof);
            ui::gap(2.f);
            ui::combo("Frame Pace", &global::setting::Performance_Mode, { "Eco VSync","VSync","Unlocked" });
            ui::gap(4.f);
            ui::labelsection("MENU COLORS");
            ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::TextMid), "Accent");
            ImGui::SameLine(ImGui::GetContentRegionMax().x - ui::kColW);
            ui::color4("##thacc", global::setting::color::Accent);
            ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::TextMid), "Accent 2");
            ImGui::SameLine(ImGui::GetContentRegionMax().x - ui::kColW);
            ui::color4("##thacc2", global::setting::color::Accent2);
            ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::TextMid), "Window");
            ImGui::SameLine(ImGui::GetContentRegionMax().x - ui::kColW);
            ui::color4("##thwin", global::setting::color::Window);
            ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::TextMid), "Cards");
            ImGui::SameLine(ImGui::GetContentRegionMax().x - ui::kColW);
            ui::color4("##thcard", global::setting::color::card);
            ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(color::TextMid), "Text");
            ImGui::SameLine(ImGui::GetContentRegionMax().x - ui::kColW);
            ui::color4("##thtext", global::setting::color::text);

            const float remaining = ImGui::GetContentRegionAvail().y - 34.f;
            if (remaining > 0.f) ImGui::Dummy({ 0.f, remaining });

            if (ui::button("  UNLOAD  ", color::DangerDim, color::DangerBrd, color::Danger,
                ImGui::GetContentRegionAvail().x, 26.f))
                ExitProcess(0);
        }
        ui::card::end();
    }

    ImGui::EndGroup();
    ImGui::PopStyleVar();
    ImGui::PopClipRect();
    ImGui::End();
}

void graphic::visual()
{
    color::refresh();
    cursor();
    ball::render();
    esp::run();
    hud::render(Running);
    welcome(Running);
}
