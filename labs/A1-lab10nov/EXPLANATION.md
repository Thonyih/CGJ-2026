# A1 Exercises 1-6, Explained

## Basics you need

- **Vector:** an arrow in 3D, written as 3 numbers `(x, y, z)`. In GLM: `glm::vec3`.
- **Length** `|v|`: how long the arrow is. `glm::length(v)`.
- **Normalize** `v / |v|`: the same direction, but length 1 (a "unit vector"). `glm::normalize(v)`.
- **Dot product** `a . b`: gives a **number**. If it is 0, the two arrows are at 90° to each other. `glm::dot(a, b)`.
- **Cross product** `a x b`: gives a new **arrow** at 90° to both `a` and `b`. Swapping the order flips it: `b x a = -(a x b)`. `glm::cross(a, b)`.
- **Comparing floats:** computers round decimals slightly, so two results count as equal when they differ by less than `THRESHOLD = 0.00001`. `glm::epsilonEqual` does this comparison.

---

## Exercise 1: Triple product

**The rule:**

    i x (j x k) = j(i . k) - k(i . j)

Two different calculations should give **the same arrow**:

| Left side | Right side |
|---|---|
| 1. cross `j` and `k` | 1. dot `i . k` (a number) and multiply `j` by it |
| 2. cross `i` with that result | 2. dot `i . j` (a number) and multiply `k` by it |
| | 3. subtract the two |

**Why it matters:** it turns two cross products into simpler dot products. The lecture uses it to derive the matrix form of Rodrigues (Exercise 4).

**What the code does** (`tripleProduct`):
1. Computes the left side and the right side.
2. Prints `i`, `j`, `k` and both results.
3. Prints `Equal: yes` if they match, and returns `true` or `false`.

`main` calls it **5 times with random vectors** from `randomVec(1, 5)`, which works the same way as the helper shown in class.

**Why whole numbers 1-5:** with values up to 5 the results reach the hundreds. A `float` keeps only about 7 digits, so decimal inputs would give rounding errors bigger than `THRESHOLD = 0.00001`, and about 1 in 4 **correct** results would print "Equal: no". With whole numbers, every float cross and dot product is **exact** (no rounding), so the comparison always works. Tested over 200 runs: every comparison printed "Equal: yes".

**Example:**

    i = (5, 1, 2)   j = (1, 4, 3)   k = (2, 1, 3)
    i x (j x k)         = (-13, 53, 6)
    j(i . k) - k(i . j) = (-13, 53, 6)
    Equal: yes

---

## Exercise 2: Camera coordinate frame

**The idea:** a camera needs its own 3 axes, all at 90° to each other and all of length 1:

| Axis | Meaning | Formula |
|---|---|---|
| `v` | where the camera looks | `view / |view|` |
| `w` | the camera's "side" | `(up x v) / |up x v|` |
| `u` | the camera's real "up" | `v x w` |

**Step by step:**
1. `v`: make the view direction length 1.
2. `w`: a cross product gives an arrow at 90° to both `up` and `v`, which is the sideways direction. Make it length 1.
3. `u`: a cross product of `v` and `w` gives the third axis, at 90° to both. It is already length 1.

The `up` you give is only a rough hint. The real `u` comes out exactly at 90° to where the camera looks.

**What the code does** (`cameraFrame`): computes and prints `v`, `w`, `u`.

**The 3 tests (up = (0, 1, 0)):**

| view | Result |
|---|---|
| (2, 3, 5) | `v = (0.3244, 0.4867, 0.8111)`, `w = (0.9285, 0, -0.3714)`, `u = (-0.1807, 0.8736, -0.4519)` |
| (3, 2, 5) | `v = (0.4867, 0.3244, 0.8111)`, `w = (0.8575, 0, -0.5145)`, `u = (-0.1669, 0.9459, -0.2782)` |
| (0, 1, 0) | **Error**: the frame cannot be created |

**Why test 3 fails (on purpose):** `view` and `up` point the same way. The cross product of two parallel arrows is `(0, 0, 0)`, so `|up x v| = 0`, and `w` would need a **division by zero**. There is no "sideways" direction when you look straight up. The code checks for this first (`length < THRESHOLD`) and prints an error instead of producing garbage (NaN).

---

## Exercise 3: Rodrigues rotation formula

**The idea:** rotate a vector `v` by an angle `t` around **any** axis `a`, not just x, y or z.

    vrot = v cos(t) + (a x v) sin(t) + a(a . v)(1 - cos(t))

`a` must have length 1, so the code normalizes the axis first.

**What each part does:**

| Part | Meaning |
|---|---|
| `v cos(t)` | shrinks the original vector as it turns |
| `(a x v) sin(t)` | pushes it sideways, around the axis |
| `a(a . v)(1 - cos(t))` | keeps the part of `v` that lies along the axis (that part never moves) |

**What the code does:**
- `rodrigues(v, axis, angle)` returns the rotated vector. The angle is in **radians**, like everywhere else in GLM (for example `glm::rotate`). To use degrees, pass `glm::radians(degrees)`.
- `testRodrigues` prints the input, the **expected** result and the computed result, then `Equal: yes/no`.

