# Makes the Pickagram meshes in Blender and exports each one to an OBJ file.
#
# Run it from a terminal (Blender opens without a window, runs this, and quits):
#   blender --background --python make-pieces.py
#
# THE REAL PICKAGRAM (Docs/Assignments/A3/shark-pickagram-dimensions.md)
#   The 7 pieces put together make an 89 x 89 mm square. Seen from the side,
#   that square is a double pyramid: the outer rim is 13 mm thick and the front
#   and back surfaces rise in straight slopes to a peak in the middle, where it
#   is 55 mm thick. The creases between the 4 slopes are the 2 diagonals.
#   Here the square is 1 x 1, so every millimetre is divided by 89.
#
# HOW A PIECE IS MADE
#   1. Its outline in the square (from its BOX placement, the same table as the
#      C++ code) is cut along the 2 diagonals, so each part lies on one slope.
#   2. Front surface: z = +half_depth(x, y). Back surface: z = -half_depth(x, y).
#      half_depth grows in a straight line from the rim to the peak.
#   3. Side walls join the front and back along the outline.
#   4. The piece is then moved into its own model space, the same as in A2:
#      corner 0 at (0, 0, 0), the outline in the x-y plane, depth along z
#      (centred on z = 0).
#
# Because each piece sits in a different place on the pyramid, pieces of the
# same outline can have different slopes (the 2 small triangles differ). Only
# the 2 large triangles are the same shape, so there are 6 meshes for 7 pieces.
#
# Output:
#   ../models/large.obj          large triangle (head and body)
#   ../models/medium.obj         medium triangle (tail)
#   ../models/small-center.obj   small triangle from the centre (top fin)
#   ../models/small-corner.obj   small triangle from the corner (lower fin)
#   ../models/square.obj         square
#   ../models/parallelogram.obj  parallelogram
#   ../models/table.obj          the tabletop (top surface at y = 0)
#   pickagram.blend              all of them, to open and look at in Blender
# The files are exported with no axis change (forward = Y, up = Z), so the
# coordinates in the OBJ are exactly the ones made here.

import os
import math
import bpy
import bmesh

RIM = 6.5 / 89.0               # half of the 13 mm rim
PEAK = 27.5 / 89.0             # half of the 55 mm at the centre
SLOPE = (PEAK - RIM) / 0.5     # change of half-depth per unit towards the centre

HERE = os.path.dirname(os.path.abspath(__file__))
MODELS = os.path.join(HERE, "..", "models")

# The 3 outlines, as in A2 (corner 0 first, counter-clockwise).
TRIANGLE = [(0.0, 0.0), (1.0, 0.0), (0.5, 0.5)]
SQUARE = [(0.0, 0.0), (0.25, 0.25), (0.0, 0.5), (-0.25, 0.25)]
PARALLELOGRAM = [(0.0, 0.0), (0.5, 0.0), (0.75, 0.25), (0.25, 0.25)]

# Each mesh: outline, size, and where it sits in the box (x, y, angle) -
# the same numbers as the BOX table in src/A3-assignment.cpp.
PIECES = [
    ("large",         TRIANGLE,      1.0,               1.0,  1.0,  180.0),  # blue (top)
    ("small-center",  TRIANGLE,      0.5,               0.25, 0.25,   0.0),  # teal
    ("small-corner",  TRIANGLE,      0.5,               1.0,  0.5,   90.0),  # red
    ("square",        SQUARE,        1.0,               0.75, 0.25,   0.0),  # green
    ("parallelogram", PARALLELOGRAM, 1.0,               0.0,  0.0,    0.0),  # orange
    ("medium",        TRIANGLE,      math.sqrt(2) / 2,  1.0,  0.5,  225.0),  # purple
]


def half_depth(x, y):
    d = min(x, y, 1.0 - x, 1.0 - y)  # distance to the nearest edge of the square
    return RIM + SLOPE * d


