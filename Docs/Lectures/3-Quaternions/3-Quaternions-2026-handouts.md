# 3. Quaternions

[Lecture PDF](3-Quaternions-2026-handouts.pdf) · Carlos Martinho · PDF page numbers below

## Why use quaternions? (pp. 2–6)

Three angles can describe a 3D rotation. Classic Euler sequences repeat the first axis at the end, while Tait–Bryan sequences use three distinct axes. Always record the axis sequence and convention. The slide's yaw/pitch/roll diagram uses yaw about y, pitch about z and roll about x. Its table lists several triples for the same turn-around orientation. The circular diagram shows the ambiguity in choosing an interpolation path between endpoint angles. In the Blender image the colored gimbal rings illustrate gimbal lock. Two axes align, leaving only two independent instantaneous rotation axes. Interpolating matrix entries can yield a matrix whose axes are no longer orthonormal.

## From complex numbers to quaternions (pp. 8–21)

A complex number `a + ib` is a point in a two-dimensional plane, with conjugate `a − ib` and magnitude `sqrt(a²+b²)`. The reflection across the real axis in the diagram shows conjugation. Multiplying by `i` maps `(a,b)` to `(−b,a)`, a 90° rotation. Multiplying by `cos θ + i sin θ` rotates through `θ`. The equivalent matrix has rows `(a,−b)` and `(b,a)`.

Hamilton extended the construction to four real components, with basis `1,i,j,k`. The bridge plaque photo gives the rule `i² = j² = k² = ijk = −1`. The circular arrows show `ij=k`, `jk=i`, and `ki=j`. Traversing the products in reverse changes their sign.

Write a quaternion as `q = (s,v)` with scalar `s` and 3D vector `v`. Addition is componentwise. The Hamilton product is

```text
(s,u)(t,v) = (st − u·v, s v + t u + u×v)
```

For pure quaternions the scalar part of the product is the negative dot product, and the vector part is the cross product. Quaternion multiplication is associative and generally order-dependent. Conjugation flips the vector part and reverses products. For nonzero `q`, the inverse is `conjugate(q)/|q|²`. With unit length it is just the conjugate. The matrix representations on p. 21 express the same multiplication through complex 2×2 or real 4×4 matrices.

## Rotation recipes (pp. 23–30)

| Task | Recipe |
| --- | --- |
| Build a rotation from axis and angle | For unit axis `a`, use `q = (cos(θ/2), a sin(θ/2))`. Normalize to correct numerical drift. |
| Rotate a vector | Form the pure quaternion `p = (0,v)` and take the vector part of `q p q^−1`. |
| Compose rotations | Apply `q1` then `q2` using `q2*q1`, under the rotation convention above. |
| Recover axis and angle | With normalized `q=(s,v)`, use `θ = 2 acos(clamp(s,−1,1))` and `a = v/sqrt(1−s²)` when the denominator is large enough. Near identity the axis is arbitrary. |
| Convert to a matrix | Page 28 lists the 3×3 rotation matrix for a unit quaternion. In GLM use `glm::mat3_cast(q)` or `glm::mat4_cast(q)`. |
| Rotate one direction to another | Normalize both inputs, then use their dot product for the angle and normalize their cross product for the axis. Equal directions use identity. Opposite directions need a 180° turn about a chosen perpendicular axis. |

Both `q` and `−q` describe the same orientation. For shortest-path interpolation, flip the second quaternion if their four-component dot product is negative. With angle `α = acos(clamp(dot(q0,q1),−1,1))` between the adjusted unit quaternions, the SLERP weights are `sin((1−t)α)/sin α` and `sin(t α)/sin α`. Near coincident endpoints, normalized linear interpolation avoids division by a small sine.

**Diagram on p. 30.** The straight chord illustrates LERP and can pass inside the unit sphere. The circular arc illustrates SLERP, which stays on the unit sphere and advances at constant angular speed. The drawing is a 2D slice of the four-dimensional quaternion geometry. Page 31 lists further visual resources, including the GuerrillaCG gimbal-lock and 3Blue1Brown quaternion demonstrations.
