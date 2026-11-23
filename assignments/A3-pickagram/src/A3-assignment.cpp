////////////////////////////////////////////////////////////////////////////////
//
// Assignment A3 - 3D Pickagram (the A2 shark, now in 3D)
//
// Built on the lecture code:
//   - hello-3d-world: the camera (mgl::Camera, a UBO with the view and
//     projection matrices) and the ModelMatrix uniform.
//   - mesh-loader:    meshes read from OBJ files with Assimp (mgl::Mesh).
// The teacher's mgl folder is used unchanged.
//
// WHAT IT DOES
//   - 7 Tangram pieces (made in Blender, see assets/blender/make-pieces.py)
//     stand on a table as the shark from A2.
//   - LEFT / RIGHT arrow (hold): animate the pieces into their box (a closed
//     square lying on the table) / back into the shark. Let go to stop halfway.
//   - LEFT mouse drag:  move the table (and everything on it).
//   - RIGHT mouse drag: orbit the camera around its centre.
//   - Scroll wheel:     move the camera closer / further.
//   - C: switch between the 2 cameras.   P: perspective <-> orthographic.
//
// THE FILE, TOP TO BOTTOM
//   1. Settings      sizes and speeds
//   2. SceneNode     the scene graph (lecture 6)
//   3. OrbitCamera   a camera that orbits a centre, turned with a quaternion
//   4. The pieces    the 2 configurations: the box and the shark
//   5. MyApp         loads everything, animates and draws, handles input
//
////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../mgl/mgl.hpp" // IWYU pragma: keep

///////////////////////////////////////////////////////////////////////////////
// 1. SETTINGS
///////////////////////////////////////////////////////////////////////////////

const float SQRT2 = 1.41421356f;
const float Q = SQRT2 / 4.0f;      // short side of a small triangle (as in A2)
const float THICKNESS = 0.1f;      // thickness of every piece (as in make-pieces.py)
const float ANIMATION_SPEED = 0.5f; // the full animation takes 1 / 0.5 = 2 seconds

const std::string MODELS = "../assets/models/"; // relative to src\ (the working folder)

const glm::mat4 I(1.0f);
const glm::vec3 X_AXIS(1.0f, 0.0f, 0.0f);
const glm::vec3 Y_AXIS(0.0f, 1.0f, 0.0f);
const glm::vec3 Z_AXIS(0.0f, 0.0f, 1.0f);

///////////////////////////////////////////////////////////////////////////////
// 2. SCENE GRAPH
///////////////////////////////////////////////////////////////////////////////
//
// Each node has (as the assignment asks):
//   - Mesh:        the mesh to draw (nullptr = draws nothing, only moves its children)
//   - Shader:      the shader program used while drawing it
//   - ModelMatrix: where it is, RELATIVE TO ITS PARENT
// plus the colour of the piece and its children.
//
// draw() works like the matrix stack in lecture 6: a node's place in the world
// is its parent's place times its own matrix (world = parentWorld * local).
// So moving a node moves all its children with it.
//
// Our scene:
//   Root
//   └── Table (moved with the left mouse button)
//       ├── TableTop (the table mesh)
//       └── Pickagram
//           └── the 7 pieces (animated between the box and the shark)

class SceneNode {
public:
  mgl::Mesh *Mesh = nullptr;
  mgl::ShaderProgram *Shader = nullptr;
  glm::mat4 ModelMatrix = glm::mat4(1.0f);
  glm::vec4 Color = glm::vec4(1.0f);
  std::vector<SceneNode *> Children;

  void addChild(SceneNode *child) { Children.push_back(child); }

  void draw(const glm::mat4 &parentWorld) {
    glm::mat4 world = parentWorld * ModelMatrix;
    if (Mesh != nullptr) {
      Shader->bind();
      glUniformMatrix4fv(Shader->Uniforms[mgl::MODEL_MATRIX].index, 1, GL_FALSE,
                         glm::value_ptr(world));
      glUniform4fv(Shader->Uniforms["Color"].index, 1, glm::value_ptr(Color));
      Mesh->draw();
      Shader->unbind();
    }
    for (int i = 0; i < (int)Children.size(); i++) {
      Children[i]->draw(world);
    }
  }
};

