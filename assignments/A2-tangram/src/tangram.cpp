////////////////////////////////////////////////////////////////////////////////
//
// Assignment A2 - 2D Tangram: the Shark
//
// Built on the Lecture 4 "hello-2d-world" code (same structure, same mgl
// framework). Draws the 7 Tangram pieces directly in clip space.
//
// - 3 base shapes, each in its own buffer objects (VAO + VBO + EBO):
//   a right triangle, a square and a parallelogram, made only of triangles,
//   with the vertices in counter-clockwise order (right-hand rule).
// - All 5 triangle pieces reuse the same base right triangle, with a
//   different scale and colour.
// - Each piece is placed with its own model matrix:  M = T * R * S
//   (scale, then rotate, then translate), sent to the shader as a uniform.
// - Each piece has its own colour, sent to the shader as a uniform.
//
////////////////////////////////////////////////////////////////////////////////

#include <cstddef>
#include <memory>
#include <vector>

#include "../mgl/mgl.hpp" // IWYU pragma: keep

/////////////////////////////////////////////////////////////////////// VERTICES

// A vertex only has a position (x, y, z, w). z = 0 because we draw in 2D.
struct Vertex {
  GLfloat XYZW[4];
};

// Sizes follow Tangram-Stock-Sizing.jpeg: the whole Tangram square is 1 x 1.
//   large triangle   long side 1, short sides sqrt(2)/2  (base triangle, scale 1)
//   medium triangle  long side sqrt(2)/2, short sides 1/2 (base triangle, scale sqrt(2)/2)
//   small triangle   long side 1/2, short sides sqrt(2)/4 (base triangle, scale 1/2)
//   square           diagonal 1/2, side sqrt(2)/4
//   parallelogram    sides 1/2 and sqrt(2)/4
//
// Each base shape is created the way it sits in the stock Tangram square,
// with a simple reference point at the origin (0, 0):
//   triangle: the middle of its long side; square: its centre;
//   parallelogram: its bottom-left corner.
// "translate to (x, y)" moves that reference point to (x, y).

// Right triangle = the LARGE triangle: long side 1 on the x axis,
// right angle on top at (0, 1/2).
//
//       2          2 = (0, 1/2), the right angle
//     /   \   .
//   0---+---1      0 = (-1/2, 0), 1 = (1/2, 0), + = origin
const Vertex TriangleVertices[] = {
    {{-0.5f, 0.0f, 0.0f, 1.0f}},
    {{0.5f, 0.0f, 0.0f, 1.0f}},
    {{0.0f, 0.5f, 0.0f, 1.0f}}};
const GLubyte TriangleIndices[] = {0, 1, 2};

// Square: a diamond with diagonal 1/2, centred on the origin. Two triangles.
//
//      2           0 = (0, -1/4), 1 = (1/4, 0)
//    /   \   .     2 = (0, 1/4),  3 = (-1/4, 0)
//   3  +  1
//    \   /
//      0
const Vertex SquareVertices[] = {
    {{0.0f, -0.25f, 0.0f, 1.0f}},
    {{0.25f, 0.0f, 0.0f, 1.0f}},
    {{0.0f, 0.25f, 0.0f, 1.0f}},
    {{-0.25f, 0.0f, 0.0f, 1.0f}}};
const GLubyte SquareIndices[] = {0, 1, 2, 0, 2, 3};

// Parallelogram: long side 1/2 along the bottom, short sides going up and
// to the right at 45 degrees (as in the stock square). Two triangles.
//
//      3-------2   0 = (0, 0),     1 = (1/2, 0)
//     /       /    2 = (3/4, 1/4), 3 = (1/4, 1/4)
//    0-------1
const Vertex ParallelogramVertices[] = {
    {{0.0f, 0.0f, 0.0f, 1.0f}},
    {{0.5f, 0.0f, 0.0f, 1.0f}},
    {{0.75f, 0.25f, 0.0f, 1.0f}},
    {{0.25f, 0.25f, 0.0f, 1.0f}}};
const GLubyte ParallelogramIndices[] = {0, 1, 2, 0, 2, 3};

////////////////////////////////////////////////////////////////////////// SHAPE

// One base shape stored on the graphics card: its VAO, VBO and EBO.
// Created once, then drawn as many times as needed.
class Shape {
public:
  void create(const Vertex *vertices, GLsizei vertexCount,
              const GLubyte *indices, GLsizei indexCount);
  void draw() const;
  void destroy();

private:
  GLuint VaoId = 0, VboId = 0, EboId = 0;
  GLsizei IndexCount = 0;
};

