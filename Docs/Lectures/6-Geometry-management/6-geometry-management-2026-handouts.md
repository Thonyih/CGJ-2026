# 6. Managing Geometry in Modern OpenGL

[Lecture PDF](6-geometry-management-2026-handouts.pdf) · Carlos Martinho · PDF page numbers below

## Export and load a mesh (pp. 2–3)

The first screenshot pairs a cube in Blender with an editor showing the exported Wavefront OBJ. An OBJ can store positions (`v`), texture coordinates (`vt`), normals (`vn`) and faces (`f`). Faces can reference a separate index for each attribute, commonly written `position/texcoord/normal`. For a triangle-only draw path, triangulate on export or in the importer.

The table on p. 3 shows the stages of packing an OBJ for rendering.

1. Load the independent position, texture-coordinate and normal arrays.
2. Read each face corner’s available indices and look up the attribute values.
3. Produce one packed position/UV/normal record per draw vertex. You can expand the triangles and use `glDrawArrays`, or deduplicate the full index tuples and use `glDrawElements`. A shared position may need multiple records at a UV seam or a hard normal boundary.

## Reuse geometry with a matrix stack (pp. 4–8)

The sample renders show an enclosure with four columns and an articulated machine, viewed at different angles. The environment drawing labels dimensions around a flat ground piece and scaled columns. The machine drawing shows its base, vertical structure and arms. Both assemblies are made from copies of the same cube given different transforms.

The stack diagrams show the active model matrix beside the saved matrices. Push the current matrix before modifying it for a local part. Apply its translation, rotation and scale, draw the mesh, and pop to restore the parent matrix. The environment example restores the identity before placing a sibling. The machine example saves the base transform so its subparts inherit the base pose while each receives its own local scale or offset. In column-vector notation, `world = parentWorld * local`.

## Scene graphs (pp. 9–11)

The two rendered views on p. 9 show the same sample geometry managed by a scene graph. The hierarchy drawing on p. 10 contains branches for the environment (ground plus four columns) and the machine. The machine includes base pieces and nested pivots for the lower arm, upper arm and pincers. The orange transform nodes show where a pose can propagate to descendants. The blue leaves show individual rendered parts. Moving the upper-arm pivot carries its pincers along.

Page 11 sketches an engine structure. The scene graph owns or references the root and a camera. A scene node carries a model matrix, mesh, shader program, callbacks and texture information, and can reference its parent and children. The camera provides view and projection matrices, with a UBO shown as one way to supply them. Traverse from the root, compose each local matrix with its parent's world matrix, and draw any mesh at that node. Transform-only nodes are suitable for pivots.
