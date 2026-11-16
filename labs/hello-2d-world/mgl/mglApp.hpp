////////////////////////////////////////////////////////////////////////////////
//
// Application Class
//
// Copyright (c)2022-26 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_APP_HPP
#define MGL_APP_HPP

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "./mglGL.hpp" // IWYU pragma: keep

namespace mgl {

class App;
class Engine;

//////////////////////////////////////////////////////////////////////////// App

class App {
public:
  virtual ~App() = default;
  virtual void initCallback(GLFWwindow *window [[maybe_unused]]) {}
  virtual void displayCallback(GLFWwindow *window [[maybe_unused]],
                               double elapsed [[maybe_unused]]) {}
  virtual void windowCloseCallback(GLFWwindow *window [[maybe_unused]]) {}
  virtual void windowSizeCallback(GLFWwindow *window [[maybe_unused]],
                                  int width [[maybe_unused]],
                                  int height [[maybe_unused]]) {}
  virtual void cursorCallback(GLFWwindow *window [[maybe_unused]],
                              double xpos [[maybe_unused]],
                              double ypos [[maybe_unused]]) {}
  virtual void keyCallback(GLFWwindow *window [[maybe_unused]],
                           int key [[maybe_unused]],
                           int scancode [[maybe_unused]],
                           int action [[maybe_unused]],
                           int mods [[maybe_unused]]) {}
  virtual void mouseButtonCallback(GLFWwindow *window [[maybe_unused]],
                                   int button [[maybe_unused]],
                                   int action [[maybe_unused]],
                                   int mods [[maybe_unused]]) {}
  virtual void scrollCallback(GLFWwindow *window [[maybe_unused]],
                              double xoffset [[maybe_unused]],
                              double yoffset [[maybe_unused]]) {}
  virtual void joystickCallback(int jid [[maybe_unused]],
                                int event [[maybe_unused]]) {}
};

///////////////////////////////////////////////////////////////////////// Engine

class Engine {
public:
  int WindowWidth, WindowHeight;

  static Engine &getInstance();
  Engine(Engine const &) = delete;
  void operator=(Engine const &) = delete;

  void setApp(std::unique_ptr<App> app);
  App *getApp();
  void setOpenGL(int major, int minor);
  void setWindow(int width, int height, const std::string &title,
                 int fullscreen, int vsync);
  void init();
  void run();

protected:
  ~Engine();

private:
  Engine();
  std::unique_ptr<App> GlApp;
  GLFWwindow *Window;
  std::string WindowTitle;
  int GlMajor, GlMinor;
  int Fullscreen;
  int Vsync;
  bool GlfwInitialized;

  void displayInfo();
  void setupWindow();
  void setupGLFW();
  void setupGLEW();
  void setupOpenGL();
  void setupCallbacks();
  void cleanup();
};

////////////////////////////////////////////////////////////////////////////////
} // namespace mgl

#endif /* MGL_APP_HPP */
