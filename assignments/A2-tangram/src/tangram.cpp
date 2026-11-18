////////////////////////////////////////////////////////////////////////////////
//
// Assignment A2 - 2D Tangram: the Shark
//
// WHAT THIS PROGRAM DOES
//   Opens a window and draws the shark from the Pickagram book using the
//   7 Tangram pieces: 2 large triangles, 1 medium triangle, 2 small
//   triangles, 1 square and 1 parallelogram. Everything is drawn directly in
//   clip space (the -1..+1 box that the window shows), in 2D (z = 0).
//
// HOW IT IS BUILT
//   The structure is the same as the Lecture 4 "hello-2d-world" example, and
//   it uses the teacher's mgl framework (mgl\ folder) without changes:
//     mgl::Engine opens the window and runs the main loop.
//     MyApp (below) fills in the callbacks that the engine calls:
//       initCallback        once, at the start
//       displayCallback     every frame
//       windowSizeCallback  when the window is resized
//       windowCloseCallback once, at the end
//
// THE IDEA IN 5 STEPS
//   1. Only 3 BASE SHAPES are stored on the graphics card (in buffer
//      objects): a right triangle, a square and a parallelogram. They are
//      made only of triangles, with the corners in counter-clockwise order.
//   2. All 5 triangle pieces reuse the SAME base triangle (the large one),
//      just scaled: 1 for large, sqrt(2)/2 for medium, 1/2 for small.
//   3. Every piece gets its own MODEL MATRIX  M = T * R * S
//      (scale, then rotate, then translate - Lecture 2).
//   4. Every piece gets its own COLOUR.
//   5. To draw a piece, its matrix and colour are sent to the shaders as
//      UNIFORMS, then its base shape is drawn.
//
// SIZES (from Tangram-Stock-Sizing.jpeg): the whole Tangram square is 1 x 1.
//
// TO SEE EACH STEP of T * R * S: change the STEP setting just below the
// #include lines (0, 1, 2 or 3), then restart the program.
//
////////////////////////////////////////////////////////////////////////////////

#include <memory> // std::unique_ptr, std::make_unique (used by mgl)

#include "../mgl/mgl.hpp" // IWYU pragma: keep
// mgl.hpp brings in everything we need: OpenGL (GLEW), GLFW and GLM.

// ============================================================================
//  STEP
// ----------------------------------------------------------------------------
//  Shows the pieces part-way through  M = T * R * S,  to see what each
//  matrix does. To use it:
//    1. change the number on the  const int STEP  line below
//    2. stop the program (Shift+F5) and start it again (F5)
//
//    STEP = 0   the base shapes exactly as written in section 1, no matrix:
//               every piece starts at (0, 0), so they lie on top of each other
//    STEP = 1   + scale:  each piece gets its size (large, medium, small)
//    STEP = 2   + rotate: each piece turns around its corner 0
//    STEP = 3   + translate: every piece moves to its place = THE SHARK
//
//  Steps 0, 1 and 2 draw only the outlines (so pieces on top of each other
//  stay visible) and keep (0, 0) in the middle of the window.
//
//  >>> Keep STEP = 3 for the finished assignment. <<<
// ============================================================================
const int STEP = 3; // <-- CHANGE THIS NUMBER: 0, 1, 2 or 3

///////////////////////////////////////////////////////////////////////////////
// 1. VERTICES: the corners of the 3 base shapes
///////////////////////////////////////////////////////////////////////////////

// A vertex is one corner of a shape. It only has a position:
//   x, y  where the corner is
//   z     always 0, because we draw in 2D
//   w     always 1, which means "this is a point" (homogeneous coordinates,
//         Lecture 2). w = 1 is what lets a 4x4 matrix MOVE the point.
// The colour is NOT stored here: it is sent separately for each piece
// (see section 4), so the same triangle can be drawn in different colours.
struct Vertex {
  GLfloat XYZW[4];
};

