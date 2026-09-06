#pragma once
#include "driver.hpp"
#include "globals.hpp"
#include "scanner.hpp"
#include "../Offsets/offsets.hpp"

namespace Movement {

    inline uintptr_t GetLocalHumanoid() {
        if (!globals::localPlayer) return 0;
        uintptr_t character = g_driver.Read<uintptr_t>(globals::localPlayer + Offsets::Player::ModelInstance);
        if (!character) return 0;
        return Scanner::FindFirstChildOfClass(character, "Humanoid");
    }

    inline void SetWalkSpeed(float value) {
        uintptr_t h = GetLocalHumanoid();
        if (!h) return;
        g_driver.Write<float>(h + Offsets::Humanoid::Walkspeed, value);
        g_driver.Write<float>(h + Offsets::Humanoid::WalkspeedCheck, value);
    }

    inline void SetJumpPower(float value) {
        uintptr_t h = GetLocalHumanoid();
        if (!h) return;
        g_driver.Write<float>(h + Offsets::Humanoid::JumpPower, value);
    }

    inline void Tick() {
        if (globals::settings::speedEnabled) SetWalkSpeed(globals::settings::speedValue);
        if (globals::settings::jumpEnabled)  SetJumpPower(globals::settings::jumpValue);
    }
}
