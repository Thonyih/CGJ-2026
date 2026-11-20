# Progress

What has been done, newest first.

## 2026-11-20 - New course material: Lectures 5-6 and Assignment A3
- Added to `Docs/`:
  - `Lectures/5-Viewing-Pipeline/`: handouts + `03-hello-3d-world` (3D, cameras: mgl now has `mglCamera`)
  - `Lectures/6-Geometry-management/`: handouts + `04-mesh-loader` (meshes loaded from `.obj` files with **Assimp**: mgl now has `mglMesh` and `mglScenegraph`) + Blender assets
  - `Assignments/A3/AssignmentA3.pdf`: A3 Pickagram
- A3 asks for:
  - **Geometry:** 3D pieces from Blender, loaded with Assimp; a colour per piece and a different tint per face; a scene graph (mesh + shader + model matrix per node, children follow parents); the pieces on a tabletop that moves with a left-mouse drag.
  - **Animation:** a second "closed box" configuration on another plane; the left/right arrows animate between the two, can stop halfway and reverse.
  - **Camera:** 2 orbiting cameras (right-mouse drag, scroll to zoom), quaternions, `c` switches camera, `p` switches orthographic/perspective, no stretching on resize.
- Not started yet. **To do first:** add Assimp to the libraries (it isn't in `libraries/` yet) and get `04-mesh-loader` running in Visual Studio.

## 2026-11-18 - Assignment A2: delivered version (`assignments/A2-tangram-clean/`)
- Rebuilt A2 step by step, starting from the lecture's hello-2d-world, to understand each part. This is the version handed in (`A2-CGJ-Grupo3.zip`).
- One `Vertex` = position only. Colour is a `Color` uniform sent before each draw, so the 5 triangle pieces share one triangle buffer.
- `Shape` class (VAO + VBO + EBO, `create` / `draw` / `destroy`) and `Piece` class (shape + colour + x, y, angle, size). The 7 pieces are a list, drawn in a loop.
- `modelMatrix(x, y, angle, size)` = `Aspect * SCENE * T * R * S`. `SCENE` fits the shark in the window. `Aspect` is updated in `windowSizeCallback` so the shark isn't stretched when the window is resized.
- Verified: builds in Visual Studio and shows the full shark.

## 2026-11-16 - Assignment A2: 2D Tangram (Shark)
- New `assignments/` folder. A1 moved to `assignments/A1-lab10nov/`.
- Added `labs/hello-2d-world/`: the Lecture 4 example, set up to run in Visual Studio.
- Added `assignments/A2-tangram/`, built on hello-2d-world with the teacher's `mgl` unchanged:
  - 3 base shapes (right triangle, square, parallelogram) as buffer objects, made only of counter-clockwise triangles.
  - All 5 triangles reuse the same base triangle, scaled 1, √2 or 2.
  - Each piece is placed with `T * R * S` matrices sent as a uniform, and has its own colour uniform.
  - `Shape` and `Piece` classes. The drawing order doesn't matter.
- The sizes follow `Docs/Lab/A2/Tangram-Stock-Sizing.jpeg`: the Tangram square is 1, and the base triangle is the large one (long side 1), scaled 1, √2/2 or ½. Each base shape is made as it sits in the stock square.
- A2 is a standalone Visual Studio delivery: open `A2-tangram.sln` and press F5. There is no CMake. The libraries in `libraries\` are compiled with the program, and all paths are relative.
- Verified: Debug and Release build with MSBuild. A copy in another folder builds and runs, and the window matches `Docs/Lab/A2/Shark-Color.jpeg`.

## 2026-11-12 - Assignment A1: exercises 4-6
- Added to `labs/A1-lab10nov/main.cpp`:
  - Rodrigues rotation in matrix form (each axis rotated by 90° around x, y and z: 9 tests with expected outputs)
  - (AB)^T = B^T A^T (5 random matrix pairs, values -5 to 5)
  - (AB)^-1 = B^-1 A^-1 (5 random invertible pairs, det != 0)
- `float` only. Random values are whole numbers so float precision stays within `THRESHOLD = 1.0e-5`.
- Verified: 200 runs, 5,400 comparisons, all "Equal: yes". Builds standalone and from the root, with Ninja and with Visual Studio 2026.

## 2026-11-10 - Assignment A1: exercises 1-3
- Added `labs/A1-lab10nov/`, a self-contained console program in one `main.cpp`:
  - triple product (5 random tests, values 1-5)
  - camera frame from view and up (the 3 given cases, including the parallel case, which is detected)
  - Rodrigues rotation in vector form (3 calls with expected outputs)
- Uses `THRESHOLD = 1.0e-5` and `glm::epsilonEqual`, as the assignment requires.
- Simplified to plain output (no colours or PASS/FAIL markers): each comparison prints "Equal: yes/no". All comparisons print yes.
- Verified: builds standalone and from the root, with Ninja and with the Visual Studio 2026 solution.

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