// Each base shape has the same tilt as one piece in the stock Tangram square
// (Tangram-Stock-Sizing.jpeg), so its sizes can be checked against that
// picture and its corners are simple halves and quarters:
//   - the parallelogram is the orange piece (exactly the same corners)
//   - the square is the green diamond
//   - the triangle is tilted like the teal piece, but at the LARGE size
//     (long side 1); the matrix scales it down for the medium and small ones.
// Any other tilt would also work: the rotation in the model matrix turns each
// piece to the angle it needs in the shark.
//
// ONE RULE FOR ALL 3 SHAPES: corner 0 is at the origin (0, 0), and the other
// corners follow counter-clockwise. Corner 0 is the shape's REFERENCE POINT:
//   - rotate turns the shape around corner 0
//   - translate(x, y) moves corner 0 to (x, y)
//
// The corners are listed COUNTER-CLOCKWISE (right-hand rule, Lecture 2).
// The mgl engine turns on face culling with glFrontFace(GL_CCW): triangles
// whose corners go clockwise are treated as "facing away" and NOT drawn.
//
// INDICES: each shape has TWO arrays:
//   ...Vertices[]  the corners. A corner's INDEX is its position in this
//                  array: the first corner is index 0, the next index 1, ...
//   ...Indices[]   which corners make each triangle, as index numbers,
//                  read in groups of 3 (one group = one triangle).
// Example: TriangleIndices = {0, 1, 2} means "one triangle, from corner 0
// to corner 1 to corner 2". The order inside each group is counter-clockwise.
//
// Why bother? Shapes can SHARE corners. The square and the parallelogram have
// 4 corners but are drawn as 2 triangles, {0, 1, 2} and {0, 2, 3}: corners 0
// and 2 are used by both triangles but stored only once (4 corners instead
// of 6).
//
// GLubyte = OpenGL's name for an unsigned byte (a whole number 0..255),
// plenty for corner numbers. It must match GL_UNSIGNED_BYTE in
// glDrawElements (Shape::draw). The indices are uploaded to the graphics card
// in the EBO (Shape::create). Same type as the teacher's hello-2d-world.

// --- Right triangle: the LARGE triangle (long side 1) ---
// Long side on the x axis from (0, 0) to (1, 0), right angle on top.
// Reference point: corner 0, the left end of the long side.
// Short sides: sqrt(2)/2. All 5 triangle pieces are this triangle, scaled.
//
//       2          0 = (0, 0)
//     /   \   .    1 = (1, 0)
//   0-------1      2 = (1/2, 1/2), the right angle
const Vertex TriangleVertices[] = {
    {{0.0f, 0.0f, 0.0f, 1.0f}},  // index 0 -> corner (0, 0)
    {{1.0f, 0.0f, 0.0f, 1.0f}},  // index 1 -> corner (1, 0)
    {{0.5f, 0.5f, 0.0f, 1.0f}}}; // index 2 -> corner (1/2, 1/2)
// 3 corners, 1 triangle = 3 indices.
const GLubyte TriangleIndices[] = {0, 1, 2}; // the triangle: corner 0 -> 1 -> 2 (counter-clockwise)

// --- Square: a diamond with diagonal 1/2 (side sqrt(2)/4) ---
// Reference point: corner 0, the bottom corner.
//
//      2           0 = (0, 0)
//    /   \   .     1 = (1/4, 1/4)
//   3     1        2 = (0, 1/2)
//    \   /         3 = (-1/4, 1/4)
//      0
const Vertex SquareVertices[] = {
    {{0.0f, 0.0f, 0.0f, 1.0f}},    // index 0 -> corner (0, 0), bottom
    {{0.25f, 0.25f, 0.0f, 1.0f}},  // index 1 -> corner (1/4, 1/4), right
    {{0.0f, 0.5f, 0.0f, 1.0f}},    // index 2 -> corner (0, 1/2), top
    {{-0.25f, 0.25f, 0.0f, 1.0f}}}; // index 3 -> corner (-1/4, 1/4), left
