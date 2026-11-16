////////////////////////////////////////////////////////////////////////////////
//
// Application Class
//
// Copyright (c)2022-26 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#include "./mglApp.hpp"

#include <iostream>
#include <stdexcept>

#include "./mglError.hpp" // IWYU pragma: keep -- required in debug mode

namespace mgl {

/////////////////////////////////////////////////////////////// STATIC CALLBACKS

static void window_close_callback(GLFWwindow *window) {
  mgl::App *app = mgl::Engine::getInstance().getApp();
  if (app)
    app->windowCloseCallback(window);
}

static void framebuffer_size_callback(GLFWwindow *window, int width,
                                      int height) {
  Engine::getInstance().WindowWidth = width;
  Engine::getInstance().WindowHeight = height;
  glViewport(0, 0, width, height);
  mgl::App *app = mgl::Engine::getInstance().getApp();
  if (app) {
    app->windowSizeCallback(window, width, height);
  }
}

static void glfw_error_callback(int error [[maybe_unused]],
                                const char *description) {
  std::cerr << "GLFW Error: " << description << std::endl;
}

static void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos) {
  mgl::App *app = mgl::Engine::getInstance().getApp();
  if (app)
    app->cursorCallback(window, xpos, ypos);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
  mgl::App *app = mgl::Engine::getInstance().getApp();
  if (app)
    app->keyCallback(window, key, scancode, action, mods);
}

static void mouse_button_callback(GLFWwindow *window, int button, int action,
                                  int mods) {
  mgl::App *app = mgl::Engine::getInstance().getApp();
  if (app)
    app->mouseButtonCallback(window, button, action, mods);
}

static void scroll_callback(GLFWwindow *window, double xoffset,
                            double yoffset) {
  mgl::App *app = mgl::Engine::getInstance().getApp();
  if (app)
    app->scrollCallback(window, xoffset, yoffset);
}

static void joystick_callback(int jid, int event) {
  mgl::App *app = mgl::Engine::getInstance().getApp();
  if (app)
    app->joystickCallback(jid, event);
}

////////////////////////////////////////////////////////////////////////// SETUP

Engine::Engine()
    : WindowWidth(640), WindowHeight(480), GlApp(nullptr), Window(nullptr),
      WindowTitle("OpenGL App GLFW Window 2026(c) Carlos Martinho"), GlMajor(4),
      GlMinor(6), Fullscreen(0), Vsync(0), GlfwInitialized(false) {}

Engine::~Engine() { cleanup(); }

void Engine::cleanup() {
  if (GlApp) {
    // Destroy the app and its shaders while context is valid
    GlApp.reset();
  }
  if (Window) {
    glfwMakeContextCurrent(nullptr);
    glfwDestroyWindow(Window);
    Window = nullptr;
  }
  if (GlfwInitialized) {
    glfwTerminate();
    GlfwInitialized = false;
  }
}

Engine &Engine::getInstance() {
  static Engine instance;
  return instance;
}

App *Engine::getApp() { return GlApp.get(); }

void Engine::setApp(std::unique_ptr<App> app) { GlApp = std::move(app); }

void Engine::setOpenGL(int major, int minor) {
  GlMajor = major;
  GlMinor = minor;
}

void Engine::setWindow(int width, int height, const std::string &title,
                       int fullscreen, int vsync) {
  WindowWidth = width;
  WindowHeight = height;
  WindowTitle = title;
  Fullscreen = fullscreen;
  Vsync = vsync;
}

/////////////////////////////////////////////////////////////////////////// INIT

void Engine::setupWindow() {
  GLFWmonitor *monitor = Fullscreen ? glfwGetPrimaryMonitor() : nullptr;
  Window = glfwCreateWindow(WindowWidth, WindowHeight, WindowTitle.c_str(),
                            monitor, nullptr);
  if (!Window) {
    throw std::runtime_error("Failed to create GLFW window.");
  }
  glfwMakeContextCurrent(Window);
  glfwSwapInterval(Vsync);
  // Sync WindowWidth/WindowHeight with actual framebuffer pixels (for High-DPI)
  glfwGetFramebufferSize(Window, &WindowWidth, &WindowHeight);
}

void Engine::setupCallbacks() {
  glfwSetCursorPosCallback(Window, cursor_pos_callback);
  glfwSetKeyCallback(Window, key_callback);
  glfwSetMouseButtonCallback(Window, mouse_button_callback);
  glfwSetScrollCallback(Window, scroll_callback);
  glfwSetJoystickCallback(joystick_callback);
  glfwSetWindowCloseCallback(Window, window_close_callback);
  glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);
}

void Engine::setupGLFW() {
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW.");
  }
  GlfwInitialized = true;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GlMajor);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GlMinor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef DEBUG
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
  setupWindow();
  setupCallbacks();
}

void Engine::setupGLEW() {
  GLenum result = glewInit();
  if (result != GLEW_OK) {
    // On Wayland, glewInit often returns GLEW_ERROR_NO_GLX_DISPLAY because
    // GLX is not available, but EGL is used instead. If this happens,
    // we can usually proceed as function pointers are still loaded.
    if (result == GLEW_ERROR_NO_GLX_DISPLAY) {
      return;
    }
    std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
    throw std::runtime_error("Failed to initialize GLEW.");
  }
}

void Engine::setupOpenGL() {
  glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDepthMask(GL_TRUE);
  glDepthRange(0.0, 1.0);
  glClearDepth(1.0);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glViewport(0, 0, WindowWidth, WindowHeight);
}

void Engine::displayInfo() {
  std::cerr << "OpenGL Renderer: " << glGetString(GL_RENDERER) << " ("
            << glGetString(GL_VENDOR) << ")" << std::endl;
  std::cerr << "OpenGL version " << glGetString(GL_VERSION) << std::endl;
  std::cerr << "GLSL version " << glGetString(GL_SHADING_LANGUAGE_VERSION)
            << std::endl;
  std::cout << "GLEW version " << glewGetString(GLEW_VERSION) << std::endl;
  std::cout << "GLFW version " << glfwGetVersionString() << std::endl;
  std::cout << GLM_VERSION_MESSAGE << std::endl;
}

void Engine::init() {
  if (!GlApp) {
    throw std::runtime_error("No app set. Call setApp() before init().");
  }
  try {
    setupGLFW();
    setupGLEW();
    setupOpenGL();
#ifdef DEBUG
    displayInfo();
    setupDebugOutput();
#endif
    GlApp->initCallback(Window);
  } catch (...) {
    cleanup();
    throw;
  }
}

//////////////////////////////////////////////////////////////////////////// RUN

void Engine::run() {
  if (!GlApp) {
    throw std::runtime_error("No app set. Call setApp() before run().");
  }
  double last_time = glfwGetTime();
  try {
    while (!glfwWindowShouldClose(Window)) {
      double time = glfwGetTime();
      double elapsed_time = time - last_time;
      last_time = time;
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
              GL_STENCIL_BUFFER_BIT);
      GlApp->displayCallback(Window, elapsed_time);
      glfwSwapBuffers(Window);
      glfwPollEvents();
    }
  } catch (const std::exception &e) {
    std::cerr << "FRAME EXCEPTION: " << e.what() << std::endl;
    cleanup();
    throw;
  }
  cleanup();
}

////////////////////////////////////////////////////////////////////////////////
} // namespace mgl
