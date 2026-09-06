#pragma once
#include "globals.hpp"
#include "math.hpp"
#include <Windows.h>

namespace Aimbot {

    inline uintptr_t stickyTarget = 0;

    inline void SendSmoothInput(float dx, float dy, float smooth) {
        if (smooth < 1.f) smooth = 1.f;
        dx /= smooth; dy /= smooth;
        INPUT input{};
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_MOVE;
        input.mi.dx = static_cast<LONG>(dx);
        input.mi.dy = static_cast<LONG>(dy);
        SendInput(1, &input, sizeof(INPUT));
    }

    inline void Run() {
        if (!globals::settings::aimbotEnabled) return;
        bool keyDown = (GetAsyncKeyState(globals::settings::aimbotKey) & 0x8000) != 0;
        if (!keyDown) { stickyTarget = 0; return; }

        POINT cur; GetCursorPos(&cur); ScreenToClient(globals::gameWindow, &cur);
        Vec2 mouse((float)cur.x, (float)cur.y);
        float sw = (float)globals::screenW, sh = (float)globals::screenH;

        float bestDist = globals::settings::aimbotFov;
        Vec2 bestScreen{}; uintptr_t bestHumanoid = 0; bool found = false;

        std::lock_guard<std::mutex> lock(globals::playerMtx);
        for (const auto& p : globals::players) {
            if (!p.valid || !p.humanoid) continue;

            if (globals::settings::aimbotSticky && stickyTarget != 0) {
                if (p.humanoid == stickyTarget) {
                    Vec2 sh2;
                    if (globals::viewMatrix.WorldToScreen(p.headPos, sh2, sw, sh))
                        SendSmoothInput(sh2.x - mouse.x, sh2.y - mouse.y, globals::settings::aimbotSmooth);
                    return;
                }
                continue;
            }

            Vec2 screenHead;
            if (!globals::viewMatrix.WorldToScreen(p.headPos, screenHead, sw, sh)) continue;
            float dist = mouse.DistTo(screenHead);
            if (dist < bestDist) { bestDist = dist; bestScreen = screenHead; bestHumanoid = p.humanoid; found = true; }
        }

        if (found) {
            stickyTarget = bestHumanoid;
            SendSmoothInput(bestScreen.x - mouse.x, bestScreen.y - mouse.y, globals::settings::aimbotSmooth);
        }
    }
}
