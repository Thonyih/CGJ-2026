# Lecture 4 - Hello 2D World

The teacher's example from `Docs/Lectures/4-Rendering-Pipeline/02-hello-2d-world-2026`. It draws two triangles directly in clip space using the course `mgl` framework. This is the starting point for Assignment A2 (Tangram).

The code in `mgl\` and `src\` is copied **unchanged**. Only the build files were added.

## Run

This folder is **self-contained**: it has its own `CMakeLists.txt`, its own `CMakePresets.json` and its own `libraries\` (GLM 1.0.1, GLFW 3.4, GLEW 2.2.0).

**On its own** (open `labs/hello-2d-world/` as the folder):
- **Visual Studio (main target):**
  - Open Folder: File → Open → Folder, then F5.
  - Solution: run `cmake --preset vs2026` (or `vs2022`), open `build\vs2026\hello-2d-world.slnx`, then F5. `hello-2d-world` is the startup project.
- **VS Code:** pick the *Debug (MSVC, Ninja)* preset, then Build and Run.
- **Command line** (Developer PowerShell): `cmake --preset debug`, then `cmake --build --preset debug`, then `.\build\debug\hello-2d-world.exe`

**From the main project** (open the repository root): choose the `hello-2d-world` target.

A 600×600 window opens with two coloured triangles. In Debug builds, the console prints the OpenGL, GLEW, GLFW and GLM versions.

## Build notes

- **Shaders:** `clip-vs.glsl` and `clip-fs.glsl` are opened by file name, so the program needs to run from a folder that contains them.
  - The Visual Studio solution runs it from `src\`.
  - After every build, the shaders are also copied next to the `.exe`, for Open Folder, VS Code and the command line.
- **`DEBUG`:** `mgl` uses `#ifdef DEBUG` to turn on the OpenGL debug output. MSVC only defines `_DEBUG`, so `CMakeLists.txt` defines `DEBUG` in Debug builds.

## Files

| File | Contents |
|---|---|
| `src/hello-2d-world.cpp` | `MyApp`: the VAO, VBO and EBO, the shader program, and the draw calls |
| `src/clip-vs.glsl`, `src/clip-fs.glsl` | Vertex shader (`Matrix * inPosition`) and fragment shader (vertex colour) |
| `mgl/` | Course framework: `Engine` (window and main loop), `App` (callbacks), `ShaderProgram` and the OpenGL debug output |
| `libraries/` | GLM, GLFW, GLEW |
