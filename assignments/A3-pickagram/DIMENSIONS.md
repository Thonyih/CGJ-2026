# A3 Pickagram: units and real dimensions

## The rule: 1 unit = 89 mm

The real Pickagram pieces put together make a square of **89 × 89 mm**. In our program and in Blender, that whole square is **1 × 1**. So:

| | |
|---|---|
| **units → mm** | multiply by 89 |
| **mm → units** | divide by 89 |

**Why this scale:**
- A2 already used "the Tangram square = 1" (from `Tangram-Stock-Sizing.jpeg`), so every A2 number (1, ½, √2/2, Q) stays the same in A3.
- OpenGL has no real units, so choosing 1 = 89 mm only sets the scale. It changes nothing else.

The millimetre values come from `Docs/Assignments/A3/shark-pickagram-dimensions.md` (the Pickagram book and the maker's specifications).

Two values come up often:
- **Q = √2/4 ≈ 0.354**: the side of the square piece, and the short side of a small triangle.
- **√2/2 ≈ 0.707**: the short side of a large triangle.

## Piece outlines (seen from the front)

| Piece | Side | Units | × 89 = mm |
|---|---|---|---|
| Large triangle (head, body) | long side | 1 | 89.00 |
| | short sides | √2/2 ≈ 0.707 | 62.93 |
| Medium triangle (tail) | long side | √2/2 ≈ 0.707 | 62.93 |
| | short sides | ½ = 0.5 | 44.50 |
| Small triangle (top fin, lower fin) | long side | ½ = 0.5 | 44.50 |
| | short sides | Q ≈ 0.354 | 31.47 |
| Square | side | Q ≈ 0.354 | 31.47 |
| | diagonal | ½ = 0.5 | 44.50 |
| Parallelogram (tail stalk) | long sides | ½ = 0.5 | 44.50 |
| | short sides | Q ≈ 0.354 | 31.47 |

The parallelogram's angles are 45° and 135°.

## Depth: the pyramid shape

Put together, the pieces form a double pyramid: thin at the edge of the square and thick in the middle.

| Where in the square | Depth (front to back) | Units | Half (front or back face) |
|---|---|---|---|
| Edge (rim) | 13 mm | 13 / 89 ≈ 0.146 | 6.5 mm ≈ 0.073 |
| Halfway to the centre | 34 mm | 34 / 89 ≈ 0.382 | 17 mm ≈ 0.191 |
| Centre (peak) | 55 mm | 55 / 89 ≈ 0.618 | 27.5 mm ≈ 0.309 |

In between, the depth changes in a straight line. With **d** = the distance from the point to the nearest edge of the square:

```text
half_depth = 6.5 + 21 × d / 44.5          (mm, with d in mm)
half_depth = 6.5/89 + 0.472 × d           (units, with d in units)
```

At the edge d = 0, so the half-depth is 6.5 mm. At the centre d = 44.5 mm, so it is 6.5 + 21 = 27.5 mm.

**Depth of each piece**, depending on where it sits in the square:

| Piece | Depth range |
|---|---|
| Large triangles, square | 13 → 55 mm (they reach the centre) |
| Small triangle from the centre (top fin) | 34 → 55 mm |
| Small triangle from the corner (lower fin) | 13 → 34 mm |
| Medium triangle, parallelogram | 13 → 34 mm |

## The scene

| What | Units | mm |
|---|---|---|
| Table (width × thickness × depth) | 6.4 × 0.1 × 3.0, top surface at height 0 | 569.6 × 8.9 × 267 |
| Shark, standing (width × height) | 2.56 × 1.21 | 227.9 × 107.4 |
| Shark position on the table | right by 1.2, up by Q ≈ 0.354 (its lowest tip touches the table) | 106.8, 31.47 |
| Box, lying flat | 1 × 1 | 89 × 89 |
| Box position on the table | left by 2.5, up by 0.309 (resting on its back peak) | 222.5, 27.5 |

## Where these numbers are in the files

| Number | `assets/blender/make-pieces.py` | `src/A3-assignment.cpp` |
|---|---|---|
| Rim half-depth 6.5 mm | `RIM = 6.5 / 89.0` | not needed |
| Peak half-depth 27.5 mm | `PEAK = 27.5 / 89.0` | `PEAK = 27.5f / 89.0f` (lifts the box) |
| Piece outlines | `TRIANGLE`, `SQUARE`, `PARALLELOGRAM` (+ the sizes in `PIECES`) | not needed (they are in the meshes) |
| Q, √2 | `math.sqrt(2) / 2` (medium size) | `Q = SQRT2 / 4.0f` (shark positions) |
| Box placements | `PIECES` (x, y, angle) | `BOX` (same numbers) |
| Shark placements | not needed | `SHARK` |
| Table | `make_box("table", -3.2, 3.2, -0.1, 0.0, -1.5, 1.5)` | not needed (it is in the mesh) |