EPS = 1e-9  # values this close to 0 count as 0 (rounding noise)


def crosses(fp, fq):
    """True if the values have clearly different signs (the edge crosses the line)."""
    return (fp > EPS and fq < -EPS) or (fp < -EPS and fq > EPS)


def clip(polygon, a, b, c):
    """Keep the part of a convex polygon where a*x + b*y + c >= 0."""
    result = []
    for i in range(len(polygon)):
        p, q = polygon[i], polygon[(i + 1) % len(polygon)]
        fp = a * p[0] + b * p[1] + c
        fq = a * q[0] + b * q[1] + c
        if fp >= -EPS:
            result.append(p)
        if crosses(fp, fq):
            t = fp / (fp - fq)
            result.append((p[0] + t * (q[0] - p[0]), p[1] + t * (q[1] - p[1])))
    return result


def area(polygon):
    s = 0.0
    for i in range(len(polygon)):
        p, q = polygon[i], polygon[(i + 1) % len(polygon)]
        s += p[0] * q[1] - q[0] * p[1]
    return s / 2.0


# The 4 slopes of the pyramid (between the 2 diagonals x = y and x + y = 1).
SLOPES = [[(1, -1, 0), (-1, -1, 1)],   # bottom
          [(1, -1, 0), (1, 1, -1)],    # right
          [(-1, 1, 0), (1, 1, -1)],    # top
          [(-1, 1, 0), (-1, -1, 1)]]   # left


def make_object(name, vertices, faces):
    mesh = bpy.data.meshes.new(name)
    mesh.from_pydata(vertices, [], faces)
    bm = bmesh.new()
    bm.from_mesh(mesh)
    bmesh.ops.remove_doubles(bm, verts=bm.verts, dist=1e-6)   # join shared corners
    bmesh.ops.recalc_face_normals(bm, faces=bm.faces)        # all faces outwards
    bm.to_mesh(mesh)
    bm.free()
    for polygon in mesh.polygons:
        polygon.use_smooth = False  # flat: one normal per face
    obj = bpy.data.objects.new(name, mesh)
    bpy.context.scene.collection.objects.link(obj)
    return obj


def make_piece(name, outline, size, px, py, angle):
    # 1. The outline where it sits in the box (square 0..1).
    c, s = math.cos(math.radians(angle)), math.sin(math.radians(angle))
    box = [(px + size * (c * x - s * y), py + size * (s * x + c * y)) for (x, y) in outline]

    vertices, faces = [], []

    def add_face(points2d, front):
        start = len(vertices)
        sign = 1.0 if front else -1.0
        for (x, y) in points2d:
            vertices.append((x, y, sign * half_depth(x, y)))
        indices = list(range(start, start + len(points2d)))
        faces.append(indices if front else indices[::-1])

    # 2. Front and back surfaces, one flat face per slope the piece lies on.
    for (a1, a2) in SLOPES:
        part = clip(clip(box, *a1), *a2)
        if len(part) >= 3 and area(part) > 1e-9:
            add_face(part, True)
            add_face(part, False)

    # 3. Side walls, split where an edge crosses a diagonal.
    for i in range(len(box)):
        p, q = box[i], box[(i + 1) % len(box)]
        ts = [0.0, 1.0]
        for (a, b, k) in [(1, -1, 0), (1, 1, -1)]:
            fp, fq = a * p[0] + b * p[1] + k, a * q[0] + b * q[1] + k
            if crosses(fp, fq):
                ts.append(fp / (fp - fq))
        ts.sort()
        points = [(p[0] + t * (q[0] - p[0]), p[1] + t * (q[1] - p[1])) for t in ts]
        for j in range(len(points) - 1):
            (x0, y0), (x1, y1) = points[j], points[j + 1]
            h0, h1 = half_depth(x0, y0), half_depth(x1, y1)
            start = len(vertices)
            vertices.extend([(x0, y0, -h0), (x1, y1, -h1), (x1, y1, h1), (x0, y0, h0)])
            faces.append([start, start + 1, start + 2, start + 3])

    # 4. Move into model space: corner 0 to the origin, undo the box turn.
    local = []
    for (x, y, z) in vertices:
        dx, dy = x - px, y - py
        local.append((c * dx + s * dy, -s * dx + c * dy, z))
    return make_object(name, local, faces)


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

