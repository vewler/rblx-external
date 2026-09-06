# **rblx-external**

External Roblox cheat featuring kernel driver communication, DX11 overlay, and an ImGui menu.

---

## Features

### ESP
- **2D Boxes** — Player bounding boxes
- **Player Names** — Overhead text labels
- **Health Bars** — Dynamic player health visualizer
- **Snaplines** — Lines pointing to player positions
- **Distance** — Real-time distance calculation

### Aimbot
- **Smooth Mouse Movement** — Natural target tracking via `SendInput`
- **Sticky Targeting** — Locks onto target while active
- **Configurable FOV** — Custom field-of-view radius

### Movement
- **WalkSpeed & JumpPower** — Real-time movement property modification
- **Checksum Bypass** — Memory check prevention

### Overlay
- **Transparent DX11 Overlay** — Direct3D 11 rendering with DWM extension
- **Click-Through** — Pass-through mouse input when menu is closed

### Menu
- **NullSyntax ImGui** — Custom styled interface toggled via <kbd>INSERT</kbd>

---

## Requirements

> [!IMPORTANT]
> Make sure all requirements are fulfilled before launching the application.

- **OS:** Windows 10/11 x64
- **IDE & Toolset:** Visual Studio 2025 (v145 toolset)
- **Driver:** NullKD kernel driver loaded
- **Roblox:** Game running and spawned in-game before launching

---

## Building

1. Open `RobloxExternal.sln` in **Visual Studio**.
2. Set configuration to **Release | x64**.
3. Build the solution using <kbd>Ctrl</kbd> + <kbd>Shift</kbd> + <kbd>B</kbd>.
4. The output binary will be located in `bin/Release/`.

---

## Usage

1. Load the **NullKD** driver.
2. Launch Roblox and join a game session.
3. Run `RobloxExternal.exe` as **Administrator**.
4. Press <kbd>INSERT</kbd> to toggle the menu interface.
5. Press <kbd>END</kbd> to safely exit.

---

## Controls

| Key | Action |
| :--- | :--- |
| <kbd>INSERT</kbd> | Toggle Menu |
| <kbd>END</kbd> | Exit |
