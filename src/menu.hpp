#pragma once
#include <imgui.h>
#include "globals.hpp"
#include <algorithm>

namespace Menu {

    inline int currentTab = 0;

    inline void ApplyNullSyntaxTheme() {
        ImGuiStyle& s = ImGui::GetStyle();
        s.WindowRounding = 4.f; s.FrameRounding = 2.f; s.GrabRounding = 12.f;
        s.ScrollbarRounding = 2.f; s.WindowPadding = ImVec2(16,16);
        s.FramePadding = ImVec2(8,4); s.ItemSpacing = ImVec2(8,8);
        s.ScrollbarSize = 10.f; s.GrabMinSize = 10.f;
        s.WindowBorderSize = 0.f; s.FrameBorderSize = 0.f;

        ImVec4* c = s.Colors;
        ImVec4 bg(0.10f,0.10f,0.10f,1.f), bgD(0.07f,0.07f,0.07f,1.f), bgL(0.14f,0.14f,0.14f,1.f);
        ImVec4 acc(0.70f,0.40f,1.00f,1.f), accH(0.80f,0.55f,1.00f,1.f);

        c[ImGuiCol_WindowBg]=bg; c[ImGuiCol_ChildBg]=bgD; c[ImGuiCol_PopupBg]=bg;
        c[ImGuiCol_Border]=ImVec4(0.20f,0.20f,0.20f,0.50f);
        c[ImGuiCol_FrameBg]=bgL; c[ImGuiCol_FrameBgHovered]=ImVec4(0.18f,0.18f,0.18f,1.f);
        c[ImGuiCol_FrameBgActive]=ImVec4(0.22f,0.22f,0.22f,1.f);
        c[ImGuiCol_TitleBg]=bgD; c[ImGuiCol_TitleBgActive]=bgD; c[ImGuiCol_TitleBgCollapsed]=bgD;
        c[ImGuiCol_ScrollbarBg]=bgD; c[ImGuiCol_ScrollbarGrab]=bgL;
        c[ImGuiCol_ScrollbarGrabHovered]=acc; c[ImGuiCol_ScrollbarGrabActive]=accH;
        c[ImGuiCol_CheckMark]=acc; c[ImGuiCol_SliderGrab]=acc; c[ImGuiCol_SliderGrabActive]=accH;
        c[ImGuiCol_Button]=bgL; c[ImGuiCol_ButtonHovered]=ImVec4(acc.x,acc.y,acc.z,0.40f);
        c[ImGuiCol_ButtonActive]=acc;
        c[ImGuiCol_Header]=bgL; c[ImGuiCol_HeaderHovered]=ImVec4(acc.x,acc.y,acc.z,0.30f);
        c[ImGuiCol_HeaderActive]=acc;
        c[ImGuiCol_Separator]=ImVec4(0.20f,0.20f,0.20f,1.f);
        c[ImGuiCol_Tab]=bgD; c[ImGuiCol_TabHovered]=ImVec4(acc.x,acc.y,acc.z,0.40f);
        c[ImGuiCol_TabActive]=acc;
        c[ImGuiCol_Text]=ImVec4(1,1,1,1); c[ImGuiCol_TextDisabled]=ImVec4(0.6f,0.6f,0.6f,1.f);
    }

    inline bool CustomCheckbox(const char* label, bool* v) {
        ImGui::PushID(label);
        ImDrawList* dl = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetCursorScreenPos();
        float sq = 14.0f;
        ImVec2 ls = ImGui::CalcTextSize(label);
        float totalH = (std::max)(sq, ls.y);

        bool pressed = ImGui::InvisibleButton("##cb", ImVec2(sq + 6.f + ls.x, totalH));
        if (pressed) *v = !*v;
        bool hovered = ImGui::IsItemHovered();

        float boxY = pos.y + (totalH - sq) * 0.5f;
        ImVec2 bMin(pos.x, boxY), bMax(pos.x + sq, boxY + sq);
        dl->AddRectFilled(bMin, bMax, hovered ? IM_COL32(56,56,56,255) : IM_COL32(36,36,36,255), 2.f);
        if (*v) dl->AddRectFilled(ImVec2(bMin.x+2,bMin.y+2), ImVec2(bMax.x-2,bMax.y-2), IM_COL32(178,102,255,255), 1.f);
        dl->AddText(ImVec2(pos.x+sq+6.f, pos.y+(totalH-ls.y)*0.5f), IM_COL32(255,255,255,255), label);
        ImGui::PopID();
        return pressed;
    }

