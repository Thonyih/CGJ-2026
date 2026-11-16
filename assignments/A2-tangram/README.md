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
| All 5 triangles use the same base right triangle | The 5 triangle pieces all point to `Triangle`, with scale 1 (small), √2 (medium) or 2 (large) |
| Only triangles; vertices given; right-hand rule | Square and parallelogram are 2 triangles each, and every triangle is counter-clockwise. Back faces are culled, so a wrong order would make a piece disappear |
| Placed with scale, rotation and translation matrices | `modelMatrix()`: `T * R * S` with `glm::translate`, `glm::rotate`, `glm::scale` |
| Model matrix sent as a uniform | Uniform `Matrix` in `tangram-vs.glsl` |
| Clip space, z = 0, x and y in [-1, 1] | All vertices have z = 0. `SCENE` centres the shark and scales it by 0.25 (x within ±0.91, y within ±0.43) |
| Simple class for drawing | `Shape` (buffers + draw) and `Piece` (shape + colour + model matrix + draw) |
| Drawing order doesn't matter | Each `Piece::draw` sets its own matrix and colour. The pieces don't overlap |
| Unique colour per piece | Uniform `Color` in `tangram-fs.glsl`, so one base shape can have any colour. The colours match the assignment's Tangram pictures |

## The measurements

The unit is the **short side of the small triangle = 1**. Then:

| Piece | Base shape | Size |
|---|---|---|
| small triangle | right triangle | legs 1 |
| medium triangle | right triangle | legs √2 |
| large triangle | right triangle | legs 2 |
| square | square | side 1 |
| parallelogram | parallelogram | sides 1 and √2 |

Each base shape has a corner at (0, 0): the right angle, for the triangle. So `translate(x, y)` puts that corner at (x, y). The green square is at (0, 0).

| Piece | Colour | Corner (x, y) | Rotation | Scale |
|---|---|---|---|---|
| Head, large triangle | blue | (−1−√2, √2) | 225° | 2 |
| Body, large triangle | magenta | (0, 1) | 180° | 2 |
| Top fin, small triangle | teal | (0, 1) | 90° | 1 |
| Lower fin, small triangle | red | (−1−√2/2, −√2/2) | 45° | 1 |
| Body, square | green | (0, 0) | 0° | 1 |
| Tail stalk, parallelogram | orange | (1, 0) | 0° | 1 |
| Tail, medium triangle | purple | (2, 1) | 0° | √2 |

These positions were measured from the coloured shark picture, where 1 unit is about 168 pixels.

## Files

| File | Contents |
|---|---|
| `src/tangram.cpp` | The whole program |
| `src/tangram-vs.glsl` | Vertex shader: `Matrix * inPosition` |
| `src/tangram-fs.glsl` | Fragment shader: paints the `Color` uniform |
| `mgl/` | The teacher's framework, unchanged |
| `libraries/` | GLM, GLFW, GLEW |
| `A2-tangram.sln`, `A2-tangram.vcxproj`, `.vcxproj.filters` | Visual Studio solution and project |
