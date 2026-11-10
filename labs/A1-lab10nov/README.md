# Assignment A1 - Math Challenge (exercises 1-3)

Console program for exercises 1 to 3 of `Docs/Lab/A1/AssignmentA1.pdf`. All the code is in one file, `main.cpp`, written as simply as possible.

## Run

This folder is self-contained: it has its own `CMakeLists.txt` and `CMakePresets.json`, and GLM 1.0.1 is included in `libraries\glm`.

- **Visual Studio:** File → Open → Folder (this folder), then F5.
- **VS Code:** pick the *Debug (MSVC, Ninja)* preset, then Build and Run.
- **Command line** (Developer PowerShell): `cmake --preset debug`, then `cmake --build --preset debug`, then `.\build\debug\A1-lab10nov.exe`

## Methods

| Exercise | Method | What it does |
|---|---|---|
| 1 | `tripleProduct(i, j, k)` | Computes i x (j x k) and j(i . k) - k(i . j) in double precision, prints both and whether they are equal, and returns `true` or `false`. Called 5 times with `randomVec(1, 5)` |
| 2 | `cameraFrame(view, up)` | Computes and prints v, w and u. If view and up are parallel, it prints an error instead (it can't divide by zero). Called with the 3 cases from the assignment |
| 3 | `rodrigues(v, axis, angle)` | Returns the rotated vector. The angle is in radians, as in GLM. `testRodrigues` prints the input, the expected output and the result, and whether they are equal |

Helpers: `randomVec(min, max)` and `randomMat3(min, max)` use the same technique as the helpers shown in class (`std::mt19937` + `uniform_real_distribution`), with the range as a parameter. Vectors are printed with `glm::to_string`.

Comparisons use `THRESHOLD = 1.0e-5` and `glm::epsilonEqual`, as the assignment asks.

## Exercise 3 inputs and expected outputs

| v | axis | angle | expected |
|---|---|---|---|
| (1, 2, 3) | (2, 2, 2) | 120 | (3, 1, 2) |
| (1, 2, 3) | (1, 1, 1) | 180 | (3, 2, 1) |
| (4, 4, 4) | (3, 3, 3) | 90 | (4, 4, 4) |

For a plain explanation of each exercise, see [EXPLANATION.md](EXPLANATION.md).
