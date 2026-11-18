# Assignment A2 - 2D Tangram: the Shark

Draws the Shark figure from the Pickagram book with the 7 Tangram pieces, directly in clip space. It is built on the Lecture 4 `hello-2d-world` code, and the teacher's `mgl` framework is used **unchanged**.

## Run (Visual Studio Community 2022 or 2026)

1. Open **`A2-tangram.sln`**.
2. Press **F5**.

That's all. You need the *Desktop development with C++* workload, and nothing else has to be installed.

**What the project does for you:**
- **Libraries:** they are in `libraries\` (GLEW 2.2.0, GLFW 3.4, GLM 1.0.1). GLEW and GLFW are compiled from source with the program, so there are no `.lib` or `.dll` files to set up or copy.
- **Paths:** they are all relative (`$(SolutionDir)libraries\...`), so the folder works wherever it is unzipped.
- **Compiler:** the project uses the compiler of whichever Visual Studio opens it (`$(DefaultPlatformToolset)`), so no retargeting is needed.
- **Working folder:** F5 runs the program from `src\`, where the shader files are. Each build also copies the shaders next to the `.exe` in `build\Debug\` or `build\Release\`.
- **Build output:** everything the build creates goes into `build\`. Delete `build\` and `.vs\` before zipping.

## How each requirement is met

| Requirement | Where, in `src/tangram.cpp` |
|---|---|
| Start from hello-2d-world | Same structure (`MyApp`, `createBufferObjects`, `createShaderProgram`, `drawScene`) and the same `mgl` |
| 7 pieces: 1 parallelogram, 1 square, 2 small, 1 medium, 2 large triangles | `createPieces()` |
| 3 base shapes as buffer objects | `Triangle`, `Square`, `Parallelogram`: one VAO + VBO + EBO each (`Shape::create`) |
| All 5 triangles use the same base right triangle | The 5 triangle pieces all point to `Triangle` (the large triangle), with scale 1 (large), √2/2 (medium) or ½ (small) |
| Only triangles; vertices given; right-hand rule | Square and parallelogram are 2 triangles each, and every triangle is counter-clockwise. Back faces are culled, so a wrong order would make a piece disappear |
| Placed with scale, rotation and translation matrices | `modelMatrix()`: `T * R * S` with `glm::translate`, `glm::rotate`, `glm::scale` |
| Model matrix sent as a uniform | Uniform `Matrix` in `tangram-vs.glsl` |
| Clip space, z = 0, x and y in [-1, 1] | All vertices have z = 0. `SCENE` centres the shark and scales it by 0.7 (x within ±0.90, y within ±0.42) |
| Simple class for drawing | `Shape` (buffers + draw) and `Piece` (shape + colour + model matrix + draw) |
| Drawing order doesn't matter | Each `Piece::draw` sets its own matrix and colour. The pieces don't overlap |
| Unique colour per piece | Uniform `Color` in `tangram-fs.glsl`, so one base shape can have any colour. The colours match the assignment's Tangram pictures |

## Seeing each step: the `STEP` setting

Near the top of `src/tangram.cpp`, in the box marked **SETTING YOU CAN CHANGE**:

```cpp
const int STEP = 3; // <-- CHANGE THIS NUMBER: 0, 1, 2 or 3
```

Change the number, then stop (Shift+F5) and start (F5) the program:

| STEP | What you see |
|---|---|
| 0 | The base shapes exactly as written in the code, with no matrix: every piece starts at (0, 0) |
| 1 | + scale: each piece has its size |
| 2 | + rotate: each piece turned around its corner 0 |
| 3 | + translate: the finished shark (**keep this for the delivery**) |

Steps 0–2 draw outlines only, so pieces lying on top of each other stay visible, and keep (0, 0) in the middle of the window.

## Resizing the window

mgl sets the viewport to the whole window when it is resized, which would stretch the square clip space and distort the shark. `MyApp::windowSizeCallback` then sets the viewport to the biggest square that fits, centred, so the shark keeps its shape at any window size.

## The measurements

The sizes follow the assignment's sizing picture (`Tangram-Stock-Sizing.jpeg`): **the whole Tangram square is 1 × 1**. The large triangle's long side is 1, the small triangle's long side is ½, and the medium triangle's short sides are ½.

| Piece | Base shape | Size | Scale |
|---|---|---|---|
| large triangle | right triangle | long side 1, short sides √2/2 | 1 |
| medium triangle | right triangle | long side √2/2, short sides ½ | √2/2 |
| small triangle | right triangle | long side ½, short sides √2/4 | ½ |
| square | square | diagonal ½, side √2/4 | 1 |
| parallelogram | parallelogram | sides ½ and √2/4 | 1 |

Each base shape is created the way it sits in the stock square. One rule for all three: **corner 0 is at (0, 0)**, and the other corners follow counter-clockwise. Rotate turns the shape around corner 0, and `translate(x, y)` puts corner 0 at (x, y).

| Base shape | Vertices (corner 0 first) | Corner 0 is |
|---|---|---|
| right triangle (large) | (0, 0), (1, 0), (½, ½) | left end of the long side |
| square (diamond) | (0, 0), (¼, ¼), (0, ½), (−¼, ¼) | bottom corner |
| parallelogram | (0, 0), (½, 0), (¾, ¼), (¼, ¼) | bottom-left corner |

Placement, with Q = √2/4 (the short side of a small triangle). The shark's origin is the head's right corner, where the head, the lower fin and the body meet. Angles are counter-clockwise.

| Piece | Colour | Corner 0 goes to | Rotation | Scale |
|---|---|---|---|---|
| Head, large triangle | blue | (−1, 0) | 0° | 1 |
| Body, large triangle | magenta | (−Q, Q) | −45° | 1 |
| Top fin, small triangle | teal | (Q, 2Q) | −135° | ½ |
| Lower fin, small triangle | red | (0, 0) | 180° | ½ |
| Body, square | green | (2Q, 0) | 45° | 1 |
| Tail stalk, parallelogram | orange | (2Q, 0) | 45° | 1 |
| Tail, medium triangle | purple | (3Q + ½, Q) | 135° | √2/2 |

The shape was measured from the coloured shark picture, and each placement was checked by computing the transformed vertices by hand.

## Files

| File | Contents |
|---|---|
| `src/tangram.cpp` | The whole program |
| `src/tangram-vs.glsl` | Vertex shader: `Matrix * inPosition` |
| `src/tangram-fs.glsl` | Fragment shader: paints the `Color` uniform |
| `mgl/` | The teacher's framework, unchanged |
| `libraries/` | GLM, GLFW, GLEW |
| `A2-tangram.sln`, `A2-tangram.vcxproj`, `.vcxproj.filters` | Visual Studio solution and project |
