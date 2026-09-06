#pragma once
#include "globals.hpp"
#include "math.hpp"
#include <imgui.h>
#include <string>
#include <algorithm>

namespace ESP {

    inline ImColor HealthToColor(float ratio) {
        ratio = std::clamp(ratio, 0.f, 1.f);
        return ImColor(1.0f - ratio, ratio, 0.15f, 1.0f);
    }

    inline void DrawTextWithShadow(ImDrawList* dl, const Vec2& pos, const char* text, ImU32 color) {
        dl->AddText(ImVec2(pos.x + 1.f, pos.y + 1.f), IM_COL32(0, 0, 0, 180), text);
        dl->AddText(ImVec2(pos.x, pos.y), color, text);
    }

    inline void Render() {
        if (!globals::settings::espEnabled) return;
        auto* dl = ImGui::GetBackgroundDrawList();
        float sw = static_cast<float>(globals::screenW);
        float sh = static_cast<float>(globals::screenH);

        std::lock_guard<std::mutex> lock(globals::playerMtx);
        for (const auto& p : globals::players) {
            if (!p.valid) continue;

            Vec2 screenHead, screenFeet;
            Vec3 feetPos = { p.rootPos.x, p.rootPos.y - 3.0f, p.rootPos.z };
            Vec3 topPos  = { p.headPos.x, p.headPos.y + 0.6f, p.headPos.z };

            if (!globals::viewMatrix.WorldToScreen(topPos, screenHead, sw, sh)) continue;
            if (!globals::viewMatrix.WorldToScreen(feetPos, screenFeet, sw, sh)) continue;

            float boxH = screenFeet.y - screenHead.y;
            if (boxH < 4.f) continue;
            float boxW = boxH * 0.45f;
            float left = screenHead.x - boxW * 0.5f, right = screenHead.x + boxW * 0.5f;
            float top = screenHead.y, bottom = screenFeet.y;

            ImU32 boxCol = ImGui::ColorConvertFloat4ToU32(ImVec4(
                globals::settings::espBoxColor[0], globals::settings::espBoxColor[1],
                globals::settings::espBoxColor[2], globals::settings::espBoxColor[3]));

            if (globals::settings::espBoxes) {
                dl->AddRect(ImVec2(left, top), ImVec2(right, bottom), boxCol, 0.f, 0, 1.0f);
                dl->AddRect(ImVec2(left-1, top-1), ImVec2(right+1, bottom+1), IM_COL32(0,0,0,100), 0.f, 0, 1.0f);
            }

            if (globals::settings::espNames) {
                ImVec2 ts = ImGui::CalcTextSize(p.name.c_str());
                DrawTextWithShadow(dl, { screenHead.x - ts.x * 0.5f, top - ts.y - 3.f },
                    p.name.c_str(), IM_COL32(255,255,255,255));
            }

            if (globals::settings::espHealth && p.maxHealth > 0.f) {
                float ratio = std::clamp(p.health / p.maxHealth, 0.f, 1.f);
                float barX = left - 5.f;
                dl->AddRectFilled(ImVec2(barX-1, top-1), ImVec2(barX+2, bottom+1), IM_COL32(0,0,0,160));
                dl->AddRectFilled(ImVec2(barX, bottom - boxH*ratio), ImVec2(barX+1, bottom), HealthToColor(ratio));
            }

            if (globals::settings::espSnaplines)
                dl->AddLine(ImVec2(sw*0.5f, sh), ImVec2(screenFeet.x, screenFeet.y), IM_COL32(255,255,255,200), 1.0f);

            if (globals::settings::espDistance) {
                float dist = globals::localRootPos.DistTo(p.rootPos);
                char buf[32]; snprintf(buf, sizeof(buf), "[%.0fm]", dist);
                ImVec2 ds = ImGui::CalcTextSize(buf);
                DrawTextWithShadow(dl, { screenFeet.x - ds.x*0.5f, bottom + 2.f }, buf, IM_COL32(200,200,200,220));
            }
        }
    }

    inline void DrawFOVCircle() {
        if (!globals::settings::fovCircle || !globals::settings::aimbotEnabled) return;
        POINT cur; GetCursorPos(&cur); ScreenToClient(globals::overlayWindow, &cur);
        ImGui::GetBackgroundDrawList()->AddCircle(
            ImVec2((float)cur.x, (float)cur.y), globals::settings::aimbotFov, IM_COL32(255,255,255,120), 64, 1.0f);
    }
}