///////////////////////////////////////////////////////////////////////////////
// 3. ORBIT CAMERA
///////////////////////////////////////////////////////////////////////////////
//
// The camera sits on a sphere around Center, always looking at it.
//   - Orientation (a quaternion) turns the camera around Center. The camera
//     starts on the +z side of Center, so its position is
//         eye = Center + Orientation * (0, 0, Distance)
//   - Its "up" and "right" are the y and x axes turned the same way.
//   - Orbiting multiplies Orientation by two small turns around the camera's
//     OWN up and right axes. Turning around the camera's own axes (not around
//     fixed x / y / z angles) is what avoids gimbal lock (lecture 3).
//   - The view matrix is glm::lookAt(eye, center, up), as in lecture 5.

class OrbitCamera {
public:
  glm::vec3 Center = glm::vec3(0.0f);
  float Distance = 5.0f;
  glm::quat Orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // no turn
  bool Perspective = true;

  glm::vec3 eye() const { return Center + Orientation * glm::vec3(0.0f, 0.0f, Distance); }
  glm::vec3 up() const { return Orientation * Y_AXIS; }
  glm::vec3 right() const { return Orientation * X_AXIS; }
  glm::vec3 front() const { return Orientation * glm::vec3(0.0f, 0.0f, -1.0f); }

  // dx, dy: how far the mouse moved, in pixels.
  void orbit(float dx, float dy) {
    glm::quat turnAroundUp = glm::angleAxis(glm::radians(-0.3f * dx), Y_AXIS);
    glm::quat turnAroundRight = glm::angleAxis(glm::radians(-0.3f * dy), X_AXIS);
    Orientation = glm::normalize(Orientation * turnAroundUp * turnAroundRight);
  }

  // steps: how far the wheel turned (+1 = one notch forward = closer).
  void zoom(float steps) {
    Distance = Distance * (1.0f - 0.1f * steps);
    if (Distance < 1.0f) Distance = 1.0f;
    if (Distance > 30.0f) Distance = 30.0f;
  }

  glm::mat4 viewMatrix() const { return glm::lookAt(eye(), Center, up()); }

  // aspect = window width / height, so the scene is never stretched.
  glm::mat4 projectionMatrix(float aspect) const {
    float fovy = glm::radians(30.0f);
    if (Perspective) {
      return glm::perspective(fovy, aspect, 0.1f, 100.0f);
    }
    // Orthographic box the same size as what the perspective camera sees at
    // the centre, so switching with P keeps the scene about the same size.
    float h = Distance * std::tan(fovy / 2.0f);
    return glm::ortho(-h * aspect, h * aspect, -h, h, 0.1f, 100.0f);
  }
};

///////////////////////////////////////////////////////////////////////////////
// 4. THE PIECES AND THEIR 2 CONFIGURATIONS
///////////////////////////////////////////////////////////////////////////////
//
// Every piece is one of 3 meshes, made like the A2 shapes: the flat shape in
// the x-y plane with corner 0 at (0, 0, 0), THICKNESS deep along z.
// The base triangle is the large one, scaled for the medium and small ones.
//
// A configuration is written as in A2, in the flat 2D picture:
//   x, y  = where corner 0 goes      angle = turn around corner 0 (degrees)
//
// SHARK: the A2 shark, standing up in the x-y plane on the table.
// BOX:   the Tangram square (1 x 1, from Tangram-Stock-Sizing.jpeg), lying
//        flat on the table: the 2D picture is turned -90 degrees around x,
//        so the picture's y becomes -z and the thickness points up.

const int NUMBER_OF_PIECES = 7;

//                                  head    body   top fin lower fin square parallel. tail
const int MESH[NUMBER_OF_PIECES] = {0,      0,     0,      0,        1,     2,        0}; // 0 triangle, 1 square, 2 parallelogram
const float SIZE[NUMBER_OF_PIECES] = {1.0f, 1.0f,  0.5f,   0.5f,     1.0f,  1.0f,     SQRT2 / 2.0f};

const float SHARK[NUMBER_OF_PIECES][3] = {
    // x                y           angle
    {-1.0f,             0.0f,       0.0f},   // head, large triangle, blue
    {-Q,                Q,        -45.0f},   // body, large triangle, magenta
    {Q,                 2.0f * Q, -135.0f},  // top fin, small triangle, teal
    {0.0f,              0.0f,     180.0f},   // lower fin, small triangle, red
    {2.0f * Q,          0.0f,      45.0f},   // body, square, green
    {2.0f * Q,          0.0f,      45.0f},   // tail stalk, parallelogram, orange
    {3.0f * Q + 0.5f,   Q,        135.0f}};  // tail, medium triangle, purple

