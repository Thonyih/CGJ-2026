# Pickagram shark dimensions

The flat shark layout spans approximately **227.90 mm × 123.17 mm** along the axes of square piece 5. These dimensions are calculated from the 89 mm square reference and the flat assembly on printed page 80 (PDF page 85).

## Reference dimensions

| Label | Meaning |
| --- | --- |
| 89 × 89 mm | Front-view size of the reference square formed by all seven pieces. |
| 13 mm | Narrow edge dimension in the side construction of the reference square. |
| 55 mm | Other side dimension shown in the golden-ratio construction for the reference square. |

The maker lists an 89 × 89 mm front and a 13 × 55 mm side. The *How to start playing?* image gives the numbered flat outlines and identifies 3b as the reverse of 3a. The numbered shark uses face 3a. [Pickagram’s product specifications](https://www.wadiz.io/web/store/detail/6203/satisfaction)

The 89 mm reference is the hypotenuse of each large triangle. It equals 2√2 times the side of piece 5, so the side of piece 5 is **89 ÷ (2√2) ≈ 31.47 mm**.

## Piece dimensions in the flat guide

| Piece | Outline | Edge lengths (mm) |
| --- | --- | --- |
| 1 | Large right isosceles triangle | Legs 62.93 each, hypotenuse 89.00 |
| 2 | Large right isosceles triangle | Legs 62.93 each, hypotenuse 89.00 |
| 3a | Parallelogram | Short sides 31.47 each, long sides 44.50 each |
| 4 | Small right isosceles triangle | Legs 31.47 each, hypotenuse 44.50 |
| 5 | Square | Four sides 31.47 |
| 6 | Small right isosceles triangle | Legs 31.47 each, hypotenuse 44.50 |
| 7 | Medium right isosceles triangle | Legs 44.50 each, hypotenuse 62.93 |

The parallelogram has angles of 45° and 135°. The edge lengths describe the flat outlines shown in the guide. The physical pieces include sloped faces. [Pickagram patent US12434133B2](https://patents.google.com/patent/US12434133B2/en#description)

## Shark layout coordinates

The origin is the corner shared by pieces 2, 4, and 5. The positive x-axis follows the upper edge of piece 5 toward the tail. The positive y-axis follows the left edge of piece 5 downward. Vertices are in perimeter order and rounded to 0.01 mm.

| Piece | Vertices (x, y) in mm |
| --- | --- |
| 1 | (-120.47, 31.47), (-75.97, -13.03), (-31.47, 31.47) |
| 2 | (-62.93, 0), (0, 0), (0, 62.93) |
| 3a | (31.47, 0), (62.93, -31.47), (62.93, 0), (31.47, 31.47) |
| 4 | (-31.47, 0), (0, -31.47), (0, 0) |
| 5 | (0, 0), (31.47, 0), (31.47, 31.47), (0, 31.47) |
| 6 | (-75.97, 31.47), (-31.47, 31.47), (-53.72, 53.72) |
| 7 | (62.93, -60.23), (107.43, -15.73), (62.93, -15.73) |

The coordinate bounds are x = -120.47 to 107.43 mm and y = -60.23 to 62.93 mm. The lower shark illustration provides a 3D assembly view. Its overall width, height, and depth would require a measured assembly or additional dimensioned views.

## Blender setup

Use Metric units with Unit Scale 1.0 and display lengths in millimetres. Blender mesh coordinates at this scale are in metres, so multiply the millimetre values by 0.001.

For an upright model facing the Front view (looking from negative Y), map a guide vertex (x, y) in millimetres to Blender coordinates as follows.

```text
u = 89 / (2 × sqrt(2)) mm
X = x / 1000
Y = 0
Z = (2u - y) / 1000
```

This places the outline's lowest point at Z = 0, with the nose on the left and the tail on the right. Keep all seven pieces as separate mesh objects so their seams remain visible. Check the silhouette in Front Orthographic view.

### Exact vertices for the mesh

Use this table for geometry so shared edges line up without rounding. Here r = sqrt(2). Multiply each component by u to obtain the guide coordinates in millimetres before applying the mapping above.

| Piece | Exact guide vertices (x/u, y/u) |
| --- | --- |
| 1 | (-1-2r, 1), (-1-r, 1-r), (-1, 1) |
| 2 | (-2, 0), (0, 0), (0, 2) |
| 3a | (1, 0), (2, -1), (2, 0), (1, 1) |
| 4 | (-1, 0), (0, -1), (0, 0) |
| 5 | (0, 0), (1, 0), (1, 1), (0, 1) |
| 6 | (-1-r, 1), (-1, 1), (-1-r/2, 1+r/2) |
| 7 | (2, -1/2-r), (2+r, -1/2), (2, -1/2) |

### Depth and sloped faces

For a simple solid blockout, a 13 mm placeholder extrusion along Y is one modeling choice. With a centered extrusion, set the front and back to Y = -0.0065 m and +0.0065 m. The resulting blockout measures **227.9 mm in X, 13 mm in Y, and 123.2 mm in Z**.

For a faithful 3D model, use the lower shark illustration to guide the visible slopes and piece placement. Exact reproduction needs the 3D piece geometry and placement, such as measured pieces plus orthographic views of the assembled shark, or original mesh/CAD files.

## Starting square for Blender

The 89 × 89 mm square uses the upper-left corner as its origin, with x to the right and y downward. The table follows the numbered square on PDF page 3. To stand the square on the same floor as the shark, use Blender coordinates X = x / 1000, Y = 0, Z = (89 - y) / 1000.

| Piece | Starting-square vertices (x, y) in mm |
| --- | --- |
| 1 | (0, 0), (44.5, 44.5), (0, 89) |
| 2 | (0, 0), (89, 0), (44.5, 44.5) |
| 3a | (0, 89), (22.25, 66.75), (66.75, 66.75), (44.5, 89) |
| 4 | (22.25, 66.75), (44.5, 44.5), (66.75, 66.75) |
| 5 | (44.5, 44.5), (66.75, 22.25), (89, 44.5), (66.75, 66.75) |
| 6 | (66.75, 22.25), (89, 0), (89, 44.5) |
| 7 | (44.5, 89), (89, 44.5), (89, 89) |

### Pyramid-shaped starting solid

The 89 × 89 mm coordinate table is the front projection of the assembled square. The annotated Measurements.jpeg marks an 89 mm square edge and a 13 mm rim. The depth chain is 21 mm from one peak to its rim face, 13 mm across the rim, and 21 mm from the other rim face to its peak. The total peak-to-peak depth is 55 mm. The pink 34 mm mark reads as a partial depth equal to one 21 mm rise plus the 13 mm rim. [Pickagram’s product specifications](https://www.wadiz.io/web/store/detail/6203/satisfaction)

For an upright Blender model centered in depth, use these positions:

| Feature | Blender position |
| --- | --- |
| Front and rear square rims | Y = -0.0065 m and +0.0065 m |
| Front peak | (0.0445, -0.0275, 0.0445) m |
| Rear peak | (0.0445, +0.0275, 0.0445) m |

To reconstruct the seven pieces from the square, use each polygon in the starting-square table as its projected footprint. For a footprint point (x, y) in millimetres, compute the distance to the nearest square edge and use it to set depth:

~~~text
d = min(x, y, 89 - x, 89 - y)
half_depth = 6.5 + 21 * d / 44.5
X = x / 1000
Z = (89 - y) / 1000
Y_front = -half_depth / 1000
Y_rear  =  half_depth / 1000
~~~

Split the front and rear surfaces wherever a piece crosses either diagonal of the square (x = y or x + y = 89). These diagonals are the creases between the pyramid faces. Add side faces along each piece perimeter and keep the seven solids separate so the same meshes can be moved into the shark. This reconstructs the square's sloped surfaces from the illustrations and nominal dimensions. The annotated depth dimensions fix the rim and peak depths. The mesh reconstruction assumes centered peaks and straight slopes.

### 3D dimensions of the seven pieces

Each tuple below is (x, y, h) in millimetres. The first two coordinates are the starting-square footprint and h is the distance from the midplane to either sloped face. Each tuple gives two vertices. With the square standing upright, Blender coordinates in metres are (x / 1000, -h / 1000, (89 - y) / 1000) for the front and (x / 1000, +h / 1000, (89 - y) / 1000) for the rear. The local front-to-rear depth is 2h.

| Piece | Footprint vertices and half-depth (x, y, h), mm | Local depth range (mm) |
| --- | --- | --- |
| 1 | (0, 0, 6.5), (44.5, 44.5, 27.5), (0, 89, 6.5) | 13–55 |
| 2 | (0, 0, 6.5), (89, 0, 6.5), (44.5, 44.5, 27.5) | 13–55 |
| 3a | (0, 89, 6.5), (22.25, 66.75, 17), (66.75, 66.75, 17), (44.5, 89, 6.5) | 13–34 |
| 4 | (22.25, 66.75, 17), (44.5, 44.5, 27.5), (66.75, 66.75, 17) | 34–55 |
| 5 | (44.5, 44.5, 27.5), (66.75, 22.25, 17), (89, 44.5, 6.5), (66.75, 66.75, 17) | 13–55 |
| 6 | (66.75, 22.25, 17), (89, 0, 6.5), (89, 44.5, 6.5) | 13–34 |
| 7 | (44.5, 89, 6.5), (66.75, 66.75, 17), (89, 44.5, 6.5), (89, 89, 6.5) | 13–34 |

Piece 7 includes an extra vertex at (66.75, 66.75) on its long edge. On each cap, connect that vertex to (89, 89) to form the crease between its two sloped faces. The other six pieces each have one planar front face and one planar rear face in this reconstruction.

These coordinates define the piece meshes in the starting position. To form the 3D shark, move and rotate those meshes as rigid objects. The square depths describe the Y direction in the starting position.

### Face winding

The shark and starting-square polygon tables use clockwise order in the guide view, where y increases downward. With the upright Blender mapping, that order points the face normal toward positive Y. Reverse each row's face indices once if creating a front face that should point toward negative Y. For a solid with caps, use the original order on the positive-Y cap and the reversed order on the negative-Y cap.

## Color image variant

`Shark-Color.jpeg` shows a second flat layout. To match it, add u/2 (approximately 15.73 mm) to the guide y-coordinate of every vertex of piece 7. This moves only the tail fin downward, or along negative Z in Blender. The resulting front silhouette is approximately **227.90 mm × 107.43 mm**. The primary shark tables follow the numbered layout on printed page 80.

### Purple tail alignment

Use the color image for placement and snap the purple triangle (piece 7) to the orange parallelogram (piece 3a). The JPEG shows a slight pixel mismatch at the join. Set the purple triangle's lower-left right-angle corner to the orange piece's lower-right corner at (2u, 0). The purple bottom edge then lines up with the top edge of the green square (piece 5) at y = 0.

| Piece 7 vertex | Exact guide coordinates (x/u, y/u) | Guide coordinates (mm) |
| --- | --- | --- |
| Upper-left tip | (2, -sqrt(2)) | (62.93, -44.50) |
| Right tip | (2 + sqrt(2), 0) | (107.43, 0) |
| Lower-left corner | (2, 0) | (62.93, 0) |

The purple and orange pieces share a vertical edge segment at x = 2u from y = -u to y = 0. The purple tip extends 13.03 mm above that segment. Use the exact snap coordinates for the mesh, keeping the two objects separate.
