# Assignment A1 - Math Challenge (exercises 1-6)

Console program for all six exercises of `Docs/Lab/A1/AssignmentA1.pdf`. All the code is in one file, `main.cpp`, written as simply as possible and using `float` only.

## Run

This folder is self-contained: it has its own `CMakeLists.txt` and `CMakePresets.json`, and GLM 1.0.1 is included in `libraries\glm`.

- **Visual Studio:** File → Open → Folder (this folder), then F5.
- **VS Code:** pick the *Debug (MSVC, Ninja)* preset, then Build and Run.
- **Command line** (Developer PowerShell): `cmake --preset debug`, then `cmake --build --preset debug`, then `.\build\debug\A1-lab10nov.exe`

## Methods

| Exercise | Method | What it does |
|---|---|---|
| 1 | `tripleProduct(i, j, k)` | Computes i x (j x k) and j(i . k) - k(i . j), prints both and whether they are equal, and returns `true` or `false`. Called 5 times with `randomVec(1, 5)` |
| 2 | `cameraFrame(view, up)` | Computes and prints v, w and u. If view and up are parallel, it prints an error instead. Called with the 3 cases from the assignment |
| 3 | `rodrigues(v, axis, angle)` | Returns the rotated vector (angle in radians). `testRodrigues` prints the input, the expected output, the result and whether they are equal. Called 3 times |
| 4 | `rodriguesMatrix(axis, angle)` | Returns R = I + sin(t)A + (1 - cos(t))A² (angle in radians). `testRodriguesMatrix` applies it to a vector and prints the input, the expected output and the result. Each axis is rotated by 90° around x, y and z, for 9 tests |
| 5 | `transposeProperty(A, B)` | Computes (AB)^T and B^T A^T, prints both and whether they are equal, and returns `true` or `false`. Called 5 times with `randomMat3(-5, 5)` |
| 6 | `inverseProperty(A, B)` | Computes (AB)^-1 and B^-1 A^-1, prints both and whether they are equal, and returns `true` or `false`. It refuses non-invertible matrices (det = 0). Called 5 times with random invertible `randomMat3(-5, 5)` |

**Helpers:**
- `randomVec(min, max)` and `randomMat3(min, max)` use the same technique as the helpers shown in class (`std::mt19937`). They give whole numbers, so the float results are precise enough for the threshold.
- `matricesEqual(A, B)` compares every entry with `glm::epsilonEqual`.
- Results are printed with `glm::to_string`.

Comparisons use `THRESHOLD = 1.0e-5` and `glm::epsilonEqual`, as the assignment asks.

## Expected outputs

**Exercise 3**

| v | axis | angle | expected |
|---|---|---|---|
| (1, 2, 3) | (2, 2, 2) | 120 | (3, 1, 2) |
| (1, 2, 3) | (1, 1, 1) | 180 | (3, 2, 1) |
| (4, 4, 4) | (3, 3, 3) | 90 | (4, 4, 4) |

**Exercise 4** (all rotations are 90°)

| Rotate around | x axis (1,0,0) | y axis (0,1,0) | z axis (0,0,1) |
|---|---|---|---|
| x | (1, 0, 0) | (0, 0, 1) | (0, -1, 0) |
| y | (0, 0, -1) | (0, 1, 0) | (1, 0, 0) |
| z | (0, 1, 0) | (-1, 0, 0) | (0, 0, 1) |

For a plain explanation of each exercise, see [EXPLANATION.md](EXPLANATION.md).