const float BOX[NUMBER_OF_PIECES][3] = {
    // x       y       angle
    {1.0f,     1.0f,   180.0f},  // blue:    top of the square
    {0.0f,     1.0f,   -90.0f},  // magenta: left side
    {0.25f,    0.25f,    0.0f},  // teal:    middle, under the centre
    {1.0f,     0.5f,    90.0f},  // red:     right side, top
    {0.75f,    0.25f,    0.0f},  // green:   right of the centre
    {0.0f,     0.0f,     0.0f},  // orange:  bottom left
    {1.0f,     0.5f,   225.0f}}; // purple:  bottom right

const glm::vec4 COLOR[NUMBER_OF_PIECES] = {
    {0.07f, 0.51f, 0.94f, 1.0f},  // blue
    {0.80f, 0.06f, 0.39f, 1.0f},  // magenta
    {0.00f, 0.61f, 0.65f, 1.0f},  // teal
    {0.92f, 0.27f, 0.16f, 1.0f},  // red
    {0.14f, 0.67f, 0.14f, 1.0f},  // green
    {1.00f, 0.55f, 0.00f, 1.0f},  // orange
    {0.43f, 0.23f, 0.75f, 1.0f}}; // purple
const glm::vec4 TABLE_COLOR(0.60f, 0.45f, 0.30f, 1.0f); // wood

// Where each 2D picture is put on the table (the table top is at y = 0).
// Shark: its lowest corner (the body's tip, y = -Q) touches the table, and it
// is centred on the table's depth. Box: to the left of the shark.
const glm::vec3 SHARK_OFFSET(1.2f, Q, -THICKNESS / 2.0f);
const glm::vec3 BOX_OFFSET(-2.5f, 0.0f, 0.5f);

///////////////////////////////////////////////////////////////////////////////
// 5. MYAPP
///////////////////////////////////////////////////////////////////////////////

class MyApp : public mgl::App {
public:
  void initCallback(GLFWwindow *win) override;
  void displayCallback(GLFWwindow *win, double elapsed) override;
  void windowSizeCallback(GLFWwindow *win, int width, int height) override;
  void cursorCallback(GLFWwindow *win, double xpos, double ypos) override;
  void keyCallback(GLFWwindow *win, int key, int scancode, int action, int mods) override;
  void scrollCallback(GLFWwindow *win, double xoffset, double yoffset) override;

private:
  const GLuint UBO_BP = 0; // binding point of the camera's uniform block

  // Meshes and shaders (as in the lecture's mesh-loader)
  std::unique_ptr<mgl::Mesh> Meshes[3]; // 0 triangle, 1 square, 2 parallelogram
  std::unique_ptr<mgl::Mesh> TableMesh;
  std::unique_ptr<mgl::ShaderProgram> Shaders;

  // Cameras: one UBO (mgl::Camera, as in hello-3d-world), filled every frame
  // from whichever of our 2 orbit cameras is active.
  std::unique_ptr<mgl::Camera> Camera;
  OrbitCamera Cameras[2];
  int ActiveCamera = 0;
  int WindowWidth = 800, WindowHeight = 600;

  // Scene graph
  SceneNode Root, Table, TableTop, Pickagram;
  SceneNode Pieces[NUMBER_OF_PIECES];
  glm::vec3 TablePosition = glm::vec3(0.0f);

  // Each piece's position (corner 0) and turn in the 2 configurations
  glm::vec3 BoxPosition[NUMBER_OF_PIECES], SharkPosition[NUMBER_OF_PIECES];
  glm::quat BoxTurn[NUMBER_OF_PIECES], SharkTurn[NUMBER_OF_PIECES];
  float Progress = 1.0f; // 0 = in the box, 1 = the shark

  // Mouse position at the last cursor event
  double LastX = 0.0, LastY = 0.0;

  std::unique_ptr<mgl::Mesh> loadMesh(const std::string &file);
  void createMeshes();
  void createShaderProgram();
  void createCameras();
  void createConfigurations();
  void createSceneGraph();
  void animate(GLFWwindow *win, float elapsed);
  void moveTable(float dx, float dy);
  void drawScene();
};

