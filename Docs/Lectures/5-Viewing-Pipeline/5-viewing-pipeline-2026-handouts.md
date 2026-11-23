# 5. Viewing in Modern OpenGL

[Lecture PDF](5-viewing-pipeline-2026-handouts.pdf) · Carlos Martinho · PDF page numbers below

## Coordinate frames and the viewing pipeline (pp. 3–7)

A transformation can move a point in a fixed frame or change the frame used to describe a fixed point. The translation sketch shows that moving a point left by one has the same coordinate effect as shifting the origin right by one. The rotation sketch similarly pairs rotating the point with rotating the coordinate axes in the opposite direction. The two-frame example gives the same point world coordinates `(2,1)` and eye coordinates `(−2,0)` in the illustrated 2D frames.

The colored pipeline diagram shows tangent, model, world, eye, clip and screen spaces. TBN converts tangent-space components using the tangent, bitangent and normal basis. For ordinary vertex positions using column vectors,

```glsl
gl_Position = projection * view * model * vec4(position, 1.0);
```

Model places the object in the world, view expresses it in camera coordinates, and projection produces clip coordinates. Clipping precedes the divide by `w`. The viewport then maps normalized device coordinates to window coordinates.

## View matrix and look-at camera (pp. 8–15)

Given eye `e`, target `c` and up hint `up`, construct:

```text
f = normalize(c - e)
s = normalize(cross(f, up))
u = cross(s, f)
```

The diagrams first build these axes, then align them with camera right, camera up, and forward along negative z, and place the eye at the origin. The final matrix is `R * T(-e)`. Its rows are:

```text
s.x   s.y   s.z   -dot(s,e)
u.x   u.y   u.z   -dot(u,e)
-f.x  -f.y  -f.z   dot(f,e)
0     0     0      1
```

Pages 13–15 contrast views from `(5,5,5)` and `(−5,−5,−5)) toward the origin. In either case the target is `sqrt(75)` units in front of the eye, so its eye-space z is about `−8.66`. An eye equal to its target or an up hint parallel to forward cannot define this basis. In GLM use `glm::lookAt(eye, center, up)` with the right-handed convention used here.

## Orthographic projection (pp. 17–22)

The projection-family tree compares parallel projections with perspective projections. In the orthographic illustration, rays are parallel and objects keep the same projected size at different depths. The boxed diagrams first center the view box, then scale each dimension into the normalized cube. For left/right `l,r`, bottom/top `b,t` and near/far distances `n,f`, visible eye-space z spans `−f` to `−n`. Use the following matrix:

```text
2/(r-l)   0         0          -(r+l)/(r-l)
0         2/(t-b)   0          -(t+b)/(t-b)
0         0        -2/(f-n)    -(f+n)/(f-n)
0         0         0           1
```

Page 22 uses symmetric x and y bounds of ±2 with near/far distances 1 and 10. The resulting x and y scale is 0.5.

## Perspective projection (pp. 23–31)

The projection rays converge at the eye. The side-view drawing uses similar triangles to show that projected x and y shrink as distance `−z` grows. The red cars illustrate the difference in apparent size inside the frustum. Its vertical half-angle is `fovy/2`, and `aspect = width / height`.

Let `k = 1/tan(fovy/2)` and `0 < n < f`. For the default OpenGL depth convention, the lecture’s perspective matrix is:

```text
k/aspect   0   0               0
0          k   0               0
0          0   (f+n)/(n-f)     (2*f*n)/(n-f)
0          0   -1              0
```

The bottom row makes `w_clip = −z_eye`. The third row maps the near and far planes to normalized z of −1 and +1 after the divide. Page 31 illustrates a 30° vertical field of view, 640×480 aspect ratio and near/far 1/10. Pass radians to `glm::perspective` and use floating-point aspect-ratio division.

## Cube example and shared uniforms (pp. 32–34)

The exploded cube view labels eight corners and shows colored faces with diagonals indicating their triangle splits. The camera targets the cube at the origin. The UBO diagram shows two shader programs mapping their camera uniform blocks to the same binding point in the OpenGL context. One buffer supplies the three example `vec4` values for eye, center and up. Programs sharing a buffer need a compatible block layout and matching binding-point setup.
