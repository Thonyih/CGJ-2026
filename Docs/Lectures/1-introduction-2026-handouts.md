# 1. Introduction to Computer Graphics for Games

[Lecture PDF](1-introduction-2026-handouts.pdf) · Carlos Martinho · PDF page numbers below

## Course goals and practice

The course builds a real-time graphics engine in modern C++, OpenGL and GLSL while explaining the GPU algorithms that make the engine work. The expected foundations are linear algebra, object-oriented programming, C++ and the STL. Image formats and raster algorithms are useful prior knowledge. The grading slide gives the two-person base project 40% and the individual shader project 60%, and lists a threshold above 9.5 for each. See the [quarter planning notes](0-planning-2026.md) for the schedule and report requirements.

### Opening visuals and learning workflow (pp. 2–20)

- **2–5.** The Epic Games student showcase supplies examples of finished student work. The workflow graphics compare development before and after generative AI. A vintage computer and game inset accompany the question of first-pass output versus refinement. The quality graphic draws attention to the human work of analyzing and evaluating a result.
- **6–10.** A Steam Next Fest article, a cloning-video thumbnail and the Atari console/cartridge photos frame concerns about low-effort or derivative releases. SimCity and the globe graphic introduce the piracy discussion. The Valve/Steam image shifts the discussion to storefront treatment of AI-generated releases.
- **11–14.** A neon playtest sign emphasizes trying the game. Console controllers and a mock “SteamStation” illustrate the platform discussion. The car buffer makes polishing literal, as an analogy for refinement after a first pass.
- **16–20.** The learning flow shows hands-on work and feedback leading to skill, or AI-produced work requiring the learner to spot issues, explain them and use feedback to develop judgment. The drills compare a manual attempt with an AI-assisted one and ask the learner to identify weaknesses in both. The chalkboard names the course’s project-based approach.

### References and applications (pp. 29–53)

- **29–33.** Book covers group engine architecture, real-time rendering, OpenGL, GPU programming and C++ references. The website screenshots introduce online libraries and creative-coding tools including openFrameworks, Cinder and Processing.
- **35–36.** The uncanny-valley chart plots familiarity against human likeness. Familiarity dips for near-human examples before recovering for a healthy human, with the dashed moving curve showing a deeper dip. The film posters illustrate increasing use of CG across live-action effects and animated films.
- **37–43.** The Toy Story and Kingdom Hearts comparison places film imagery next to in-game character rendering. BioShock 2 illustrates an atmospheric in-game scene. The Last Remnant and Borderlands comparisons give examples of visual changes across a remaster and sequels. Shadow Warrior 2 illustrates varying shading resolution. The Metal Gear Solid 2 comparison shows original and upscaled footage. Cyberpunk 2077 illustrates a rendering/reconstruction pipeline and its performance impact in the demo.
- **44–45.** A Steam revenue headline and charts of play and revenue rankings motivate the significance of games as an application of graphics.
- **47–49.** Early computers, speculative interfaces and media-platform logos trace a move from text output to interactive images and video. Maps, virtual homes and social worlds illustrate digital twins and immersive spaces. The sports overlay, portrait pair and before/after examples show how compositing and processing can change the information or impression an image conveys.
- **50–53.** The iPhone launch and portrait comparisons show capture as a computational process. Lens flare appears both as an optical artifact and as an intentional visual effect. The future-of-media video still shows a person interacting with a surface or device. The Unreal Paris room demonstrates architectural visualization with a game engine.

## How rendering techniques developed

| PDF pages | Visual explanation |
| --- | --- |
| 55–61 | The SAGE radar photos show an early graphical system, and the Manchester Mark I image shows text-oriented output. Early GUI screenshots introduce visual controls. The Sketchpad still shows a light pen drawing directly on a screen. The following clip accompanies that history. SuperPaint screenshots illustrate framebuffer-based digital painting. |
| 63–65 | Teapot, bunny, dragon and digitization examples show ways to obtain 3D models. The wireframe room shows the geometric edges. Hidden-line and hidden-surface methods determine which edges and surfaces are visible. |
| 66–71, 73 | The same still life progresses from constant colors to flat face shading, smooth diffuse shading, specular highlights, curved surfaces and textures, then shadow and antialiasing improvements. Gouraud shading interpolates lighting evaluated at vertices. A Phong-style specular term creates concentrated view-dependent highlights. |
| 72 | The Z-buffer sequence compares each incoming fragment depth against the stored depth. Passing fragments can update color and depth. The grayscale image visualizes per-pixel depth in the example scene. |
| 75–80 | Rays leave the eye through the image plane and can spawn shadow, reflection and refraction rays at a hit. Checkerboards and glassy spheres make reflected or bent rays visible. The demo scene and the UE4 helmets show newer uses, especially reflections. |
| 81–85 | The colored room and architectural interiors illustrate radiosity. Surfaces are divided into patches that exchange diffuse light. The patch sequence shows indirect illumination reaching a region that was initially dark. The numbered frames show the distribution evolving as more iterations run. |
| 86–89, 91 | The rendering equation expresses outgoing radiance as emission plus reflected incoming light integrated over directions. Path tracing estimates light transport by sampling paths. The gathering/shooting diagrams trace from the eye or light, and bidirectional tracing connects subpaths built from both ends. The dashed next-event estimation rays test a direct connection to a light from a path vertex. |
| 90, 92–94 | The car demo shows early real-time path tracing. The tiger frames combine path tracing and denoising. The game frame on p. 94 is presented as an example of path tracing paired with reconstruction. |

## Project visuals (pp. 95–105)

The calendar and tables repeat the quarter schedule. The Tangram and Pickagram drawings explain reusing pieces in 2D and composing them in 3D. The material spheres preview the shader project. The final book and IDE images accompany the choice of a C++/GLSL development environment.
