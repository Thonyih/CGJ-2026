# 2. Mathematics for Computer Graphics

[Lecture PDF](2-Math4CG-2026-handouts.pdf) · Carlos Martinho · PDF page numbers below

## Part I · Vectors and geometry (pp. 2–16)

- Scalars have addition and multiplication with identities, distributivity and inverses (a multiplicative inverse requires a nonzero scalar). A vector space supports addition of vectors and multiplication by scalars. A basis is a linearly independent spanning set. Its size is the dimension of the space.
- In an affine space, subtract points to get a displacement: `v = P - Q`. Add a displacement to a point to get another point: `P = Q + v`. An origin and a basis define a coordinate frame. Euclidean geometry adds distance and angle. The lecture uses right-handed Cartesian 3D coordinates.
- The dot product is `u·v = Σ u_i v_i = |u||v| cos θ`. It gives squared length when a vector is dotted with itself. For nonzero vectors, zero dot product means perpendicular. The projection onto nonzero `v` is `v (u·v)/(v·v)`. The remaining component is perpendicular to the line, and its length is the distance for the point-to-line construction.
- The cross product is perpendicular to its inputs and has magnitude `|u||v| sin θ`. In a right-handed frame, `x × y = z`. Reversing the inputs reverses the result. For noncollinear plane points, compute `n = (P1-P0) × (P2-P0)`, then test coplanarity with `n·(P-P0) = 0`.
- An orthonormal frame has perpendicular unit axes. The slide constructs one from a view vector and an up hint. Normalize the view, use a cross product to obtain a perpendicular axis, then cross again to obtain the missing axis. A parallel up hint needs a different choice.

### Reading the diagrams

- **3, 5–6.** Head-to-tail arrows display vector addition. Point labels anchor displacements in affine space. The illustrated hand shows the orientation of the Cartesian axes.
- **8–11.** Two arrows define the dot-product angle. The right-angle diagram separates the component along a line from the shortest segment to the point. Quadrance is squared length, so the Pythagorean relation for a right triangle leads to the zero dot product condition.
- **13–16.** The cross-product arrows stick out of the parallelogram on opposite sides when input order is swapped. The three-point diagram constructs a plane normal. The camera sketch converts a view direction and an up hint into three perpendicular axes.

## Part II · Matrices and transformations (pp. 18–43)

With column vectors, a matrix acts as `p' = M p`. A product of an `m × n` and an `n × k` matrix gives an `m × k` matrix. Multiplication is associative and generally order-dependent. The identity leaves coordinates unchanged. Transposition reverses a product, `(AB)^T = B^T A^T`, and inversion does the same for invertible factors, `(AB)^−1 = B^−1 A^−1`.

| Operation | Formula or rule | Diagram and pages |
| --- | --- | --- |
| Reflection | `(x,y) → (−x,y)` | The point crosses the vertical axis while keeping its height (20). |
| Scale | `(x,y) → (sx*x, sy*y)` | The dotted guides show each output component stretching by its own factor (25). |
| Shear in x | `(x,y) → (x+a*y,y)` | An upright shape slants because horizontal shift grows with height (26). |
| Planar rotation | `(x,y) → (cx−sy,sx+cy)`, with `c = cos θ` and `s = sin θ` | The point moves along a circular arc at fixed radius. The columns are the rotated basis axes and the inverse is the transpose (27–29). |
| 3D rotation | Use rotations about Cartesian axes or a unit arbitrary axis | The coordinate-frame rows take dot products with the input. The dice end at different orientations when x/y rotations are swapped (31–33). |
| Arbitrary-axis rotation | `v' = v cos θ + (a × v) sin θ + a(a·v)(1−cos θ)` | The side view decomposes the input into axial and perpendicular parts. The top view rotates the perpendicular part on a circle (34–37). |

For the Rodrigues matrix form, choose the skew-symmetric matrix `A` such that `Av = a × v`. Then `R = I + sin(θ) A + (1−cos θ) A²`.

The transformation game (p. 24) illustrates how matrix operations change a shape’s pose. In homogeneous coordinates, represent a finite point as `(x,y,z,1)` and a direction as `(x,y,z,0)`. For a finite homogeneous value with nonzero `w`, recover Cartesian coordinates by dividing by `w`. The highlighted translation column on pp. 40–41 adds an offset for points and contributes zero for directions. Scale, rotate and translate through `M = T R S`, evaluating the rightmost factor first. Swapping `T` and `R` rotates the translation vector as well (p. 42).

## Surface normals and matrix inversion (pp. 45–50)

The shear sketch shows why the transformed normal must still be perpendicular to all transformed tangent vectors. If the linear part is `L` and `t' = L t`, use `n' = (L^−1)^T n` and renormalize. Then `n'·t' = n·t`. In GLM use `glm::transpose(glm::inverse(glm::mat3(model)))` for a world-space normal, assuming an invertible model transform.

For a 2×2 matrix with rows `(a,b)`, `(c,d)`, the determinant is `ad−bc`. The inverse is the matrix with rows `(d,−b)`, `(−c,a)`, divided by the determinant. The 3×3 example uses the first-row sign pattern `+, −, +` and computes determinant `−24 + 40 − 15 = 1`. The last two slides form the signed minors of the transpose to obtain the adjugate, then divide by the determinant. A zero determinant indicates a singular matrix.
