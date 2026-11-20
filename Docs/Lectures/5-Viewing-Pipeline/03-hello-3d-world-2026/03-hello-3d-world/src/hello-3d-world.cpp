////////////////////////////////////////////////////////////////////////////////
//
// Adding a camera and moving to the 3rd dimension.
// A "Hello 3D World" of Modern OpenGL.
//
// Copyright (c) 2013-25 by Carlos Martinho
//
// INTRODUCES:
// VIEW PIPELINE, UNIFORM BUFFER OBJECTS, mglCamera.hpp
//
////////////////////////////////////////////////////////////////////////////////

#include <cstddef> // IWYU pragma: keep
#include <memory>

#include "../mgl/mgl.hpp" // IWYU pragma: keep

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {
public:
  void initCallback(GLFWwindow *win) override;
  void displayCallback(GLFWwindow *win, double elapsed) override;
  void windowCloseCallback(GLFWwindow *win) override;

private:
  const GLuint POSITION = 0, COLOR = 1, UBO_BP = 0;
  GLuint VaoId = 0, VboId = 0, EboId = 0;

  std::unique_ptr<mgl::ShaderProgram> Shaders = nullptr;
  std::unique_ptr<mgl::Camera> Camera = nullptr;
  GLint ModelMatrixId;

  void createShaderProgram();
  void createBufferObjects();
  void destroyBufferObjects();
  void drawScene();
};

////////////////////////////////////////////////////////////////// VAO, VBO, EBO

struct Vertex {
  GLfloat XYZW[4];
  GLfloat RGBA[4];
};

const Vertex Vertices[] = {
    {{0.0f, 0.0f, 1.0f, 1.0f}, {0.9f, 0.1f, 0.1f, 1.0f}}, // 0 - FRONT
    {{1.0f, 0.0f, 1.0f, 1.0f}, {0.9f, 0.1f, 0.1f, 1.0f}}, // 1
    {{1.0f, 1.0f, 1.0f, 1.0f}, {0.9f, 0.1f, 0.1f, 1.0f}}, // 2
                                                          // 2
    {{0.0f, 1.0f, 1.0f, 1.0f}, {0.9f, 0.1f, 0.1f, 1.0f}}, // 3
                                                          // 0

    {{1.0f, 0.0f, 1.0f, 1.0f}, {0.1f, 0.9f, 0.1f, 1.0f}}, // 4 - RIGHT
    {{1.0f, 0.0f, 0.0f, 1.0f}, {0.1f, 0.9f, 0.1f, 1.0f}}, // 5
    {{1.0f, 1.0f, 0.0f, 1.0f}, {0.1f, 0.9f, 0.1f, 1.0f}}, // 6
                                                          // 6
    {{1.0f, 1.0f, 1.0f, 1.0f}, {0.1f, 0.9f, 0.1f, 1.0f}}, // 7
                                                          // 4

    {{1.0f, 1.0f, 1.0f, 1.0f}, {0.1f, 0.1f, 0.9f, 1.0f}}, // 8 - TOP
    {{1.0f, 1.0f, 0.0f, 1.0f}, {0.1f, 0.1f, 0.9f, 1.0f}}, // 9
    {{0.0f, 1.0f, 0.0f, 1.0f}, {0.1f, 0.1f, 0.9f, 1.0f}}, // 10
                                                          // 10
    {{0.0f, 1.0f, 1.0f, 1.0f}, {0.1f, 0.1f, 0.9f, 1.0f}}, // 11
                                                          // 8

    {{1.0f, 0.0f, 0.0f, 1.0f}, {0.1f, 0.9f, 0.9f, 1.0f}}, // 12 - BACK
    {{0.0f, 0.0f, 0.0f, 1.0f}, {0.1f, 0.9f, 0.9f, 1.0f}}, // 13
    {{0.0f, 1.0f, 0.0f, 1.0f}, {0.1f, 0.9f, 0.9f, 1.0f}}, // 14
                                                          // 14
    {{1.0f, 1.0f, 0.0f, 1.0f}, {0.1f, 0.9f, 0.9f, 1.0f}}, // 15
                                                          // 12

    {{0.0f, 0.0f, 0.0f, 1.0f}, {0.9f, 0.1f, 0.9f, 1.0f}}, // 16 - LEFT
    {{0.0f, 0.0f, 1.0f, 1.0f}, {0.9f, 0.1f, 0.9f, 1.0f}}, // 17
    {{0.0f, 1.0f, 1.0f, 1.0f}, {0.9f, 0.1f, 0.9f, 1.0f}}, // 18
                                                          // 18
    {{0.0f, 1.0f, 0.0f, 1.0f}, {0.9f, 0.1f, 0.9f, 1.0f}}, // 19
                                                          // 16

    {{0.0f, 0.0f, 1.0f, 1.0f}, {0.9f, 0.9f, 0.1f, 1.0f}}, // 20 - BOTTOM
    {{0.0f, 0.0f, 0.0f, 1.0f}, {0.9f, 0.9f, 0.1f, 1.0f}}, // 21
    {{1.0f, 0.0f, 0.0f, 1.0f}, {0.9f, 0.9f, 0.1f, 1.0f}}, // 22
                                                          // 22
    {{1.0f, 0.0f, 1.0f, 1.0f}, {0.9f, 0.9f, 0.1f, 1.0f}}  // 23
                                                          // 20
};

