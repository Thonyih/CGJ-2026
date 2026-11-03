# How to build and run

This project targets **Windows only** and uses **OpenGL 4.6 Core**, with GLFW, GLEW and GLM.

## 1. Prerequisites

- **Visual Studio Community** 2026 or 2022, with the **Desktop development with C++** workload. This installs the MSVC compiler, CMake and Ninja.
- **Git**.
- An internet connection for the first configure. CMake downloads GLFW 3.4, GLEW 2.2.0 and GLM 1.0.1 into `build/`, so there's nothing to install by hand.

## 2. Get the code

```
git clone https://github.com/Thonyih/CGJ-2026.git
cd CGJ-2026
```

## 3. Build and run

You can use any of the three options below. They all use the same `CMakeLists.txt` and `CMakePresets.json`.

### Option A: VS Code (day-to-day development)

1. Install the **CMake Tools**, **C/C++** and **clangd** extensions.
2. Open the folder. When CMake Tools asks for a configure preset, pick **Debug (MSVC, Ninja)**.
3. Build with F7, or with the Build button in the status bar. Run or debug with Shift+F5 or F5.

**Code completion and errors:** clangd reads `build/debug/compile_commands.json`, as set in `.clangd`, so configure the `debug` preset at least once. `.vscode/settings.json` turns off the C/C++ extension's own IntelliSense so you don't get duplicate errors; that extension is still used for debugging. If you see "cannot open source file GL/glew.h", configure or build once, then run **clangd: Restart language server**.

### Option B: Visual Studio, Open Folder

1. Choose *File → Open → Folder…* and select the project folder.
2. Pick the **Debug (MSVC, Ninja)** configuration in the toolbar.
3. Select `my-engine.exe` as the startup item and press F5.

### Option C: Visual Studio solution (.sln / .slnx)

From a **Developer PowerShell** or **x64 Native Tools** prompt, in the project folder:

```
cmake --preset vs2026      # VS 2026: build/vs2026/my-engine.slnx
cmake --preset vs2022      # VS 2022: build/vs2022/my-engine.sln
```

Open the generated solution. `my-engine` is already the startup project, and it runs from the project root.

### Command line only

From an **x64 Native Tools** prompt:

```
cmake --preset debug
cmake --build --preset debug
build\debug\my-engine.exe
```

A window with a blue background should open, and the console prints the OpenGL version (4.6), the renderer and the GLEW version. Press **Esc** or close the window to quit.

## Project layout

```
CMakeLists.txt       build configuration (downloads GLFW, GLEW, GLM)
CMakePresets.json    shared build presets for VS Code and Visual Studio
src/main.cpp         program entry point
shaders/             GLSL shader files
assets/              models, textures, etc.
Docs/                course lectures and lab instructions
```

## Differences from the lab setup guide

The lab guide (`Docs/Lab/project_configuration_instructions.md`) puts prebuilt GLEW, GLFW and GLM zips in `libraries\` and configures the `.sln` by hand. This project gets the same libraries through CMake instead:

- GLEW and GLFW are linked **statically** (with `GLEW_STATIC` defined), so no DLLs need to be copied next to the exe.
- `opengl32.lib` is linked automatically.
- The console subsystem is already set, so there's no `WinMain` linker error.

## Troubleshooting

| Problem | Fix |
|---|---|
| `cl.exe` or `ninja` not found | Use a *Developer PowerShell* or *x64 Native Tools* prompt, or open the folder in VS Code or Visual Studio, which set up the environment for you. |
| "Failed to create GLFW window" | The GPU or driver doesn't support OpenGL 4.6. Over RDP, check that the renderer isn't "GDI Generic". |
| `vs2022` preset fails | VS 2022 isn't installed on this machine. Use `vs2026` instead. |
| Strange build errors after pulling changes | Delete `build/` and configure again. |