//////////////////////////////////////////////////////////////////// MESHES

std::unique_ptr<mgl::Mesh> MyApp::loadMesh(const std::string &file) {
  std::unique_ptr<mgl::Mesh> mesh = std::make_unique<mgl::Mesh>();
  mesh->joinIdenticalVertices();
  mesh->create(MODELS + file);
  return mesh;
}

void MyApp::createMeshes() {
  Meshes[0] = loadMesh("triangle.obj");
  Meshes[1] = loadMesh("square.obj");
  Meshes[2] = loadMesh("parallelogram.obj");
  TableMesh = loadMesh("table.obj");
}

//////////////////////////////////////////////////////////////////// SHADER

void MyApp::createShaderProgram() {
  Shaders = std::make_unique<mgl::ShaderProgram>();
  Shaders->addShader(GL_VERTEX_SHADER, "pickagram-vs.glsl");
  Shaders->addShader(GL_FRAGMENT_SHADER, "pickagram-fs.glsl");

  Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
  Shaders->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);
  Shaders->addUniform(mgl::MODEL_MATRIX);
  Shaders->addUniform("Color");
  Shaders->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
  Shaders->create();
}

//////////////////////////////////////////////////////////////////// CAMERAS

// Camera 0 looks at the shark from the front (the program starts as the shark),
// camera 1 looks down at the box.
void MyApp::createCameras() {
  Camera = std::make_unique<mgl::Camera>(UBO_BP);

  Cameras[0].Center = SHARK_OFFSET + glm::vec3(0.28f, 0.25f, 0.0f); // middle of the shark
  Cameras[0].Distance = 7.0f;
  Cameras[0].Orientation = glm::angleAxis(glm::radians(20.0f), Y_AXIS) *
                           glm::angleAxis(glm::radians(-15.0f), X_AXIS);

  Cameras[1].Center = BOX_OFFSET + glm::vec3(0.5f, 0.0f, -0.5f); // middle of the box
  Cameras[1].Distance = 4.0f;
  Cameras[1].Orientation = glm::angleAxis(glm::radians(-55.0f), X_AXIS); // from above
}

//////////////////////////////////////////////////////////////////// PIECES

// Turns the 2D tables above into 3D positions and turns (quaternions).
void MyApp::createConfigurations() {
  glm::quat layFlat = glm::angleAxis(glm::radians(-90.0f), X_AXIS);
  for (int i = 0; i < NUMBER_OF_PIECES; i++) {
    // Shark: standing in the x-y plane, turned around z.
    SharkPosition[i] = SHARK_OFFSET + glm::vec3(SHARK[i][0], SHARK[i][1], 0.0f);
    SharkTurn[i] = glm::angleAxis(glm::radians(SHARK[i][2]), Z_AXIS);

    // Box: the same kind of 2D placement, then laid flat (2D (x, y) -> 3D (x, 0, -y)).
    BoxPosition[i] = BOX_OFFSET + glm::vec3(BOX[i][0], 0.0f, -BOX[i][1]);
    BoxTurn[i] = layFlat * glm::angleAxis(glm::radians(BOX[i][2]), Z_AXIS);
  }
}

//////////////////////////////////////////////////////////////////// SCENE GRAPH

void MyApp::createSceneGraph() {
  Root.addChild(&Table);

  Table.addChild(&TableTop);
  TableTop.Mesh = TableMesh.get();
  TableTop.Shader = Shaders.get();
  TableTop.Color = TABLE_COLOR;

  Table.addChild(&Pickagram);
  for (int i = 0; i < NUMBER_OF_PIECES; i++) {
    Pieces[i].Mesh = Meshes[MESH[i]].get();
    Pieces[i].Shader = Shaders.get();
    Pieces[i].Color = COLOR[i];
    Pickagram.addChild(&Pieces[i]);
  }
}

//////////////////////////////////////////////////////////////////// ANIMATION