**The 3 tests** (inputs chosen so the answer is easy to work out by hand):

| v | axis | angle | expected | Why |
|---|---|---|---|---|
| (1, 2, 3) | (2, 2, 2) | 120° | (3, 1, 2) | A 120° turn around the diagonal moves x → y → z → x, so `(x, y, z)` becomes `(z, x, y)` |
| (1, 2, 3) | (1, 1, 1) | 180° | (3, 2, 1) | With cos = -1 and sin = 0 the formula becomes `-v + 2a(a . v)` = `-(1,2,3) + (4,4,4)` |
| (4, 4, 4) | (3, 3, 3) | 90° | (4, 4, 4) | `v` lies on the axis, so it does not move (`a x v = 0`) |

All three print `Equal: yes`.

---

## Basics for exercises 4-6: matrices

- **Matrix:** a 3x3 grid of numbers (`glm::mat3`). Multiplying a matrix by a vector transforms the vector, for example rotating it.
- **Identity `I`:** 1s on the diagonal and 0s elsewhere. It changes nothing (`glm::mat3(1.0f)`).
- **Matrix product `AB`:** applies `B` first, then `A`. Order matters: usually `AB != BA`.
- **Transpose `A^T`:** rows become columns. `glm::transpose(A)`.
- **Determinant `det(A)`:** one number. If it is 0, the matrix squashes space flat and **cannot be undone**. `glm::determinant(A)`.
- **Inverse `A^-1`:** the matrix that undoes `A` (`A * A^-1 = I`). It only exists if `det(A) != 0`. `glm::inverse(A)`.
- **GLM is column-major:** `A[column][row]`, not `A[row][column]`.
- `glm::to_string(matrix)` prints the matrix **column by column**: each `( )` group is one column.

---

## Exercise 4: Rodrigues rotation formula, matrix form

**The idea:** the same rotation as Exercise 3, but as a **matrix** `R`. Then rotating any vector is just `R * v`.

    R = I + sin(t) A + (1 - cos(t)) A^2

`A` is built from the unit axis `a = (ax, ay, az)`:

          |  0  -az   ay |
      A = |  az   0  -ax |
          | -ay  ax    0 |

`A` is special: `A * v` gives the same result as `a x v` (the cross product). So the matrix formula is just Exercise 3's formula written with matrices. The lecture derives it using the triple product from Exercise 1.

**What the code does:**
- `rodriguesMatrix(axis, angle)` normalizes the axis, fills `A` entry by entry (remembering `A[column][row]`), and returns `I + sin(t) A + (1 - cos(t)) A*A`. The angle is in radians.
- `testRodriguesMatrix` builds `R`, computes `R * v`, and prints the input, the expected result, the computed result and `Equal: yes/no`.

**The 9 tests:** the assignment asks to rotate each axis (x, y, z) by 90° around x, then y, then z.

| Rotate around | Rule | x axis (1,0,0) | y axis (0,1,0) | z axis (0,0,1) |
|---|---|---|---|---|
| x | (x, y, z) → (x, -z, y) | (1, 0, 0) | (0, 0, 1) | (0, -1, 0) |
| y | (x, y, z) → (z, y, -x) | (0, 0, -1) | (0, 1, 0) | (1, 0, 0) |
| z | (x, y, z) → (-y, x, z) | (0, 1, 0) | (-1, 0, 0) | (0, 0, 1) |

An axis rotated around **itself** doesn't move (the diagonal of the table).

---

## Exercise 5: (AB)^T = B^T A^T

**The rule:** transposing a product is the same as multiplying the transposes **in the opposite order**.

**What the code does** (`transposeProperty`):
1. Left side: multiply `A * B`, then transpose.
2. Right side: transpose `B`, transpose `A`, multiply them (`B^T * A^T`).
3. Prints `A`, `B` and both results, plus `Equal: yes/no`, and returns `true` or `false`.

`main` calls it **5 times with random matrices** from `randomMat3(-5, 5)`.

**Why the order flips:** think of putting on socks then shoes. To undo it you take off the shoes first, then the socks. The transpose works the same way.

---

## Exercise 6: (AB)^-1 = B^-1 A^-1

**The rule:** the inverse of a product is the product of the inverses **in the opposite order**, for the same "socks and shoes" reason.

**What the code does** (`inverseProperty`):
1. Prints `A`, `B` and their determinants.
2. If either determinant is 0, it prints an error and stops, because there is no inverse.
3. Left side: `inverse(A * B)`. Right side: `inverse(B) * inverse(A)`.
4. Prints both results and `Equal: yes/no`, and returns `true` or `false`.

**Making sure A and B are invertible:** before calling the method, `main` keeps picking new random matrices until `det != 0`, as the assignment asks.

**Why whole numbers again:** inverses produce fractions (for example `-5.466667`). With random **decimal** matrices, float rounding made about 8 in 100 correct results print "no". With whole numbers it was 3 in about 95,000, and none in 200 full runs of the program.

---

## Checking all six

Over 200 runs of the program (27 comparisons per run), every comparison printed `Equal: yes`.
