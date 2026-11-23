# 4. Modern OpenGL Rendering Pipeline

[Lecture PDF](4-rendering-pipeline-2026-handouts.pdf) · Carlos Martinho · PDF page numbers below

## From a draw call to the framebuffer (pp. 2–4)

The big pipeline diagram separates application work, geometry processing, and rasterization with per-sample operations. Follow it in draw order.

1. The CPU issues a draw with a shader program and vertex data. Vertex fetch reads the enabled attributes. The vertex shader transforms each input vertex and outputs its clip position in `gl_Position`.
2. Optional tessellation control and evaluation stages surround the fixed-function tessellator, which subdivides patches. An optional geometry shader can emit primitives from its inputs. A basic draw uses vertex and fragment shaders.
3. Primitive assembly groups vertices into points, lines or triangles. Primitives are clipped in homogeneous coordinates. Perspective division and the viewport transform map surviving geometry to window coordinates. Rasterization determines covered samples and interpolates the vertex outputs.
4. The fragment shader produces color outputs and can affect fragment depth or discard a fragment. Per-sample tests determine whether samples contribute. Blending can combine a passing color with the framebuffer value.

The GLSL history table starts at OpenGL 2.0 and GLSL 1.10 for vertex and fragment shaders. It highlights geometry shaders in the OpenGL 3.3 generation and tessellation in 4.0, and ends with OpenGL 4.6. Pick a shader language version supported by the context and the features used by the program.

## Drawing directly in clip space (pp. 5–7)

The cube labeled `−1` to `+1` depicts the normalized volume. In homogeneous clip coordinates, the default bounds are `−w ≤ x,y,z ≤ w`. After dividing by `w`, each normalized component lies in `[−1,+1]`. In the 2D example `w=1`, so the clip and normalized coordinate values coincide.

The blue triangle diagram shows a single shape drawn twice. The identity instance stays in the positive quadrant. The second matrix translates it by `(−1,−1,0)` into the negative quadrant. A simple GLSL vertex transform is `gl_Position = model * vec4(position, 1.0)` when the model output is already suitable as clip coordinates. This is the basis for reusing geometry among the Tangram pieces. The title art shows an assembled colorful silhouette and a stylized character beneath it.

## Getting a frame to the monitor (pp. 8–15)

| PDF page | Diagram | What it means |
| --- | --- | --- |
| 9 | One framebuffer between renderer and monitor | Drawing and scanout share storage. The monitor can read while the displayed data is being changed. |
| 10 | Back buffer and front buffer with a racing screenshot | The renderer works off-screen. A swap during scanout can show parts of two frames separated by a visible tear. |
| 11 | V-sync between the two buffers | Present at a refresh boundary to avoid tearing. Missing a refresh can repeat a frame and make motion uneven. |
| 12 | G-Sync / FreeSync monitor | A compatible variable-refresh display can align refresh timing with arriving frames within its operating range. |
| 13 | Two back buffers and one front buffer | An extra back buffer can let rendering continue while a completed frame waits for presentation. Actual latency depends on how frames are queued. |
| 14 | Fast Sync / Enhanced Sync with a last-rendered slot | The staged diagram shows render completion and monitor refresh as separate events. A completed frame can wait until refresh, and newer frames can supersede older candidates depending on the mode. |
| 15 | Game artwork with a 30 FPS callout | The image prompts a discussion of game frame-rate targets. A 30 FPS target budgets about 33.3 ms per frame. |