const unsigned int Indices[] = {
    0,  1,  2,  2,  3,  0,  // FRONT
    4,  5,  6,  6,  7,  4,  // RIGHT
    8,  9,  10, 10, 11, 8,  // TOP
    12, 13, 14, 14, 15, 12, // BACK
    16, 17, 18, 18, 19, 16, // LEFT
    20, 21, 22, 22, 23, 20  // BOTTOM
};

void MyApp::createBufferObjects() {
  glGenVertexArrays(1, &VaoId);
  glBindVertexArray(VaoId);
  {
    glGenBuffers(1, &VboId);
    glBindBuffer(GL_ARRAY_BUFFER, VboId);
    {
      glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

      glEnableVertexAttribArray(POSITION);
      glVertexAttribPointer(POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            reinterpret_cast<void *>(0));

      glEnableVertexAttribArray(COLOR);
      glVertexAttribPointer(COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            reinterpret_cast<GLvoid *>(offsetof(Vertex, RGBA)));
    }
    glGenBuffers(1, &EboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices,
                 GL_STATIC_DRAW);
  }
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MyApp::destroyBufferObjects() {
  glDeleteVertexArrays(1, &VaoId);
  glDeleteBuffers(1, &VboId);
  glDeleteBuffers(1, &EboId);
}

///////////////////////////////////////////////////////////////////////// SHADER

void MyApp::createShaderProgram() {
  Shaders = std::make_unique<mgl::ShaderProgram>();
  Shaders->addShader(GL_VERTEX_SHADER, "color-vs.glsl");
  Shaders->addShader(GL_FRAGMENT_SHADER, "color-fs.glsl");

  Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, POSITION);
  Shaders->addAttribute(mgl::COLOR_ATTRIBUTE, COLOR);
  Shaders->addUniform(mgl::MODEL_MATRIX);
  Shaders->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);

  Shaders->create();

  ModelMatrixId = Shaders->Uniforms[mgl::MODEL_MATRIX].index;
}

////////////////////////////////////////////////////////////////////////// SCENE

const glm::mat4 ModelMatrix =
    glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(2.0f)),
                   glm::vec3(-0.5f, -0.5f, -0.5f));

// Eye(5,5,5) Center(0,0,0) Up(0,1,0)
const glm::mat4 ViewMatrix1 =
    glm::lookAt(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));

// Eye(-5,-5,-5) Center(0,0,0) Up(0,1,0)
const glm::mat4 ViewMatrix2 =
    glm::lookAt(glm::vec3(-5.0f, -5.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));

// Eye(10,10,10) Center(0,0,0) Up(0,1,0)
const glm::mat4 ViewMatrix3 =
    glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));

// Orthographic LeftRight(-2,2) BottomTop(-2,2) NearFar(5,20)
const glm::mat4 ProjectionMatrix1 =
    glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 5.0f, 20.0f);

// Perspective Fovy(30) Aspect(4/3) NearZ(1) FarZ(20)
const glm::mat4 ProjectionMatrix2 =
    glm::perspective(glm::radians(30.0f), 4.0f / 3.0f, 1.0f, 20.0f);

void MyApp::drawScene() {
  Camera->setViewMatrix(ViewMatrix1);
  Camera->setProjectionMatrix(ProjectionMatrix2);

  glBindVertexArray(VaoId);
  Shaders->bind();

  glUniformMatrix4fv(ModelMatrixId, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(std::size(Indices)),
                 GL_UNSIGNED_INT, nullptr);

  Shaders->unbind();
  glBindVertexArray(0);
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win [[maybe_unused]]) {
  createBufferObjects();
  createShaderProgram();
  Camera = std::make_unique<mgl::Camera>(UBO_BP);
}

void MyApp::windowCloseCallback(GLFWwindow *win [[maybe_unused]]) {
  destroyBufferObjects();
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
  engine.setWindow(800, 600, "Hello Modern 3D World", 0, 1);
  engine.init();
  engine.run();
  return EXIT_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
