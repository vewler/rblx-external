#pragma once
/* =============================================================
/*                       theo's offsets
/*                  https://offsets.imtheo.lol
/* -------------------------------------------------------------
/*  Dumped With     : RbxDumperV2
/*  Roblox Version  : version-e7d81637d42c4b23
/*  Dumper Version  : 2.1.7
/*  Dumped At       : 17:43 02/09/2026 (GMT)
/*  Total Offsets   : 388
/* =============================================================
*/

#include <cstdint>
#include <string>
namespace Offsets {
    inline std::string ClientVersion = "version-e7d81637d42c4b23";

    namespace BasePart {
         inline constexpr uintptr_t Primitive = 0x188;
         inline constexpr uintptr_t Transparency = 0x130;
    }

    namespace Camera {
         inline constexpr uintptr_t CameraSubject = 0xc8;
         inline constexpr uintptr_t FieldOfView = 0x140;
         inline constexpr uintptr_t Position = 0xfc;
         inline constexpr uintptr_t Rotation = 0xd8;
         inline constexpr uintptr_t ViewportSize = 0x2cc;
    }

    namespace DataModel {
         inline constexpr uintptr_t PlaceId = 0x190;
         inline constexpr uintptr_t ScriptContext = 0x440;
         inline constexpr uintptr_t Workspace = 0x158;
    }

    namespace FakeDataModel {
         inline constexpr uintptr_t Pointer = 0x8d22868;
         inline constexpr uintptr_t RealDataModel = 0x1f8;
    }

    namespace Humanoid {
         inline constexpr uintptr_t DisplayName = 0xb8;
         inline constexpr uintptr_t Health = 0x190;
         inline constexpr uintptr_t HipHeight = 0x194;
         inline constexpr uintptr_t HumanoidRootPart = 0x478;
         inline constexpr uintptr_t JumpHeight = 0x1a0;
         inline constexpr uintptr_t JumpPower = 0x1a4;
         inline constexpr uintptr_t MaxHealth = 0x1a8;
         inline constexpr uintptr_t RigType = 0x1c0;
         inline constexpr uintptr_t Walkspeed = 0x1d0;
         inline constexpr uintptr_t WalkspeedCheck = 0x3bc;
    }

    namespace Instance {
         inline constexpr uintptr_t ChildrenEnd = 0x8;
         inline constexpr uintptr_t ChildrenStart = 0x78;
         inline constexpr uintptr_t ClassDescriptor = 0x18;
         inline constexpr uintptr_t ClassName = 0x8;
         inline constexpr uintptr_t NameContainer = 0x70;
         inline constexpr uintptr_t Parent = 0x68;
    }

    namespace Misc {
         inline constexpr uintptr_t StringLength = 0x10;
    }

    namespace Player {
         inline constexpr uintptr_t DisplayName = 0x138;
         inline constexpr uintptr_t LocalPlayer = 0x130;
         inline constexpr uintptr_t ModelInstance = 0x298;
         inline constexpr uintptr_t Team = 0x2d8;
         inline constexpr uintptr_t TeamColor = 0x3b0;
         inline constexpr uintptr_t UserId = 0xd0;
    }

    namespace Primitive {
         inline constexpr uintptr_t AssemblyLinearVelocity = 0xf8;
         inline constexpr uintptr_t Position = 0xec;
         inline constexpr uintptr_t Rotation = 0xc8;
         inline constexpr uintptr_t Size = 0x1bc;
    }

    namespace RenderView {
         inline constexpr uintptr_t VisualEngine = 0x10;
    }

    namespace TaskScheduler {
         inline constexpr uintptr_t Pointer = 0x8abd728;
    }

    namespace VisualEngine {
         inline constexpr uintptr_t Pointer = 0x8351408;
         inline constexpr uintptr_t ViewMatrix = 0x1b0;
    }

    namespace Workspace {
         inline constexpr uintptr_t CurrentCamera = 0x4b8;
    }
}
