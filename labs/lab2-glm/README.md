# Lab 2 - GLM

Console program covering the five exercises from the Lab 2 GLM slides. It prints each exercise's intermediate results and a PASS/FAIL line for every check.

There is a separate visual version that draws the exercises in an OpenGL window: see [`../lab2-visual`](../lab2-visual/README.md).

## Run

This folder is **self-contained**: it has its own `CMakeLists.txt` and `CMakePresets.json`, and GLM 1.0.1 is included in `libraries\glm`. Nothing needs to be downloaded or installed. It builds either on its own or as part of the main project.

**On its own** (open `labs/lab2-glm/` as the folder):
- **Visual Studio:** File → Open → Folder, then F5. Or run `cmake --preset vs2026` (or `vs2022`) and open the solution in `build\`.
- **VS Code:** pick the *Debug (MSVC, Ninja)* preset, then Build and Run.
- **Command line** (Developer PowerShell): `cmake --preset debug`, then `cmake --build --preset debug`, then `.\build\debug\lab2-glm.exe`

**From the main project** (open the repository root): choose the `lab2-glm` target or startup item.

The program exits with code 0 only when every check passes.

## Files

| File | Contents |
|---|---|
| `main.cpp` | Seeds the random numbers, runs exercises 1-5 and prints the summary |
| `exercises.cpp` | One function per exercise, plus `orthogonalVector` and `areOrthogonal` |
| `helpers.cpp` | Random vectors, float comparison with a tolerance, printing, `check()` |
| `libraries/glm` | GLM 1.0.1 (header-only) |

## Status

| Exercise | Topic | Status |
|---|---|---|
| 1 | Cross product is distributive over addition | Done: 5 random tests |
| 2 | Rx = 45°, Ry = 90° applied to u, v, w | Done: hand-built matrices, checked against glm::rotate |
| 3 | 3D rotations are not commutative | Done: random 3D and 2D tests |
| 4 | Vector orthogonal to a plane | Done: analytic answer, swap, 2D basis, random tests |
| 5 | Unit square as a matrix, then transformed | Done: square as a 3x4 matrix, T*R and T*R*S |

**Result:** 42 / 42 checks pass.

## Answers in short

- **Ex 1:** u x (v + w) and u x v + u x w are equal for every random u, v, w.
- **Ex 2:** Rx(45) * u = (1, 0, 1.4142), Rx * v = (-1, -0.7071, -0.7071), Rx * w = (1, 0, 0). Ry(90) turns (x, y, z) into (z, y, -x): u -> (1, 1, -1), v -> (0, -1, 1), w -> (0, 0, -1).
- **Ex 3:** Ry * Rx != Rx * Ry, so the order of 3D rotations changes the result. Two 2D rotations about the origin always give the same result in either order.
- **Ex 4:** a x b = (-3, 6, -3). Its dot products with a and b are both 0. As a unit vector it is (-0.4082, 0.8165, -0.4082). Swapping the inputs gives the opposite vector, because b x a = -(a x b). For the 2D basis, e1 x e2 = (0, 0, 1).
- **Ex 5:** The 4 corners are the columns of a 3x4 matrix of homogeneous points (x, y, 1). A'B'C'D' = T(2, 1) * R(-45). A''B''C''D'' = T(-2, -2) * R(180) * S(2, 2). The rotation directions follow the corner labels in the diagram.