    inline bool CustomSlider(const char* label, float* v, float vMin, float vMax, const char* fmt = "%.1f") {
        ImGui::Text("%s", label);
        ImGui::PushID(label);
        ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.70f,0.40f,1.00f,1.f));
        ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.80f,0.55f,1.00f,1.f));
        float w = ImGui::CalcItemWidth() - 50.f;
        ImGui::SetNextItemWidth(w);
        bool changed = ImGui::SliderFloat("##sl", v, vMin, vMax, fmt);
        ImGui::SameLine();
        char buf[32]; snprintf(buf, sizeof(buf), fmt, *v);
        ImGui::Text("%s", buf);
        ImGui::PopStyleColor(2);
        ImGui::PopID();
        return changed;
    }

    inline void TabButton(const char* label, int idx) {
        ImVec4 col = (currentTab==idx) ? ImVec4(0.70f,0.40f,1.00f,1.f) : ImVec4(0.14f,0.14f,0.14f,1.f);
        ImVec4 colH = (currentTab==idx) ? ImVec4(0.80f,0.55f,1.00f,1.f) : ImVec4(0.20f,0.20f,0.20f,1.f);
        ImGui::PushStyleColor(ImGuiCol_Button, col);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colH);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.80f,0.55f,1.00f,1.f));
        if (ImGui::Button(label, ImVec2(90,30))) currentTab = idx;
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
    }

    inline void Render() {
        if (!globals::menuOpen.load()) return;
        ImGui::SetNextWindowSize(ImVec2(480,380), ImGuiCond_Once);
        ImGui::Begin("nullsyntax", nullptr, ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoResize);

        TabButton("Aim",0); TabButton("Visuals",1); TabButton("Misc",2); TabButton("Settings",3);
        ImGui::NewLine(); ImGui::Separator(); ImGui::Spacing();

        if (currentTab==0) {
            CustomCheckbox("Enable Aimbot", &globals::settings::aimbotEnabled);
            CustomCheckbox("Sticky Aim", &globals::settings::aimbotSticky);
            CustomSlider("FOV Radius", &globals::settings::aimbotFov, 10.f, 500.f);
            CustomSlider("Smoothing", &globals::settings::aimbotSmooth, 1.f, 30.f);
            CustomCheckbox("FOV Circle", &globals::settings::fovCircle);
        }
        if (currentTab==1) {
            CustomCheckbox("Enable ESP", &globals::settings::espEnabled);
            CustomCheckbox("Boxes", &globals::settings::espBoxes);
            CustomCheckbox("Names", &globals::settings::espNames);
            CustomCheckbox("Health Bars", &globals::settings::espHealth);
            CustomCheckbox("Snaplines", &globals::settings::espSnaplines);
            CustomCheckbox("Distance", &globals::settings::espDistance);
            ImGui::Spacing();
            ImGui::ColorEdit4("Box Color", globals::settings::espBoxColor, ImGuiColorEditFlags_NoInputs|ImGuiColorEditFlags_NoLabel);
            ImGui::SameLine(); ImGui::Text("Box Color");
        }
        if (currentTab==2) {
            CustomCheckbox("Speed Hack", &globals::settings::speedEnabled);
            if (globals::settings::speedEnabled) CustomSlider("Walk Speed", &globals::settings::speedValue, 16.f, 200.f);
            CustomCheckbox("Jump Hack", &globals::settings::jumpEnabled);
            if (globals::settings::jumpEnabled) CustomSlider("Jump Power", &globals::settings::jumpValue, 50.f, 500.f);
        }
        if (currentTab==3) {
            ImGui::Text("Menu Toggle: INSERT"); ImGui::Text("Aimbot Key:  Right Mouse");
            ImGui::Spacing(); ImGui::TextDisabled("nullsyntax v1.0"); ImGui::TextDisabled("kernel driver: NullKD");
        }
        ImGui::End();
    }
}
