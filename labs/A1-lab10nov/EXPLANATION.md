# A1 Exercises 1-3, Explained

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
3. Prints `Equal: yes` if they match.

`main` calls it **5 times with random vectors**, as the assignment asks.

**Why whole numbers 1-5:** with whole numbers, every cross and dot product is exact (no rounding), so the very strict `THRESHOLD` never fails by accident.

**Example:**

    i = (4, 4, 2)   j = (3, 5, 2)   k = (1, 3, 2)
    i x (j x k)         = (24, -8, -32)
    j(i . k) - k(i . j) = (24, -8, -32)
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
- `rodrigues(v, axis, degrees)` returns the rotated vector. It converts degrees to radians first, because `cos` and `sin` use radians.
- `testRodrigues` prints the input, the **expected** result and the computed result, then `Equal: yes/no`.

**The 3 tests** (inputs chosen so the answer is easy to work out by hand):

| v | axis | angle | expected | Why |
|---|---|---|---|---|
| (1, 2, 3) | (2, 2, 2) | 120° | (3, 1, 2) | A 120° turn around the diagonal moves x → y → z → x, so `(x, y, z)` becomes `(z, x, y)` |
| (1, 2, 3) | (1, 1, 1) | 180° | (3, 2, 1) | With cos = -1 and sin = 0 the formula becomes `-v + 2a(a . v)` = `-(1,2,3) + (4,4,4)` |
| (4, 4, 4) | (3, 3, 3) | 90° | (4, 4, 4) | `v` lies on the axis, so it does not move (`a x v = 0`) |

All three print `Equal: yes`.
