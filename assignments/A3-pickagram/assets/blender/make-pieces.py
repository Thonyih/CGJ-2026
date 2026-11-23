# Makes the Pickagram meshes in Blender and exports each one to an OBJ file.
#
# Run it from a terminal (Blender opens without a window, runs this, and quits):
#   blender --background --python make-pieces.py
#
# Output (next to this script):
#   pickagram.blend                  all 4 objects, to open and look at in Blender
#   ../models/triangle.obj           the base right triangle (the LARGE one)
#   ../models/square.obj             the square
#   ../models/parallelogram.obj      the parallelogram
#   ../models/table.obj              the tabletop
#
# Model space (the same as A2, now with a thickness):
#   - The Tangram square is 1 x 1.
#   - Each piece is its A2 shape in the x-y plane, with corner 0 at (0, 0, 0),
#     made THICK along z, from z = 0 to z = THICKNESS.
#   - The table's top surface is at y = 0, so pieces put on it at y = 0 sit on it.
#   - The files are exported with no axis change (forward = Y, up = Z), so the
#     coordinates in the OBJ are exactly the ones written here.

import os
import bpy
import bmesh

THICKNESS = 0.1
HERE = os.path.dirname(os.path.abspath(__file__))
MODELS = os.path.join(HERE, "..", "models")


def make_object(name, vertices, faces):
    """Create a mesh object from a list of corners and faces."""
    mesh = bpy.data.meshes.new(name)
    mesh.from_pydata(vertices, [], faces)
    # Make every face point outwards (needed for back-face culling).
    bm = bmesh.new()
    bm.from_mesh(mesh)
    bmesh.ops.recalc_face_normals(bm, faces=bm.faces)
    bm.to_mesh(mesh)
    bm.free()
    for polygon in mesh.polygons:
        polygon.use_smooth = False  # flat: one normal per face
    obj = bpy.data.objects.new(name, mesh)
    bpy.context.scene.collection.objects.link(obj)
    return obj


def make_prism(name, corners):
    """A flat 2D shape (corners counter-clockwise, in x-y) made thick along z."""
    n = len(corners)
    vertices = [(x, y, 0.0) for (x, y) in corners] + \
               [(x, y, THICKNESS) for (x, y) in corners]
    faces = [list(range(n - 1, -1, -1)),          # back  (z = 0)
             list(range(n, 2 * n))]               # front (z = THICKNESS)
    for i in range(n):                            # the sides
        j = (i + 1) % n
        faces.append([i, j, n + j, n + i])
    return make_object(name, vertices, faces)


def make_box(name, x0, x1, y0, y1, z0, z1):
    vertices = [(x0, y0, z0), (x1, y0, z0), (x1, y1, z0), (x0, y1, z0),
                (x0, y0, z1), (x1, y0, z1), (x1, y1, z1), (x0, y1, z1)]
    faces = [[0, 3, 2, 1], [4, 5, 6, 7], [0, 1, 5, 4],
             [2, 3, 7, 6], [1, 2, 6, 5], [0, 4, 7, 3]]
    return make_object(name, vertices, faces)


def export(obj, filename):
    bpy.ops.object.select_all(action='DESELECT')
    obj.select_set(True)
    bpy.ops.wm.obj_export(filepath=os.path.join(MODELS, filename),
                          export_selected_objects=True,
                          forward_axis='Y', up_axis='Z',
                          export_normals=True, export_uv=False,
                          export_materials=False,
                          export_triangulated_mesh=True)


bpy.ops.wm.read_factory_settings(use_empty=True)
os.makedirs(MODELS, exist_ok=True)

# The same corners as the A2 base shapes (corner 0 first, counter-clockwise).
triangle = make_prism("triangle", [(0.0, 0.0), (1.0, 0.0), (0.5, 0.5)])
square = make_prism("square", [(0.0, 0.0), (0.25, 0.25), (0.0, 0.5), (-0.25, 0.25)])
parallelogram = make_prism("parallelogram",
                           [(0.0, 0.0), (0.5, 0.0), (0.75, 0.25), (0.25, 0.25)])
# The tabletop: 6.4 wide (x), 3 deep (z), 0.1 thick, top surface at y = 0.
table = make_box("table", -3.2, 3.2, -0.1, 0.0, -1.5, 1.5)

export(triangle, "triangle.obj")
export(square, "square.obj")
export(parallelogram, "parallelogram.obj")
export(table, "table.obj")

bpy.ops.wm.save_as_mainfile(filepath=os.path.join(HERE, "pickagram.blend"))
print("Pickagram meshes exported to", os.path.abspath(MODELS))
