////////////////////////////////////////////////////////////////////////////////
//
// Starting OpenGL
//
// Copyright (c) 2013-26 by Carlos Martinho
//
// INTRODUCES:
// GLFW, GLEW, mglApp.hpp, mglError.hpp
//
////////////////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <cstdlib>
#include <iostream> // IWYU pragma: keep
#include <memory>

#include "../mgl/mgl.hpp" // IWYU pragma: keep

////////////////////////////////////////////////////////////////////// DEBUG_LOG

#ifdef DEBUG
#define DEBUG_LOG(msg) std::cout << msg << std::endl
#else
#define DEBUG_LOG(msg) (void(0))
#endif

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {
public:
  MyApp() = default;
  ~MyApp() = default;
  void initCallback(GLFWwindow *win) override;
  void displayCallback(GLFWwindow *win, double elapsed) override;
  void windowCloseCallback(GLFWwindow *win) override;
  void windowSizeCallback(GLFWwindow *win, int width, int height) override;
  void keyCallback(GLFWwindow *win, int key, int scancode, int action,
                   int mods) override;
  void cursorCallback(GLFWwindow *win, double xpos, double ypos) override;
  void mouseButtonCallback(GLFWwindow *win, int button, int action,
                           int mods) override;
  void scrollCallback(GLFWwindow *win, double xoffset, double yoffset) override;
  void joystickCallback(int jid, int event) override;

private:
  unsigned int AccFrames = 0;
  double AccTime = 0.0;
  void updateFPS(GLFWwindow *win, double elapsed_sec);
};

/////////////////////////////////////////////////////////////////////////// INIT

void MyApp::initCallback(GLFWwindow *win [[maybe_unused]]) {
  DEBUG_LOG("Initializing...");
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::windowCloseCallback(GLFWwindow *win [[maybe_unused]]) {
  DEBUG_LOG("closing...");
}

void MyApp::windowSizeCallback(GLFWwindow *win [[maybe_unused]], int width,
                               int height) {
  DEBUG_LOG("size: " << width << " " << height);
}

void MyApp::keyCallback(GLFWwindow *win, int key, int scancode, int action,
                        int mods) {
  DEBUG_LOG("key: " << key << " " << scancode << " " << action << " " << mods);
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(win, GLFW_TRUE);
  }
}

void MyApp::cursorCallback(GLFWwindow *win [[maybe_unused]], double xpos,
                           double ypos) {
  DEBUG_LOG("mouse: " << xpos << " " << ypos);
}

void MyApp::mouseButtonCallback(GLFWwindow *win [[maybe_unused]], int button,
                                int action, int mods) {
  DEBUG_LOG("button: " << button << " " << action << " " << mods);
}

void MyApp::scrollCallback(GLFWwindow *win [[maybe_unused]], double xoffset,
                           double yoffset) {
  DEBUG_LOG("scroll: " << xoffset << " " << yoffset);
}

void MyApp::joystickCallback(int jid, int event) {
  DEBUG_LOG("joystick: " << jid << " " << event);
}

//////////////////////////////////////////////////////////////////////////// RUN

void MyApp::displayCallback(GLFWwindow *win, double elapsed_sec) {
  updateFPS(win, elapsed_sec);
}

void MyApp::updateFPS(GLFWwindow *win, double elapsed_sec) {
  constexpr double UPDATE_TIME = 1.0;

  ++AccFrames;
  AccTime += elapsed_sec;
  if (AccTime > UPDATE_TIME) {
    double fps = static_cast<double>(AccFrames) / AccTime;
    char title[32];
    std::snprintf(title, sizeof(title), "%5.1f FPS", fps);
    glfwSetWindowTitle(win, title);

    AccFrames = 0;
    AccTime = 0.0;
  }
}

/////////////////////////////////////////////////////////////////////////// MAIN

int main() {
  constexpr int gl_major = 4, gl_minor = 6;
  constexpr int is_fullscreen = 0;
  constexpr int is_vsync = 1;
  constexpr int width = 800;
  constexpr int height = 600;

  mgl::Engine &engine = mgl::Engine::getInstance();
  engine.setApp(std::make_unique<MyApp>());
  engine.setOpenGL(gl_major, gl_minor);
  engine.setWindow(width, height, "MGL Window", is_fullscreen, is_vsync);
  engine.init();
  engine.run();
  return EXIT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////// END