const GLuint POSITION = 0; // attribute index of inPosition

void Shape::create(const Vertex *vertices, GLsizei vertexCount,
                   const GLubyte *indices, GLsizei indexCount) {
  IndexCount = indexCount;

  glGenVertexArrays(1, &VaoId);
  glBindVertexArray(VaoId);
  {
    glGenBuffers(1, &VboId);
    glBindBuffer(GL_ARRAY_BUFFER, VboId);
    {
      glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices,
                   GL_STATIC_DRAW);
      glEnableVertexAttribArray(POSITION);
      glVertexAttribPointer(POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            reinterpret_cast<GLvoid *>(0));
    }
    glGenBuffers(1, &EboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
    {
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLubyte),
                   indices, GL_STATIC_DRAW);
    }
  }
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Shape::draw() const {
  glBindVertexArray(VaoId);
  glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_BYTE,
                 reinterpret_cast<GLvoid *>(0));
  glBindVertexArray(0);
}

void Shape::destroy() {
  glDeleteVertexArrays(1, &VaoId);
  glDeleteBuffers(1, &VboId);
  glDeleteBuffers(1, &EboId);
}

////////////////////////////////////////////////////////////////////////// PIECE

// One Tangram piece: which base shape it uses, its colour and its model
// matrix. Each piece draws itself on its own, so the drawing order does not
// matter.
class Piece {
public:
  Piece(const Shape *shape, glm::vec4 color, glm::mat4 model)
      : BaseShape(shape), Color(color), Model(model) {}

  void draw(GLint matrixId, GLint colorId) const {
    glUniformMatrix4fv(matrixId, 1, GL_FALSE, glm::value_ptr(Model));
    glUniform4fv(colorId, 1, glm::value_ptr(Color));
    BaseShape->draw();
  }

private:
  const Shape *BaseShape;
  glm::vec4 Color;
  glm::mat4 Model;
};

/////////////////////////////////////////////////////////////// TRANSFORMATIONS

const glm::mat4 I(1.0f);
const float SQRT2 = 1.41421356f;
const float Q = SQRT2 / 4.0f; // short side of a small triangle (and square side)

// The shark, with the Tangram square = 1, goes from
//   x = -1     to  3*sqrt(2)/4 + 1/2   (about 2.56 wide)
//   y = -Q     to  Q + 1/2             (about 1.21 tall)
// SCENE moves its centre to (0, 0) and scales it by 0.7, so every point
// ends up inside clip space [-1, 1] (x within +-0.90, y within +-0.43).
const glm::mat4 SCENE =
    glm::scale(I, glm::vec3(0.7f, 0.7f, 1.0f)) *
    glm::translate(I, glm::vec3(0.25f - 1.5f * Q, -0.25f, 0.0f));