pieces = {}
for (name, outline, size, px, py, angle) in PIECES:
    pieces[name] = make_piece(name, outline, size, px, py, angle)
    export(pieces[name], name + ".obj")

# The tabletop: 6.4 wide (x), 3 deep (z), 0.1 thick, top surface at y = 0.
table = make_box("table", -3.2, 3.2, -0.1, 0.0, -1.5, 1.5)
export(table, "table.obj")

# ---------------------------------------------------------------------------
# ONLY FOR LOOKING AT pickagram.blend (done AFTER the export, so the OBJ files
# do not change).
# Each mesh is in its own model space, so all pieces sit at the origin, on top
# of each other. Here every piece is moved to its place in the starting square
# (the same BOX numbers as the C++ code), coloured, and laid on the table like
# the program's box: resting on the peak of its back pyramid. Moving an object
# in Blender changes only the object, not its mesh: the object's origin (the
# dot in Blender) stays at the piece's corner 0.
# ---------------------------------------------------------------------------
DISPLAY = [
    # object name,            mesh,            x,    y,    angle,  colour (RGBA)
    ("head (large)",          "large",         1.0,  1.0,  180.0, (0.07, 0.51, 0.94, 1)),
    ("body (large)",          "large",         0.0,  1.0,  -90.0, (0.80, 0.06, 0.39, 1)),
    ("top fin (small)",       "small-center",  0.25, 0.25,   0.0, (0.00, 0.61, 0.65, 1)),
    ("lower fin (small)",     "small-corner",  1.0,  0.5,   90.0, (0.92, 0.27, 0.16, 1)),
    ("square",                "square",        0.75, 0.25,   0.0, (0.14, 0.67, 0.14, 1)),
    ("tail stalk (parallel.)", "parallelogram", 0.0, 0.0,    0.0, (1.00, 0.55, 0.00, 1)),
    ("tail (medium)",         "medium",        1.0,  0.5,  225.0, (0.43, 0.23, 0.75, 1)),
]


def set_colour(obj, name, rgba):
    """Give one object its own colour (also when two objects share a mesh)."""
    material = bpy.data.materials.new(name)
    material.diffuse_color = rgba
    if len(obj.data.materials) == 0:
        obj.data.materials.append(None)       # one material slot on the mesh
    obj.material_slots[0].link = 'OBJECT'     # the colour belongs to the object
    obj.material_slots[0].material = material


used = []
for (name, mesh_name, px, py, angle, rgba) in DISPLAY:
    obj = pieces[mesh_name]
    if mesh_name in used:                     # the 2nd large triangle: same mesh, new object
        obj = bpy.data.objects.new(name, obj.data)
        bpy.context.scene.collection.objects.link(obj)
    used.append(mesh_name)
    obj.name = name
    obj.location = (px, py, PEAK)             # lifted so the back peak touches the table
    obj.rotation_euler = (0.0, 0.0, math.radians(angle))
    set_colour(obj, name, rgba)

# The table mesh is in the program's axes (y up). Blender's up is z, so turn it
# +90 degrees around x to lay it flat under the square (top surface at z = 0).
table.rotation_euler = (math.radians(90.0), 0.0, 0.0)
set_colour(table, "table", (0.60, 0.45, 0.30, 1))

bpy.ops.wm.save_as_mainfile(filepath=os.path.join(HERE, "pickagram.blend"))
print("Pickagram meshes exported to", os.path.abspath(MODELS))