// 4 corners, 2 triangles = 6 indices. The square is cut along the line from
// corner 0 to corner 2, so both triangles use corners 0 and 2.
const GLubyte SquareIndices[] = {
    0, 1, 2,  // triangle 1, the right half: corner 0 -> 1 -> 2
    0, 2, 3}; // triangle 2, the left half:  corner 0 -> 2 -> 3

// --- Parallelogram: sides 1/2 and sqrt(2)/4 ---
// Long side 1/2 along the bottom, short sides going up and to the right at
// 45 degrees, as in the stock square.
// Reference point: corner 0, the bottom-left corner.
//
//      3-------2   0 = (0, 0)      1 = (1/2, 0)
//     /       /    2 = (3/4, 1/4)  3 = (1/4, 1/4)
//    0-------1
const Vertex ParallelogramVertices[] = {
    {{0.0f, 0.0f, 0.0f, 1.0f}},    // index 0 -> corner (0, 0), bottom left
    {{0.5f, 0.0f, 0.0f, 1.0f}},    // index 1 -> corner (1/2, 0), bottom right
    {{0.75f, 0.25f, 0.0f, 1.0f}},  // index 2 -> corner (3/4, 1/4), top right
    {{0.25f, 0.25f, 0.0f, 1.0f}}}; // index 3 -> corner (1/4, 1/4), top left
// 4 corners, 2 triangles = 6 indices. The parallelogram is cut along the line
// from corner 0 to corner 2, so both triangles use corners 0 and 2.
const GLubyte ParallelogramIndices[] = {
    0, 1, 2,  // triangle 1, bottom-right half: corner 0 -> 1 -> 2
    0, 2, 3}; // triangle 2, top-left half:     corner 0 -> 2 -> 3

///////////////////////////////////////////////////////////////////////////////
// 2. SHAPE: one base shape stored on the graphics card
///////////////////////////////////////////////////////////////////////////////

// A Shape keeps the 3 OpenGL objects that hold one base shape:
//   VBO (Vertex Buffer Object)   the list of corners
//   EBO (Element Buffer Object)  which corners make each triangle
//   VAO (Vertex Array Object)    remembers the VBO, the EBO and how to read
//                                them; binding the VAO is enough to draw
// The data is uploaded ONCE (create) and then drawn every frame (draw).
class Shape {
public:
  void create(const Vertex *vertices, GLsizei vertexCount,
              const GLubyte *indices, GLsizei indexCount);
  void draw();
  void destroy();

private:
  GLuint VaoId = 0;       // name (number) of the VAO, given by OpenGL
  GLuint VboId = 0;       // name of the VBO
  GLuint EboId = 0;       // name of the EBO
  GLsizei IndexCount = 0; // how many indices to draw (3 per triangle)
};

// Attribute number of "inPosition" in the vertex shader. The shader program
// links the name "inPosition" to this number (see createShaderProgram).
const GLuint POSITION = 0;

