# Lab 2 - GLM, visual version

An OpenGL 4.6 window that draws what each Lab 2 exercise computes. It's a separate program from the console version in [`../lab2-glm`](../lab2-glm/README.md), which holds the PASS/FAIL checks.

## Run

This folder is **self-contained**: it has its own `CMakeLists.txt` and `CMakePresets.json`, and its own `libraries\` (GLM 1.0.1, GLFW 3.4, GLEW 2.2.0). Nothing needs to be downloaded or installed. It builds either on its own or as part of the main project.

**On its own** (open `labs/lab2-visual/` as the folder):
- **Visual Studio:** File → Open → Folder, then F5. Or run `cmake --preset vs2026` (or `vs2022`) and open the solution in `build\`. `lab2-visual` is the startup project.
- **VS Code:** pick the *Debug (MSVC, Ninja)* preset, then Build and Run.
- **Command line** (Developer PowerShell): `cmake --preset debug`, then `cmake --build --preset debug`, then `.\build\debug\lab2-visual.exe`

**From the main project** (open the repository root): choose the `lab2-visual` target or startup item.

## Keys

`1`-`5` choose the exercise, arrows rotate the camera, `Space` picks new random values, `R` resets the camera, `Esc` quits. The console explains the colours each time the exercise changes.

| Key | What is drawn |
|---|---|
| 1 | u, v, w and v + w. u x (v + w) (yellow) and u x v + u x w drawn tip-to-tail (orange, then magenta) end at the same point |
| 2 | u, v, w (white), after Rx 45 deg (orange) and after Ry 90 deg (cyan) |
| 3 | Three dice with their own axes: the original, x then y (Ry * Rx), and y then x (Rx * Ry). The last two face different ways |
| 4 | a, b and the plane between them, with orthogonalVector(a, b) (yellow) and the swapped version (magenta) pointing the opposite way |
| 5 | The unit square and its two transformed copies on a grid. Corner dots: A yellow, B red, C green, D blue |

## Files

| File | Contents |
|---|---|
| `main.cpp` | Window, keyboard and camera |
| `renderer.cpp` | Minimal line renderer: one shader, one buffer, lines and points |
| `scenes.cpp` | One drawing per exercise |
| `exercises.cpp`, `helpers.cpp` | **Copies** of Lab 2's files, so this folder stands alone. The drawings use the same maths that `lab2-glm` checks. If those files change in `lab2-glm`, copy them here too |
| `libraries/` | GLM, GLFW, GLEW |

## Requirements and Windows on ARM

`lab2-visual` asks for **OpenGL 4.6** and, if that isn't available, falls back to **OpenGL 3.3**. Its shaders are written for GLSL 3.30, so it runs on either.

On **Windows on ARM** (Snapdragon), OpenGL comes from Microsoft's **"OpenCL, OpenGL, and Vulkan Compatibility Pack"** (Microsoft Store). If the window still can't be created, the program says so and exits with code 1. Install that pack and try again.
