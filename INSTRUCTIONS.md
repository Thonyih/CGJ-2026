# How to build and run

This project targets **Windows only** and uses **OpenGL 4.6 Core**, with GLFW, GLEW and GLM.

## 1. Prerequisites

- **Visual Studio Community** 2026 or 2022, with the **Desktop development with C++** workload. This installs the MSVC compiler, CMake and Ninja.
- Nothing else. GLFW 3.4, GLEW 2.2.0 and GLM 1.0.1 are included in the `libraries\` folder, so there is nothing to download or install, and no internet connection is needed.

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
CMakeLists.txt       build configuration (uses the libraries in libraries/)
CMakePresets.json    shared build presets for VS Code and Visual Studio
libraries/           GLFW, GLEW and GLM, included with the project
labs/                one self-contained folder per lab (own CMakeLists.txt, presets and libraries/)
src/main.cpp         program entry point
shaders/             GLSL shader files
assets/              models, textures, etc.
Docs/                course lectures and lab instructions
```

## Differences from the lab setup guide

The lab guide (`Docs/Lab/project_configuration_instructions.md`) has you extract prebuilt GLEW, GLFW and GLM into `libraries\` and configure the project properties by hand. This project uses the **same `libraries\` folder, the same three libraries and the same OpenGL linkage**, but `CMakeLists.txt` sets them up automatically. The code is the same either way (`#include <GL/glew.h>`, `<GLFW/glfw3.h>`, `<glm/glm.hpp>`).

### Each lab-guide step and how it's covered

| Lab guide step | In this project |
|---|---|
| Extract GLEW, GLFW, GLM into `libraries\` | Same: already included as `libraries\glew`, `libraries\glfw` and `libraries\glm` (source versions) |
| Add each library's `include` folder to *Additional Include Directories* | Added automatically by `CMakeLists.txt` |
| Add the `lib` folders to *Additional Library Directories* | Not needed: CMake knows where each library is |
| Link `opengl32.lib`, `glew32.lib`, `glfw3dll.lib` | `target_link_libraries(... glew glfw)`, plus `opengl32` (GLM is header-only, nothing to link) |
| Copy `glew32.dll` and `glfw3.dll` with a post-build `xcopy` | Not needed: GLEW and GLFW are linked **statically** (`GLEW_STATIC`), so the `.exe` runs on its own |
| Set *Subsystem* to *Console* to avoid the `WinMain` LNK2019 error | Already the default for `add_executable` |
| One project per entry point | One CMake target per program (`my-engine`, `lab2-glm`, …) |

### Why this works the same or better

- **Same result:** the program compiles against the same headers and links the same libraries. Only the way they are set up differs.
- **Exact versions:**
  - Each library is pinned to an exact version: GLFW 3.4, GLM 1.0.1 and GLEW 2.2.0.
  - They are included in the project, so everyone who builds it gets identical libraries.
- **Built with your own compiler:** the libraries are compiled from source by the same compiler as the project. That avoids mismatches with prebuilt binaries made for a specific compiler version (the guide's `lib-vc2022` folder).
- **No manual configuration:** there are no property pages to fill in, no paths to type and no DLLs to copy. That removes the most common setup mistakes (wrong path, missing DLL, x86/x64 mix-up, Debug and Release set up differently).
- **Debug and Release both work:** CMake applies the same library setup to every configuration, while the guide's settings have to be repeated for each one.
- **Easy to add programs:** a new lab or assignment needs only a few lines in its own `CMakeLists.txt`, and it gets the libraries automatically.
- **Works offline, nothing to install:** the libraries are in `libraries\`, so building needs only Visual Studio with the C++ workload.

## Troubleshooting

| Problem | Fix |
|---|---|
| `cl.exe` or `ninja` not found | Use a *Developer PowerShell* or *x64 Native Tools* prompt, or open the folder in VS Code or Visual Studio, which set up the environment for you. |
| "Failed to create GLFW window" | The GPU or driver doesn't support OpenGL 4.6. Over RDP, check that the renderer isn't "GDI Generic". |
| `vs2022` preset fails | VS 2022 isn't installed on this machine. Use `vs2026` instead. |
| Strange build errors after pulling changes | Delete `build/` and configure again. |