// Hold LEFT to go towards the box (Progress -> 0), RIGHT towards the shark
// (Progress -> 1). Nothing pressed: Progress stays where it is.
void MyApp::animate(GLFWwindow *win, float elapsed) {
  if (glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS) {
    Progress = Progress - ANIMATION_SPEED * elapsed;
  }
  if (glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    Progress = Progress + ANIMATION_SPEED * elapsed;
  }
  if (Progress < 0.0f) Progress = 0.0f;
  if (Progress > 1.0f) Progress = 1.0f;

  // Each piece: position in a straight line (mix), turn with SLERP (lecture 3).
  for (int i = 0; i < NUMBER_OF_PIECES; i++) {
    glm::vec3 position = glm::mix(BoxPosition[i], SharkPosition[i], Progress);
    glm::quat turn = glm::slerp(BoxTurn[i], SharkTurn[i], Progress);
    glm::mat4 T = glm::translate(I, position);
    glm::mat4 R = glm::mat4_cast(turn);
    glm::mat4 S = glm::scale(I, glm::vec3(SIZE[i], SIZE[i], 1.0f)); // thickness not scaled
    Pieces[i].ModelMatrix = T * R * S;
  }
}

//////////////////////////////////////////////////////////////////// TABLE

// Left mouse drag. Left/right: along the camera's right vector.
// Up/down: along the camera's front vector (up = away from the camera).
// Both vectors are flattened onto the table's plane (y = 0), so the table
// slides on the floor instead of going up or down.
void MyApp::moveTable(float dx, float dy) {
  OrbitCamera &camera = Cameras[ActiveCamera];
  glm::vec3 right = camera.right();
  glm::vec3 front = camera.front();
  if (std::abs(front.y) > 0.99f) {
    front = camera.up(); // looking straight down: "away" on screen is the camera's up
  }
  right.y = 0.0f;
  front.y = 0.0f;
  right = glm::normalize(right);
  front = glm::normalize(front);

  float step = 0.002f * camera.Distance; // further away = bigger steps
  TablePosition = TablePosition + right * (dx * step) + front * (-dy * step);
  Table.ModelMatrix = glm::translate(I, TablePosition);
}

//////////////////////////////////////////////////////////////////// DRAW

void MyApp::drawScene() {
  OrbitCamera &camera = Cameras[ActiveCamera];
  float aspect = (float)WindowWidth / (float)WindowHeight;
  Camera->setViewMatrix(camera.viewMatrix());
  Camera->setProjectionMatrix(camera.projectionMatrix(aspect));

  Root.draw(I);
}

//////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win [[maybe_unused]]) {
  glClearColor(0.85f, 0.88f, 0.92f, 1.0f); // light grey-blue background
  createMeshes();
  createShaderProgram(); // after the meshes (as in mesh-loader)
  createCameras();
  createConfigurations();
  createSceneGraph();
}

void MyApp::displayCallback(GLFWwindow *win, double elapsed) {
  animate(win, (float)elapsed);
  drawScene();
}

// mgl already sets the viewport to the whole window; we keep the size so the
// projection uses the right aspect ratio (no stretching).
void MyApp::windowSizeCallback(GLFWwindow *win [[maybe_unused]], int width, int height) {
  if (width > 0 && height > 0) { // 0 when the window is minimised
    WindowWidth = width;
    WindowHeight = height;
  }
}

void MyApp::cursorCallback(GLFWwindow *win, double xpos, double ypos) {
  float dx = (float)(xpos - LastX);
  float dy = (float)(ypos - LastY);
  LastX = xpos;
  LastY = ypos;

  if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
    Cameras[ActiveCamera].orbit(dx, dy);
  }
  if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    moveTable(dx, dy);
  }
}

void MyApp::keyCallback(GLFWwindow *win [[maybe_unused]], int key,
                        int scancode [[maybe_unused]], int action,
                        int mods [[maybe_unused]]) {
  if (action != GLFW_PRESS) return;
  if (key == GLFW_KEY_C) {
    ActiveCamera = 1 - ActiveCamera; // each camera keeps its own state
  }
  if (key == GLFW_KEY_P) {
    Cameras[ActiveCamera].Perspective = !Cameras[ActiveCamera].Perspective;
  }
}

void MyApp::scrollCallback(GLFWwindow *win [[maybe_unused]],
                           double xoffset [[maybe_unused]], double yoffset) {
  Cameras[ActiveCamera].zoom((float)yoffset);
}

/////////////////////////////////////////////////////////////////////////// MAIN

int main() {
  mgl::Engine &engine = mgl::Engine::getInstance();
  engine.setApp(std::make_unique<MyApp>());
  engine.setOpenGL(4, 6);
  engine.setWindow(800, 600, "A3 - Pickagram - Group 3", 0, 1);
  engine.init();
  engine.run();
  return EXIT_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