// Uploads one base shape to the graphics card.
//   vertices, vertexCount: the corners and how many there are
//   indices, indexCount:   the corner numbers of each triangle and how many
void Shape::create(const Vertex *vertices, GLsizei vertexCount,
                   const GLubyte *indices, GLsizei indexCount) {
  IndexCount = indexCount;

  // Create the VAO and make it the current one. Everything set up while it
  // is bound (the VBO layout and the EBO) is remembered by it.
  glGenVertexArrays(1, &VaoId);
  glBindVertexArray(VaoId);
  {
    // VBO: copy the corners to the graphics card.
    // GL_STATIC_DRAW = uploaded once, drawn many times.
    glGenBuffers(1, &VboId);
    glBindBuffer(GL_ARRAY_BUFFER, VboId);
    {
      glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices,
                   GL_STATIC_DRAW);

      // Tell OpenGL how to read one corner for attribute POSITION:
      // 4 floats (x, y, z, w), not normalized, one Vertex apart, starting at
      // byte 0 of the buffer.
      glEnableVertexAttribArray(POSITION);
      glVertexAttribPointer(POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            reinterpret_cast<GLvoid *>(0));
    }

    // EBO: copy the corner numbers (indices) to the graphics card, e.g.
    // {0, 1, 2, 0, 2, 3} for the square. Each index is 1 byte (GLubyte), so
    // the size is indexCount * sizeof(GLubyte) bytes. Because the VAO is
    // bound, it remembers this EBO.
    glGenBuffers(1, &EboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
    {
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLubyte),
                   indices, GL_STATIC_DRAW);
    }
  }
  // Unbind everything so later code cannot change this shape by accident.
  // The VAO is unbound FIRST, so it keeps its link to the EBO.
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Draws the shape with whatever matrix and colour are currently set in the
// shaders (Piece::draw sets them just before calling this).
void Shape::draw() {
  glBindVertexArray(VaoId);
  // Draw triangles using the indices in the EBO:
  //   GL_TRIANGLES      read the indices in groups of 3; each group is one
  //                     triangle, e.g. {0, 2, 3} = corners 0, 2 and 3
  //   IndexCount        how many indices (3 for the triangle, 6 for the
  //                     square and the parallelogram)
  //   GL_UNSIGNED_BYTE  each index is a GLubyte (must match the arrays)
  //   0                 start at the beginning of the EBO
  glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_BYTE,
                 reinterpret_cast<GLvoid *>(0));
  glBindVertexArray(0);
}

// Frees the buffers on the graphics card (called when the window closes).
void Shape::destroy() {
  glDeleteVertexArrays(1, &VaoId);
  glDeleteBuffers(1, &VboId);
  glDeleteBuffers(1, &EboId);
}

///////////////////////////////////////////////////////////////////////////////
// 3. PIECE: one Tangram piece = a base shape + a colour + a model matrix
///////////////////////////////////////////////////////////////////////////////

// A Piece does not own any buffers: it only points to one of the 3 Shapes.
// That is how the 5 triangle pieces all share the same base triangle.
//
// Because every piece sends ITS OWN matrix and colour right before drawing,
// nothing is left over from the previous piece, so the drawing order does
// not matter (assignment requirement).
class Piece {
public:
  void set(Shape *shape, glm::vec4 color, glm::mat4 model);
  void draw(GLint matrixId, GLint colorId);

private:
  Shape *BaseShape = nullptr;       // which of the 3 base shapes to draw
  glm::vec4 Color = glm::vec4(1.0f); // red, green, blue, alpha (0..1)
  glm::mat4 Model = glm::mat4(1.0f); // where the piece goes (M = T * R * S)
};

// Stores the piece's shape, colour and model matrix.
void Piece::set(Shape *shape, glm::vec4 color, glm::mat4 model) {
  BaseShape = shape;
  Color = color;
  Model = model;
}

// Sends this piece's uniforms to the shaders, then draws its base shape.
//   matrixId: location of the uniform "Matrix" in the shader program
//   colorId:  location of the uniform "Color"
void Piece::draw(GLint matrixId, GLint colorId) {
  // 1 matrix, not transposed (GLM already stores it the way OpenGL wants).
  glUniformMatrix4fv(matrixId, 1, GL_FALSE, glm::value_ptr(Model));
  // 1 colour of 4 floats.
  glUniform4fv(colorId, 1, glm::value_ptr(Color));
  BaseShape->draw();
}

///////////////////////////////////////////////////////////////////////////////
// 4. TRANSFORMATIONS AND COLOURS
///////////////////////////////////////////////////////////////////////////////

const glm::mat4 I(1.0f); // identity matrix (changes nothing)

const float SQRT2 = 1.41421356f;
// Q = sqrt(2)/4 = the short side of a small triangle (and the side of the
// square). The pieces that are turned by 45 degrees end up on multiples of Q.
const float Q = SQRT2 / 4.0f;

