#include <Windows.h>
#include <thread>
#include <chrono>
#include <atomic>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#include "driver.hpp"
#include "globals.hpp"
#include "overlay.hpp"
#include "scanner.hpp"
#include "esp.hpp"
#include "aimbot.hpp"
#include "movement.hpp"
#include "menu.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static WNDPROC origWndProc = nullptr;
static LRESULT CALLBACK HookWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    if (globals::menuOpen.load())
        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wp, lp)) return 1;
    return CallWindowProcW(origWndProc, hWnd, msg, wp, lp);
}

static DWORD FindRobloxPid() {
    HWND hwnd = FindWindowA(nullptr, "Roblox");
    if (!hwnd) return 0;
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    return pid;
}

static void BackgroundScanThread() {
    while (globals::running.load()) {
        if (!globals::playersService) Scanner::ScanDataModel();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

static void CacheThread() {
    while (globals::running.load()) {
        Scanner::UpdateViewMatrix();
        Scanner::UpdateLocalPosition();
        Scanner::CachePlayers();
        Movement::Tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int) {
    DWORD pid = 0;
    while (!pid) { pid = FindRobloxPid(); if (!pid) Sleep(500); }

    if (!g_driver.Initialize()) {
        MessageBoxA(nullptr, "Failed to connect to NullKD driver.", "Error", MB_ICONERROR);
        return 1;
    }
    if (!g_driver.AttachToProcess(pid)) {
        MessageBoxA(nullptr, "Failed to attach to Roblox process.", "Error", MB_ICONERROR);
        g_driver.Shutdown(); return 1;
    }

    globals::gameWindow = FindWindowA(nullptr, "Roblox");
    RECT rc; GetWindowRect(globals::gameWindow, &rc);
    globals::screenW = rc.right - rc.left;
    globals::screenH = rc.bottom - rc.top;

    if (!Overlay::CreateOverlayWindow(hInst)) {
        MessageBoxA(nullptr, "Failed to create overlay.", "Error", MB_ICONERROR);
        g_driver.Shutdown(); return 1;
    }
    if (!Overlay::CreateDeviceD3D()) {
        MessageBoxA(nullptr, "Failed to create D3D11 device.", "Error", MB_ICONERROR);
        g_driver.Shutdown(); return 1;
    }

    origWndProc = reinterpret_cast<WNDPROC>(
        SetWindowLongPtrW(globals::overlayWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HookWndProc)));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    Menu::ApplyNullSyntaxTheme();
    ImGui_ImplWin32_Init(globals::overlayWindow);
    ImGui_ImplDX11_Init(Overlay::g_pd3dDevice, Overlay::g_pd3dCtx);

    std::thread bgThread(BackgroundScanThread);
    std::thread cacheThread(CacheThread);

    MSG msg{};
    while (globals::running.load()) {
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg); DispatchMessageW(&msg);
            if (msg.message == WM_QUIT) globals::running.store(false);
        }
        if (!globals::running.load()) break;

        static bool insertPrev = false;
        bool insertNow = (GetAsyncKeyState(VK_INSERT) & 0x8000) != 0;
        if (insertNow && !insertPrev) {
            globals::menuOpen.store(!globals::menuOpen.load());
            LONG exStyle = GetWindowLongW(globals::overlayWindow, GWL_EXSTYLE);
            if (globals::menuOpen.load()) {
                exStyle &= ~WS_EX_TRANSPARENT;
                SetWindowLongW(globals::overlayWindow, GWL_EXSTYLE, exStyle);
                SetForegroundWindow(globals::overlayWindow);
            } else {
                exStyle |= WS_EX_TRANSPARENT;
                SetWindowLongW(globals::overlayWindow, GWL_EXSTYLE, exStyle);
            }
        }
        insertPrev = insertNow;

        if (GetAsyncKeyState(VK_END) & 0x8000) { globals::running.store(false); break; }

        Overlay::SyncToGameWindow();
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ESP::Render();
        ESP::DrawFOVCircle();
        Aimbot::Run();
        Menu::Render();

        ImGui::Render();
        float clear[4] = { 0.f, 0.f, 0.f, 0.f };
        Overlay::g_pd3dCtx->OMSetRenderTargets(1, &Overlay::g_pRTV, nullptr);
        Overlay::g_pd3dCtx->ClearRenderTargetView(Overlay::g_pRTV, clear);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        Overlay::g_pSwapChain->Present(1, 0);
    }

    globals::running.store(false);
    if (bgThread.joinable()) bgThread.join();
    if (cacheThread.joinable()) cacheThread.join();

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    Overlay::CleanupDeviceD3D();
    DestroyWindow(globals::overlayWindow);
    g_driver.Shutdown();
    return 0;
}
