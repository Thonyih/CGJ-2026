////////////////////////////////////////////////////////////////////////////////
//
// Basic Triangle rendering
//
// This program demonstrates fundamental Modern OpenGL concepts by rendering
// two triangle instances directly in Clip Space. It serves as an introductory
// example for understanding the OpenGL graphics pipeline and basic shader
// programming.
//
// Key Concepts Demonstrated:
// - Vertex Array Objects (VAOs) and Vertex Buffer Objects (VBOs)
// - Shader program creation and management
// - Attribute and uniform handling
// - Basic transformation matrices
// - Clip space rendering without model/view/projection matrices
//
// Copyright (c) 2013-26 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#include <cstddef>
#include <memory>

#include "../mgl/mgl.hpp" // IWYU pragma: keep

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {
public:
  MyApp() = default;
  ~MyApp() override = default;

  void initCallback(GLFWwindow *win) override;
  void displayCallback(GLFWwindow *win, double elapsed) override;
  void windowCloseCallback(GLFWwindow *win) override;

private:
  const GLuint POSITION = 0, COLOR = 1;
  GLuint VaoId = 0, VboId = 0, EboId = 0;
  std::unique_ptr<mgl::ShaderProgram> Shaders = nullptr;
  GLint MatrixId;

  void createShaderProgram();
  void createBufferObjects();
  void destroyBufferObjects();
  void drawScene();
};

//////////////////////////////////////////////////////////////////////// SHADERs

void MyApp::createShaderProgram() {
  Shaders = std::make_unique<mgl::ShaderProgram>();
  Shaders->addShader(GL_VERTEX_SHADER, "clip-vs.glsl");
  Shaders->addShader(GL_FRAGMENT_SHADER, "clip-fs.glsl");

  Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, POSITION);
  Shaders->addAttribute(mgl::COLOR_ATTRIBUTE, COLOR);
  Shaders->addUniform("Matrix");

  Shaders->create();

  MatrixId = Shaders->Uniforms["Matrix"].index;
}

//////////////////////////////////////////////////////////////////// VAOs & VBOs

struct Vertex {
  GLfloat XYZW[4];
  GLfloat RGBA[4];
};

const Vertex Vertices[] = {
    {{0.25f, 0.25f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.75f, 0.25f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{0.50f, 0.75f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}};

const GLubyte Indices[] = {0, 1, 2};

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
                            reinterpret_cast<GLvoid *>(0));
      glEnableVertexAttribArray(COLOR);
      glVertexAttribPointer(COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            reinterpret_cast<GLvoid *>(offsetof(Vertex, RGBA)));
    }
    glGenBuffers(1, &EboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
    {
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices,
                   GL_STATIC_DRAW);
    }
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

////////////////////////////////////////////////////////////////////////// SCENE

const glm::mat4 I(1.0f);
const glm::mat4 M =
    glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, 0.0f));

void MyApp::drawScene() {
  // Drawing directly in clip space

  glBindVertexArray(VaoId);
  Shaders->bind();

  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(I));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE,
                 reinterpret_cast<GLvoid *>(0));

  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(M));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE,
                 reinterpret_cast<GLvoid *>(0));

  Shaders->unbind();
  glBindVertexArray(0);
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win [[maybe_unused]]) {
  createBufferObjects();
  createShaderProgram();
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
  engine.setWindow(600, 600, "Hello Modern 2D World", 0, 1);
  engine.init();
  engine.run();
  return EXIT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////// END
