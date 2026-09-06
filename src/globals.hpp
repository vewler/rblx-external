#pragma once
#include <Windows.h>
#include <atomic>
#include <mutex>
#include <vector>
#include <string>
#include "math.hpp"

struct PlayerData {
    std::string name;
    Vec3 rootPos;
    Vec3 headPos;
    float health      = 100.f;
    float maxHealth    = 100.f;
    uintptr_t humanoid = 0;
    bool valid         = false;
};

namespace globals {
    inline std::atomic<bool> running{ true };
    inline std::atomic<bool> menuOpen{ false };

    inline HWND gameWindow    = nullptr;
    inline HWND overlayWindow = nullptr;
    inline int  screenW       = 1920;
    inline int  screenH       = 1080;

    inline uintptr_t dataModel      = 0;
    inline uintptr_t visualEngine   = 0;
    inline uintptr_t playersService = 0;
    inline uintptr_t localPlayer    = 0;

    inline ViewMatrix viewMatrix{};
    inline Vec3 localRootPos{};

    inline std::mutex playerMtx;
    inline std::vector<PlayerData> players;

    namespace settings {
        inline bool espEnabled      = true;
        inline bool espBoxes        = true;
        inline bool espNames        = true;
        inline bool espHealth       = true;
        inline bool espSnaplines    = false;
        inline bool espDistance     = true;
        inline float espBoxColor[4] = { 0.7f, 0.4f, 1.0f, 1.0f };

        inline bool aimbotEnabled   = false;
        inline float aimbotFov      = 120.f;
        inline float aimbotSmooth   = 8.0f;
        inline int   aimbotKey      = VK_RBUTTON;
        inline bool  aimbotSticky   = true;

        inline bool  speedEnabled   = false;
        inline float speedValue     = 16.f;
        inline bool  jumpEnabled    = false;
        inline float jumpValue      = 50.f;

        inline bool fovCircle       = true;
    }
}
