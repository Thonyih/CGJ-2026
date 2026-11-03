# How to build and run

These steps are for macOS with an Apple Silicon or Intel Mac. The project uses OpenGL 4.1 Core, the newest version macOS supports.

## 1. Install prerequisites

Install the Xcode Command Line Tools (compiler and SDK):

```sh
xcode-select --install
```

Install [Homebrew](https://brew.sh) if you don't have it, then install the build tool and libraries:

```sh
brew install cmake glfw glm
```

GLAD is already in the repo under `external/glad/`, so there's nothing to install for it.

## 2. Get the code

```sh
git clone https://github.com/Thonyih/CGJ-2026.git
cd CGJ-2026
```

## 3. Build

Run these commands from the project root:

```sh
cmake -S . -B build
cmake --build build
```

The first command configures the project into `build/`, and the second compiles it. After you change code, you only need to rerun `cmake --build build`.

## 4. Run

```sh
./build/my-engine
```

A window should open with a blue background, and the terminal prints the OpenGL version. Press **Esc** or close the window to quit.

Build and run in one line:

```sh
cmake -S . -B build && cmake --build build && ./build/my-engine
```

## Using VS Code (optional)

Recommended extensions:
- **clangd**, or Microsoft's **C/C++**, for code completion
- **CMake Tools**, for building from the status bar
- **CodeLLDB**, for debugging

Open the project folder, let CMake Tools configure it, then build and run or debug from the status bar. Run one build first: clangd reads `build/compile_commands.json`, which the build creates, to find the headers.

## Project layout

```
CMakeLists.txt       build configuration
src/main.cpp         program entry point
external/glad/       OpenGL function loader (generated for GL 4.1 Core)
shaders/             GLSL shader files
assets/              models, textures, etc.
```

## Troubleshooting

| Problem | Fix |
|---|---|
| `Could not find a package configuration file provided by "glfw3"` (or `"glm"`) | Run `brew install glfw glm` |
| `cmake: command not found` | Run `brew install cmake` |
| Linker warnings like `building for macOS-XX, but linking with dylib ... built for newer version` | Your Command Line Tools are older than your macOS. Update them in System Settings → Software Update, then delete `build/` and rebuild. |
| Strange build errors after pulling changes | Delete `build/` (`rm -rf build`) and build again |
