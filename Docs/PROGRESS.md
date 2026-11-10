# Progress

What has been done, newest first.

## 2026-11-10 - Lab 2 (GLM): single source file
- `labs/lab2-glm/` now has all its code in one file, `main.cpp`. It's in five sections: settings, helpers, matrix builders, exercises 1-5, `main()`.
- Verified: builds standalone and from the root, with Ninja and with the Visual Studio 2026 solution. Still 42 / 42 checks.

## 2026-11-06 - Lab 2 visual moved to its own folder
- `labs/lab2-visual/` is now a separate, self-contained program with its own CMake files and libraries (GLM, GLFW, GLEW).
- `labs/lab2-glm/` is back to the console program with only GLM.
- Verified: both build and run on their own and from the root, with Ninja and with the Visual Studio 2026 solution.

## 2026-11-06 - Lab 2 (GLM): visual program
- Added `labs/lab2-glm/lab2-visual/`, an OpenGL 4.6 window that draws exercises 1-5 using the same `exercises.cpp` functions. Keys 1-5 pick the exercise, the arrow keys move the camera, and Space picks new random values.
- The lab now also includes GLFW 3.4 and GLEW 2.2.0 in its own `libraries/`, so it stays self-contained.
- Verified: builds standalone and from the root, with Ninja and with the Visual Studio 2026 solution. All five scenes were checked visually, and `lab2-glm` still passes 42 / 42.

## 2026-11-06 - Libraries included in the project
- GLFW 3.4, GLEW 2.2.0 and GLM 1.0.1 are now in `libraries/`, and Lab 2 has its own `labs/lab2-glm/libraries/glm`. Nothing is downloaded, as the submission rules require.
- Verified: the root and the standalone Lab 2 both build and run with Ninja and with the Visual Studio 2026 solution. Lab 2 passes 42 / 42.

## 2026-11-06 - Lab 2 (GLM): all exercises done
- Exercises 1-5 are implemented in `labs/lab2-glm/exercises.cpp`. Every result is checked, and 42 / 42 checks pass.
- Verified in the Visual Studio 2026 solution build.

## 2026-11-06 - Lab 2 (GLM): structure
- Added `labs/lab2-glm/`, a console program with one function per exercise, PASS/FAIL checks and random-vector helpers.
- The exercises themselves are still TODO.

## 2026-11-03 - Project setup
- The project builds and runs on Windows with MSVC, in VS Code and in Visual Studio Community (CMake presets).
- Libraries: GLFW 3.4, GLM 1.0.1 and GLEW 2.2.0. OpenGL 4.6 Core.
