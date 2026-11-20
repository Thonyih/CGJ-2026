////////////////////////////////////////////////////////////////////////////////
//
//  Loading meshes from external files
//
// Copyright (c) 2023-26 by Carlos Martinho
//
// INTRODUCES:
// MODEL DATA, ASSIMP, mglMesh.hpp
//
////////////////////////////////////////////////////////////////////////////////

#include "../mgl/mgl.hpp" // IWYU pragma: keep

#include <memory>

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {
public:
  void initCallback(GLFWwindow *win) override;
  void displayCallback(GLFWwindow *win, double elapsed) override;

private:
  const GLuint UBO_BP = 0;

  std::unique_ptr<mgl::ShaderProgram> Shaders;
  std::unique_ptr<mgl::Camera> Camera;
  GLint ModelMatrixId = -1;
  std::unique_ptr<mgl::Mesh> Mesh;

  void createMeshes();
  void createShaderPrograms();
  void createCamera();
  void drawScene();
};

///////////////////////////////////////////////////////////////////////// MESHES

void MyApp::createMeshes() {
  std::string mesh_dir = "../../assets/models/";
  // std::string mesh_file = "cube-v.obj";
  // std::string mesh_file = "cube-vn-flat.obj";
  // std::string mesh_file = "cube-vn-smooth.obj";
  // std::string mesh_file = "cube-vt.obj";
  // std::string mesh_file = "cube-vt2.obj";
  // std::string mesh_file = "torus-vtn-flat.obj";
  // std::string mesh_file = "torus-vtn-smooth.obj";
  // std::string mesh_file = "suzanne-vtn-flat.obj";
  // std::string mesh_file = "suzanne-vtn-smooth.obj";
  // std::string mesh_file = "teapot-vn-flat.obj";
  // std::string mesh_file = "teapot-vn-smooth.obj";
  // std::string mesh_file = "bunny-vn-flat.obj";
  // std::string mesh_file = "bunny-vn-smooth.obj";
  std::string mesh_file = "monkey-torus-vtn-flat.obj";
  std::string mesh_fullname = mesh_dir + mesh_file;

  Mesh = std::make_unique<mgl::Mesh>();
  Mesh->joinIdenticalVertices();
  Mesh->create(mesh_fullname);
}

///////////////////////////////////////////////////////////////////////// SHADER

void MyApp::createShaderPrograms() {
  Shaders = std::make_unique<mgl::ShaderProgram>();
  Shaders->addShader(GL_VERTEX_SHADER, "cube-vs.glsl");
  Shaders->addShader(GL_FRAGMENT_SHADER, "cube-fs.glsl");

  Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
  if (Mesh->hasNormals()) {
    Shaders->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);
  }
  if (Mesh->hasTexcoords()) {
    Shaders->addAttribute(mgl::TEXCOORD_ATTRIBUTE, mgl::Mesh::TEXCOORD);
  }

  Shaders->addUniform(mgl::MODEL_MATRIX);
  Shaders->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
  Shaders->create();

  ModelMatrixId = Shaders->Uniforms[mgl::MODEL_MATRIX].index;
}

///////////////////////////////////////////////////////////////////////// CAMERA

// Eye(5,5,5) Center(0,0,0) Up(0,1,0)
const glm::mat4 ViewMatrix1 =
    glm::lookAt(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));

// Eye(-5,-5,-5) Center(0,0,0) Up(0,1,0)
const glm::mat4 ViewMatrix2 =
    glm::lookAt(glm::vec3(-5.0f, -5.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));

// Orthographic LeftRight(-2,2) BottomTop(-2,2) NearFar(1,10)
const glm::mat4 ProjectionMatrix1 =
    glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 1.0f, 10.0f);

// Perspective Fovy(30) Aspect(640/480) NearZ(1) FarZ(10)
const glm::mat4 ProjectionMatrix2 =
    glm::perspective(glm::radians(30.0f), 640.0f / 480.0f, 1.0f, 10.0f);

void MyApp::createCamera() {
  Camera = std::make_unique<mgl::Camera>(UBO_BP);
  Camera->setViewMatrix(ViewMatrix1);
  Camera->setProjectionMatrix(ProjectionMatrix2);
}

/////////////////////////////////////////////////////////////////////////// DRAW

glm::mat4 ModelMatrix(1.0f);

void MyApp::drawScene() {
  Shaders->bind();
  glUniformMatrix4fv(ModelMatrixId, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
  Mesh->draw();
  Shaders->unbind();
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win [[maybe_unused]]) {
  createMeshes();
  createShaderPrograms(); // after mesh;
  createCamera();
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
  engine.setWindow(800, 600, "Mesh Loader", 0, 1);
  engine.init();
  engine.run();
  return EXIT_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