// SCENE: one extra matrix applied to EVERY piece.
// With the Tangram square = 1, the shark goes from
//   x = -1   to  3Q + 1/2   (about 2.56 wide)
//   y = -Q   to  Q + 1/2    (about 1.21 tall)
// which does not fit in clip space (2 x 2). SCENE first moves the shark's
// centre (3Q/2 - 1/4, 1/4) to (0, 0), then scales everything by 0.7.
// Result: x within +-0.90 and y within +-0.42, inside [-1, 1].
const glm::mat4 SCENE =
    glm::scale(I, glm::vec3(0.7f, 0.7f, 1.0f)) *
    glm::translate(I, glm::vec3(0.25f - 1.5f * Q, -0.25f, 0.0f));

// Builds the model matrix of one piece:  SCENE * T * R * S
// Read it RIGHT TO LEFT - this is the order the steps happen to each corner:
//   S: scale the base shape by "size" (z is left at 1)
//   R: rotate it by "angleDegrees" around the z axis (counter-clockwise),
//      around corner 0, which is still at (0, 0)
//   T: move corner 0 to (x, y)
//   SCENE: fit the whole shark into clip space
// Order matters (Lecture 2: RS != SR, TR != RT). Rotating AFTER moving would
// swing the piece around (0, 0) like a door on a hinge.
glm::mat4 modelMatrix(float x, float y, float angleDegrees, float size) {
  glm::mat4 T = glm::translate(I, glm::vec3(x, y, 0.0f));
  // glm::rotate wants radians, so the angle is converted from degrees.
  glm::mat4 R = glm::rotate(I, glm::radians(angleDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
  glm::mat4 S = glm::scale(I, glm::vec3(size, size, 1.0f));

  // STEP setting (top of the file): stop part-way to see each matrix.
  if (STEP == 0) {
    return I; // no matrix: the corners stay exactly as in section 1
  }
  if (STEP == 1) {
    return S; // only scale
  }
  if (STEP == 2) {
    return R * S; // scale, then rotate around corner 0
  }
  return SCENE * T * R * S; // STEP 3: everything = the finished shark
}

// One colour per piece (red, green, blue, alpha), the same colours as the
// Tangram pictures given with the assignment.
const glm::vec4 BLUE(0.07f, 0.51f, 0.94f, 1.0f);
const glm::vec4 MAGENTA(0.80f, 0.06f, 0.39f, 1.0f);
const glm::vec4 TEAL(0.00f, 0.61f, 0.65f, 1.0f);
const glm::vec4 RED(0.92f, 0.27f, 0.16f, 1.0f);
const glm::vec4 GREEN(0.14f, 0.67f, 0.14f, 1.0f);
const glm::vec4 ORANGE(1.00f, 0.55f, 0.00f, 1.0f);
const glm::vec4 PURPLE(0.43f, 0.23f, 0.75f, 1.0f);

///////////////////////////////////////////////////////////////////////////////
// 5. MYAPP: the application (structure of hello-2d-world)
///////////////////////////////////////////////////////////////////////////////

const int NUMBER_OF_PIECES = 7;

class MyApp : public mgl::App {
public:
  MyApp() = default;
  ~MyApp() override = default;

  // Callbacks called by mgl::Engine.
  void initCallback(GLFWwindow *win) override;
  void displayCallback(GLFWwindow *win, double elapsed) override;
  void windowCloseCallback(GLFWwindow *win) override;
  void windowSizeCallback(GLFWwindow *win, int width, int height) override;

private:
  // The shader program (vertex shader + fragment shader).
  std::unique_ptr<mgl::ShaderProgram> Shaders = nullptr;
  GLint MatrixId = -1; // location of the uniform "Matrix"
  GLint ColorId = -1;  // location of the uniform "Color"

  // The 3 base shapes (on the graphics card).
  Shape Triangle;
  Shape Square;
  Shape Parallelogram;

  // The 7 Tangram pieces.
  Piece Pieces[NUMBER_OF_PIECES];

  void createShaderProgram();
  void createBufferObjects();
  void destroyBufferObjects();
  void createPieces();
  void drawScene();
};

/////////////////////////////////////////////////////////////////////// SHADERS

// Builds the shader program from the two .glsl files in src\.
// (They are opened by file name, so the program must run from src\ -
// Visual Studio's F5 does that, see the project settings.)
void MyApp::createShaderProgram() {
  Shaders = std::make_unique<mgl::ShaderProgram>();
  Shaders->addShader(GL_VERTEX_SHADER, "tangram-vs.glsl");
  Shaders->addShader(GL_FRAGMENT_SHADER, "tangram-fs.glsl");

  // Link the shader input "inPosition" to attribute number POSITION (0),
  // the number used in Shape::create.
  Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, POSITION);

  // The two uniforms we will set for every piece.
  Shaders->addUniform("Matrix");
  Shaders->addUniform("Color");

  // Compile and link the program.
  Shaders->create();

  // Ask where each uniform lives, so Piece::draw can set it.
  MatrixId = Shaders->Uniforms["Matrix"].index;
  ColorId = Shaders->Uniforms["Color"].index;
}

///////////////////////////////////////////////////////////////// VAOs & VBOs

// Uploads the 3 base shapes: (corners, number of corners, indices, number of
// indices). The numbers are the sizes of the arrays in section 1:
//   triangle:      3 corners, 1 triangle  = 3 indices {0, 1, 2}
//   square:        4 corners, 2 triangles = 6 indices {0, 1, 2, 0, 2, 3}
//   parallelogram: 4 corners, 2 triangles = 6 indices {0, 1, 2, 0, 2, 3}
void MyApp::createBufferObjects() {
  Triangle.create(TriangleVertices, 3, TriangleIndices, 3);
  Square.create(SquareVertices, 4, SquareIndices, 6);
  Parallelogram.create(ParallelogramVertices, 4, ParallelogramIndices, 6);
}

void MyApp::destroyBufferObjects() {
  Triangle.destroy();
  Square.destroy();
  Parallelogram.destroy();
}

//////////////////////////////////////////////////////////////////////// PIECES

// Places the 7 pieces of the shark.
//   modelMatrix(x, y, angle, scale):
//     (x, y)  where the base shape's corner 0 goes
//     angle   in degrees, counter-clockwise (negative = clockwise)
//     scale   1 = large, sqrt(2)/2 = medium, 1/2 = small triangle
// The shark's own origin (0, 0) is the head's right corner (where the head,
// the lower fin and the body meet).
//
// Think of each piece as a cardboard cut-out with a pin through corner 0:
// scale and rotate happen around the pin, then translate moves the pin to
// (x, y). The other corners just come along. The "Corners end at" line under
// each piece lists where every corner lands in the shark (worked out by hand;
// see also explanation/A2-every-corner.png).
void MyApp::createPieces() {
  // 0. Head: large triangle, exactly as in the stock square (long side level).
  //    Corners end at: 0 -> (-1, 0)   1 -> (0, 0)   2 -> (-1/2, 1/2)
  Pieces[0].set(&Triangle, BLUE, modelMatrix(-1.0f, 0.0f, 0.0f, 1.0f));

  // 1. Body: large triangle, turned 45 degrees clockwise, so its right angle
  //    points up and to the right.
  //    Corners end at: 0 -> (-Q, Q)   1 -> (Q, -Q)   2 -> (Q, Q)
  Pieces[1].set(&Triangle, MAGENTA, modelMatrix(-Q, Q, -45.0f, 1.0f));

  // 2. Top fin: small triangle (scale 1/2) sitting on top of the body.
  //    Corners end at: 0 -> (Q, 2Q)   1 -> (0, Q)   2 -> (Q, Q)
  Pieces[2].set(&Triangle, TEAL, modelMatrix(Q, 2.0f * Q, -135.0f, 0.5f));

  // 3. Lower fin: small triangle (scale 1/2) under the head, upside down.
  //    Corners end at: 0 -> (0, 0)   1 -> (-1/2, 0)   2 -> (-1/4, -1/4)
  Pieces[3].set(&Triangle, RED, modelMatrix(0.0f, 0.0f, 180.0f, 0.5f));

  // 4. Middle of the body: square, turned 45 degrees so its sides are level.
  //    Corners end at: 0 -> (2Q, 0)   1 -> (2Q, Q)   2 -> (Q, Q)   3 -> (Q, 0)
  Pieces[4].set(&Square, GREEN, modelMatrix(2.0f * Q, 0.0f, 45.0f, 1.0f));

  // 5. Tail stalk: parallelogram, turned 45 degrees so it goes up to the right.
  //    Corners end at: 0 -> (2Q, 0)   1 -> (3Q, Q)   2 -> (3Q, 2Q)   3 -> (2Q, Q)
  Pieces[5].set(&Parallelogram, ORANGE,
                modelMatrix(2.0f * Q, 0.0f, 45.0f, 1.0f));

  // 6. Tail: medium triangle (scale sqrt(2)/2).
  //    Corners end at: 0 -> (3Q + 1/2, Q)   1 -> (3Q, Q + 1/2)   2 -> (3Q, Q)
  Pieces[6].set(&Triangle, PURPLE,
                modelMatrix(3.0f * Q + 0.5f, Q, 135.0f, SQRT2 / 2.0f));
}

///////////////////////////////////////////////////////////////////////// SCENE

// Draws all the pieces, directly in clip space.
void MyApp::drawScene() {
  Shaders->bind(); // use our shader program
  for (int i = 0; i < NUMBER_OF_PIECES; i++) {
    Pieces[i].draw(MatrixId, ColorId);
  }
  Shaders->unbind();
}

///////////////////////////////////////////////////////////////////// CALLBACKS

// Called once, after the window and OpenGL are ready.
void MyApp::initCallback(GLFWwindow *win [[maybe_unused]]) {
  glClearColor(0.96f, 0.96f, 0.96f, 1.0f); // light grey, like the picture
  if (STEP < 3) {
    // STEP setting (top of the file): draw only the edges of each triangle.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
  createBufferObjects();
  createShaderProgram();
  createPieces();
}

// Called once, when the window is closed.
void MyApp::windowCloseCallback(GLFWwindow *win [[maybe_unused]]) {
  destroyBufferObjects();
}

// Called when the window is resized, right after mgl has set the viewport to
// the whole window. Clip space is a square (-1..+1), so a non-square viewport
// would stretch the shark. Instead, use the biggest square that fits in the
// window, centred: the shark keeps its shape and just grows or shrinks.
//   glViewport(left, bottom, width, height), in pixels.
void MyApp::windowSizeCallback(GLFWwindow *win [[maybe_unused]], int width,
                               int height) {
  int size = width;
  if (height < size) {
    size = height;
  }
  glViewport((width - size) / 2, (height - size) / 2, size, size);
}

// Called every frame. The engine has already cleared the screen; after this
// it swaps the back and front buffers (double buffering, Lecture 4).
void MyApp::displayCallback(GLFWwindow *win [[maybe_unused]],
                            double elapsed [[maybe_unused]]) {
  drawScene();
}

////////////////////////////////////////////////////////////////////////// MAIN

int main() {
  mgl::Engine &engine = mgl::Engine::getInstance();
  engine.setApp(std::make_unique<MyApp>());
  engine.setOpenGL(4, 6); // OpenGL 4.6 core
  // 600 x 600 window, title, not fullscreen (0), v-sync on (1).
  engine.setWindow(600, 600, "2D Tangram - Shark", 0, 1);
  engine.init(); // creates the window, then calls initCallback
  engine.run();  // main loop: displayCallback every frame until closed
  return EXIT_SUCCESS;
}

////////////////////////////////////////////////////////////////////////// END
