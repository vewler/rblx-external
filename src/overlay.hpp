#pragma once
#include <Windows.h>
#include <dwmapi.h>
#include <d3d11.h>
#include "globals.hpp"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dwmapi.lib")

namespace Overlay {

    inline ID3D11Device*           g_pd3dDevice = nullptr;
    inline ID3D11DeviceContext*    g_pd3dCtx    = nullptr;
    inline IDXGISwapChain*         g_pSwapChain = nullptr;
    inline ID3D11RenderTargetView* g_pRTV       = nullptr;

    inline LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        if (msg == WM_DESTROY) { PostQuitMessage(0); return 0; }
        return DefWindowProcW(hWnd, msg, wParam, lParam);
    }

    inline bool CreateOverlayWindow(HINSTANCE hInst) {
        WNDCLASSEXW wc{};
        wc.cbSize        = sizeof(wc);
        wc.style         = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc   = WndProc;
        wc.hInstance      = hInst;
        wc.lpszClassName = L"NullOverlay";
        RegisterClassExW(&wc);

        globals::overlayWindow = CreateWindowExW(
            WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_NOACTIVATE,
            wc.lpszClassName, L"", WS_POPUP,
            0, 0, globals::screenW, globals::screenH,
            nullptr, nullptr, hInst, nullptr
        );
        if (!globals::overlayWindow) return false;

        SetLayeredWindowAttributes(globals::overlayWindow, RGB(0, 0, 0), 255, LWA_ALPHA);
        MARGINS margin = { -1, -1, -1, -1 };
        DwmExtendFrameIntoClientArea(globals::overlayWindow, &margin);
        ShowWindow(globals::overlayWindow, SW_SHOWDEFAULT);
        UpdateWindow(globals::overlayWindow);
        return true;
    }

    inline bool CreateDeviceD3D() {
        DXGI_SWAP_CHAIN_DESC sd{};
        sd.BufferCount        = 2;
        sd.BufferDesc.Width   = globals::screenW;
        sd.BufferDesc.Height  = globals::screenH;
        sd.BufferDesc.Format  = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate = { 0, 1 };
        sd.Flags              = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sd.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow       = globals::overlayWindow;
        sd.SampleDesc.Count   = 1;
        sd.Windowed           = TRUE;
        sd.SwapEffect         = DXGI_SWAP_EFFECT_DISCARD;

        D3D_FEATURE_LEVEL fl;
        D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
            levels, 2, D3D11_SDK_VERSION,
            &sd, &g_pSwapChain, &g_pd3dDevice, &fl, &g_pd3dCtx
        );
        if (FAILED(hr)) return false;

        ID3D11Texture2D* backBuf = nullptr;
        g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuf));
        g_pd3dDevice->CreateRenderTargetView(backBuf, nullptr, &g_pRTV);
        backBuf->Release();
        return true;
    }

    inline void CleanupDeviceD3D() {
        if (g_pRTV)       { g_pRTV->Release();       g_pRTV       = nullptr; }
        if (g_pSwapChain) { g_pSwapChain->Release();  g_pSwapChain = nullptr; }
        if (g_pd3dCtx)    { g_pd3dCtx->Release();     g_pd3dCtx    = nullptr; }
        if (g_pd3dDevice) { g_pd3dDevice->Release();   g_pd3dDevice = nullptr; }
    }

    inline void SyncToGameWindow() {
        globals::gameWindow = FindWindowA(nullptr, "Roblox");
        if (!globals::gameWindow) return;
        RECT rc;
        GetWindowRect(globals::gameWindow, &rc);
        globals::screenW = rc.right - rc.left;
        globals::screenH = rc.bottom - rc.top;
        MoveWindow(globals::overlayWindow, rc.left, rc.top, globals::screenW, globals::screenH, TRUE);
    }
}