// Model matrix of one piece: first scale it, then rotate it around the
// origin (its reference point), then move that point to (x, y).
glm::mat4 modelMatrix(float x, float y, float angleDegrees, float size) {
  glm::mat4 T = glm::translate(I, glm::vec3(x, y, 0.0f));
  glm::mat4 R =
      glm::rotate(I, glm::radians(angleDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
  glm::mat4 S = glm::scale(I, glm::vec3(size, size, 1.0f));
  return SCENE * T * R * S;
}

//////////////////////////////////////////////////////////////////////// COLOURS

// Same colours as the Tangram pictures given with the assignment.
const glm::vec4 BLUE(0.07f, 0.51f, 0.94f, 1.0f);
const glm::vec4 MAGENTA(0.80f, 0.06f, 0.39f, 1.0f);
const glm::vec4 TEAL(0.00f, 0.61f, 0.65f, 1.0f);
const glm::vec4 RED(0.92f, 0.27f, 0.16f, 1.0f);
const glm::vec4 GREEN(0.14f, 0.67f, 0.14f, 1.0f);
const glm::vec4 ORANGE(1.00f, 0.55f, 0.00f, 1.0f);
const glm::vec4 PURPLE(0.43f, 0.23f, 0.75f, 1.0f);

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {
public:
  MyApp() = default;
  ~MyApp() override = default;

  void initCallback(GLFWwindow *win) override;
  void displayCallback(GLFWwindow *win, double elapsed) override;
  void windowCloseCallback(GLFWwindow *win) override;
  void windowSizeCallback(GLFWwindow *win, int width, int height) override;

private:
  std::unique_ptr<mgl::ShaderProgram> Shaders = nullptr;
  GLint MatrixId = -1, ColorId = -1;

  Shape Triangle, Square, Parallelogram; // the 3 base shapes
  std::vector<Piece> Pieces;             // the 7 Tangram pieces

  void createShaderProgram();
  void createBufferObjects();
  void destroyBufferObjects();
  void createPieces();
  void drawScene();
};

//////////////////////////////////////////////////////////////////////// SHADERs

void MyApp::createShaderProgram() {
  Shaders = std::make_unique<mgl::ShaderProgram>();
  Shaders->addShader(GL_VERTEX_SHADER, "tangram-vs.glsl");
  Shaders->addShader(GL_FRAGMENT_SHADER, "tangram-fs.glsl");

  Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, POSITION);
  Shaders->addUniform("Matrix");
  Shaders->addUniform("Color");

  Shaders->create();

  MatrixId = Shaders->Uniforms["Matrix"].index;
  ColorId = Shaders->Uniforms["Color"].index;
}

//////////////////////////////////////////////////////////////////// VAOs & VBOs

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

///////////////////////////////////////////////////////////////////////// PIECES

// Where each piece goes (Tangram square = 1).
// (x, y) is where the base shape's reference point ends up.
// The origin (0, 0) of the shark is the middle of the magenta body's long side.
// Angles are counter-clockwise; scale 1 / sqrt(2)/2 / 1/2 = large / medium / small.
void MyApp::createPieces() {
  // Head: large triangle, as in the stock square (long side flat at y = 0).
  Pieces.push_back(Piece(&Triangle, BLUE,
                         modelMatrix(-0.5f, 0.0f, 0.0f, 1.0f)));
  // Body: large triangle, turned 45 degrees clockwise.
  Pieces.push_back(Piece(&Triangle, MAGENTA,
                         modelMatrix(0.0f, 0.0f, -45.0f, 1.0f)));
  // Top fin: small triangle on top of the body.
  Pieces.push_back(Piece(&Triangle, TEAL,
                         modelMatrix(0.5f * Q, 1.5f * Q, -135.0f, 0.5f)));
  // Lower fin: small triangle under the head, upside down.
  Pieces.push_back(Piece(&Triangle, RED,
                         modelMatrix(-0.25f, 0.0f, 180.0f, 0.5f)));
  // Middle of the body: square, turned so its sides are level.
  Pieces.push_back(Piece(&Square, GREEN,
                         modelMatrix(1.5f * Q, 0.5f * Q, 45.0f, 1.0f)));
  // Tail stalk: parallelogram, going up to the right.
  Pieces.push_back(Piece(&Parallelogram, ORANGE,
                         modelMatrix(2.0f * Q, 0.0f, 45.0f, 1.0f)));
  // Tail: medium triangle.
  Pieces.push_back(Piece(&Triangle, PURPLE,
                         modelMatrix(3.0f * Q + 0.25f, Q + 0.25f, 135.0f,
                                     SQRT2 / 2.0f)));
}

////////////////////////////////////////////////////////////////////////// SCENE

void MyApp::drawScene() {
  // Drawing directly in clip space
  Shaders->bind();
  for (const Piece &piece : Pieces) {
    piece.draw(MatrixId, ColorId);
  }
  Shaders->unbind();
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win [[maybe_unused]]) {
  glClearColor(0.96f, 0.96f, 0.96f, 1.0f); // light grey, like the picture
  createBufferObjects();
  createShaderProgram();
  createPieces();
}

void MyApp::windowCloseCallback(GLFWwindow *win [[maybe_unused]]) {
  destroyBufferObjects();
}

// Called by mgl when the window is resized (after it sets the viewport to the
// whole window). Clip space is a square (-1..+1), so if the viewport is not
// square the shark gets stretched. Use the biggest square that fits instead,
// centred in the window: the shark keeps its shape and just grows or shrinks.
void MyApp::windowSizeCallback(GLFWwindow *win [[maybe_unused]], int width,
                               int height) {
  int size = width;
  if (height < size) {
    size = height;
  }
  glViewport((width - size) / 2, (height - size) / 2, size, size);
}

void MyApp::displayCallback(GLFWwindow *win [[maybe_unused]],
                            double elapsed [[maybe_unused]]) {
  drawScene();
}

/////////////////////////////////////////////////////////////////////////// MAIN

int main() {
  mgl::Engine &engine = mgl::Engine::getInstance();
  engine.setApp(std::make_unique<MyApp>());
  engine.setOpenGL(4, 6);
  engine.setWindow(600, 600, "2D Tangram - Shark", 0, 1);
  engine.init();
  engine.run();
  return EXIT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////// END
