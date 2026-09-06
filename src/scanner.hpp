#pragma once
#include "driver.hpp"
#include "globals.hpp"
#include "../Offsets/offsets.hpp"
#include <vector>
#include <string>

class Scanner {
public:
    static std::vector<uintptr_t> GetChildren(uintptr_t instance) {
        std::vector<uintptr_t> result;
        if (!instance) return result;

        uintptr_t childrenPtr = g_driver.Read<uintptr_t>(instance + Offsets::Instance::ChildrenStart);
        if (!childrenPtr) return result;

        uintptr_t begin = g_driver.Read<uintptr_t>(childrenPtr);
        uintptr_t end   = g_driver.Read<uintptr_t>(childrenPtr + Offsets::Instance::ChildrenEnd);
        if (!begin || !end || end <= begin) return result;

        size_t count = (end - begin) / sizeof(uintptr_t);
        if (count > 2000) return result;

        std::vector<uintptr_t> buffer(count);
        if (g_driver.ReadRaw(begin, buffer.data(), count * sizeof(uintptr_t))) {
            for (auto ptr : buffer) {
                if (ptr > 0x10000) result.push_back(ptr);
            }
        }
        return result;
    }

    static std::string GetName(uintptr_t instance) {
        if (!instance) return "";
        return g_driver.ReadRobloxString(instance + Offsets::Instance::NameContainer);
    }

    static std::string GetClassName(uintptr_t instance) {
        if (!instance) return "";
        uintptr_t descriptor = g_driver.Read<uintptr_t>(instance + Offsets::Instance::ClassDescriptor);
        if (!descriptor) return "";
        return g_driver.ReadRobloxString(descriptor + Offsets::Instance::ClassName);
    }

    static uintptr_t FindFirstChild(uintptr_t instance, const std::string& name) {
        for (auto child : GetChildren(instance))
            if (GetName(child) == name) return child;
        return 0;
    }

    static uintptr_t FindFirstChildOfClass(uintptr_t instance, const std::string& className) {
        for (auto child : GetChildren(instance))
            if (GetClassName(child) == className) return child;
        return 0;
    }

    static Vec3 GetPartPosition(uintptr_t part) {
        if (!part) return {};
        uintptr_t prim = g_driver.Read<uintptr_t>(part + Offsets::BasePart::Primitive);
        if (!prim) return {};
        Vec3 pos;
        g_driver.ReadRaw(prim + Offsets::Primitive::Position, &pos, sizeof(Vec3));
        return pos;
    }

    static bool ScanDataModel() {
        uintptr_t base = g_driver.GetBaseAddress();
        if (!base) return false;

        uintptr_t fakePtr = g_driver.Read<uintptr_t>(base + Offsets::FakeDataModel::Pointer);
        if (!fakePtr) return false;

        uintptr_t dataModel = g_driver.Read<uintptr_t>(fakePtr + Offsets::FakeDataModel::RealDataModel);
        if (!dataModel) return false;

        globals::dataModel = dataModel;
        globals::visualEngine = g_driver.Read<uintptr_t>(base + Offsets::VisualEngine::Pointer);

        for (auto child : GetChildren(dataModel)) {
            if (GetClassName(child) == "Players") {
                globals::playersService = child;
                globals::localPlayer = g_driver.Read<uintptr_t>(child + Offsets::Player::LocalPlayer);
            }
        }
        return globals::playersService != 0;
    }

    static void UpdateViewMatrix() {
        if (!globals::visualEngine) return;
        g_driver.ReadRaw(globals::visualEngine + Offsets::VisualEngine::ViewMatrix, globals::viewMatrix.m, sizeof(float) * 16);
    }

    static void UpdateLocalPosition() {
        if (!globals::localPlayer) return;
        uintptr_t character = g_driver.Read<uintptr_t>(globals::localPlayer + Offsets::Player::ModelInstance);
        if (!character) return;
        uintptr_t humanoid = FindFirstChildOfClass(character, "Humanoid");
        if (!humanoid) return;
        uintptr_t rootPart = g_driver.Read<uintptr_t>(humanoid + Offsets::Humanoid::HumanoidRootPart);
        if (!rootPart) return;
        globals::localRootPos = GetPartPosition(rootPart);
    }

    static void CachePlayers() {
        if (!globals::playersService) return;

        auto playerInstances = GetChildren(globals::playersService);
        std::vector<PlayerData> newPlayers;
        newPlayers.reserve(playerInstances.size());

        for (auto playerPtr : playerInstances) {
            if (playerPtr == globals::localPlayer) continue;

            uintptr_t character = g_driver.Read<uintptr_t>(playerPtr + Offsets::Player::ModelInstance);
            if (!character) continue;

            uintptr_t humanoid = FindFirstChildOfClass(character, "Humanoid");
            if (!humanoid) continue;

            uintptr_t rootPart = g_driver.Read<uintptr_t>(humanoid + Offsets::Humanoid::HumanoidRootPart);
            if (!rootPart) continue;

            uintptr_t head = FindFirstChild(character, "Head");

            PlayerData pd;
            pd.name      = GetName(playerPtr);
            pd.rootPos   = GetPartPosition(rootPart);
            pd.headPos   = head ? GetPartPosition(head) : (pd.rootPos + Vec3(0, 1.5f, 0));
            pd.health    = g_driver.Read<float>(humanoid + Offsets::Humanoid::Health);
            pd.maxHealth = g_driver.Read<float>(humanoid + Offsets::Humanoid::MaxHealth);
            pd.humanoid  = humanoid;
            pd.valid     = (pd.rootPos.x != 0.f || pd.rootPos.y != 0.f || pd.rootPos.z != 0.f);

            if (pd.valid && pd.maxHealth > 0.f && pd.health > 0.f)
                newPlayers.push_back(std::move(pd));
        }

        std::lock_guard<std::mutex> lock(globals::playerMtx);
        globals::players = std::move(newPlayers);
    }
};
