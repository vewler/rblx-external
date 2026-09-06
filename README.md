# rblx-external

external roblox cheat with kernel driver communication, dx11 overlay, and imgui menu.

## features

- **ESP** — 2D boxes, player names, health bars, snaplines, distance
- **Aimbot** — smooth mouse movement via SendInput, sticky targeting, configurable FOV
- **Movement** — walkspeed & jumppower modification with checksum bypass
- **Overlay** — transparent dx11 overlay with dwm extend, click-through when menu is closed
- **Menu** — nullsyntax-themed imgui interface (INSERT to toggle)

## requirements

- Windows 10/11 x64
- Visual Studio 2025 (v145 toolset)
- NullKD kernel driver loaded
- Roblox running in-game before launching

## building

1. open `RobloxExternal.sln` in Visual Studio
2. set config to **Release | x64**
3. build solution (Ctrl+Shift+B)
4. output binary is in `bin/Release/`

## usage

1. load NullKD driver
2. launch Roblox and join a game
3. run `RobloxExternal.exe` as administrator
4. **INSERT** — toggle menu
5. **END** — exit

## controls

| Key | Action |
|-----|--------|
| INSERT | toggle menu |
| RMB (hold) | aimbot |
| END | exit |

## offsets

using theo's dumper — version `e7d81637d42c4b23`. update `Offsets/offsets.hpp` when roblox updates.

## structure

```
src/
  main.cpp        — entry point, overlay loop, thread management
  driver.hpp      — kernel driver IOCTL wrapper
  scanner.hpp     — datamodel traversal, player caching
  esp.hpp         — esp rendering
  aimbot.hpp      — aimbot logic
  movement.hpp    — walkspeed/jumppower hacks
  menu.hpp        — imgui menu
  overlay.hpp     — dx11 overlay setup
  globals.hpp     — shared state
  math.hpp        — vec2/vec3/viewmatrix
Offsets/
  offsets.hpp     — game offsets
ext/
  imgui/          — dear imgui (with dx11+win32 backends)
```

## disclaimer

for educational purposes only. use at your own risk.
