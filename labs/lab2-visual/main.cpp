// Lab 2 - GLM, visual version (separate program from labs/lab2-glm)
// Opens an OpenGL window and draws what each exercise computes.
//
// Keys:  1-5 = choose exercise   Arrows = rotate the camera
//        Space = new random values   R = reset camera   Esc = quit

#include <GL/glew.h>      // must come before GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>   // glm::perspective, glm::lookAt, glm::ortho

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

#include "renderer.hpp"
#include "scenes.hpp"

// What the user has chosen.
int currentScene = 1;
bool sceneChanged = true;

// Camera angles (in degrees) for the 3D scenes.
float cameraYaw = 35.0f;     // turning left / right
float cameraPitch = 25.0f;   // looking from above / below

// Puts the camera back at the starting angle for the current scene.
void resetCamera()
{
    if (currentScene == 4) {
        // Exercise 4: look from the side, otherwise b points straight at the camera.
        cameraYaw = -60.0f;
        cameraPitch = 10.0f;
    } else {
        cameraYaw = 35.0f;
        cameraPitch = 25.0f;
    }
}

// Called by GLFW whenever a key is pressed.
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS) {
        return;
    }
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key >= GLFW_KEY_1 && key <= GLFW_KEY_5) {
        currentScene = key - GLFW_KEY_1 + 1;
        sceneChanged = true;
        resetCamera();
    }
    if (key == GLFW_KEY_SPACE) {
        newRandomValues();
        sceneChanged = true;
    }
    if (key == GLFW_KEY_R) {
        resetCamera();
    }
}

// Called by GLFW when the window is resized.
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Builds the camera matrix (view * projection) for the current scene.
glm::mat4 makeViewProjection(int width, int height)
{
    float aspect = (float)width / (float)height;

    if (sceneIs2D(currentScene)) {
        // 2D: look straight at the XY plane, showing x and y from about -5 to 5.
        float halfHeight = 4.0f;
        float halfWidth = halfHeight * aspect;
        return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1.0f, 1.0f);
    }

    // 3D: the camera sits on a sphere around the origin, looking at the origin.
    float distance = sceneCameraDistance(currentScene);
    float yaw = glm::radians(cameraYaw);
    float pitch = glm::radians(cameraPitch);
    glm::vec3 eye(distance * std::cos(pitch) * std::sin(yaw),
                  distance * std::sin(pitch),
                  distance * std::cos(pitch) * std::cos(yaw));

    glm::mat4 view = glm::lookAt(eye, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
    return projection * view;
}

int main()
{
    srand((unsigned int)time(nullptr));
    newRandomValues();

    // --- Window and OpenGL 4.6 context ---
    if (!glfwInit()) {
        std::cerr << "Failed to initialise GLFW\n";
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);   // smoother lines

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Lab 2 - GLM visual", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create the window\n";
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // --- GLEW: load the OpenGL functions ---
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialise GLEW\n";
        glfwTerminate();
        return 1;
    }
    glGetError();   // glewInit can leave a harmless error behind; clear it

    std::cout << "OpenGL " << glGetString(GL_VERSION) << "\n";
    std::cout << "Keys: 1-5 exercise, arrows rotate camera, Space new random values, R reset camera, Esc quit\n";

    initRenderer();
    glEnable(GL_MULTISAMPLE);

    double lastTime = glfwGetTime();

    // --- Main loop: runs once per frame ---
    while (!glfwWindowShouldClose(window)) {
        // Time since the last frame, so the camera turns at the same speed on any PC.
        double now = glfwGetTime();
        float deltaTime = (float)(now - lastTime);
        lastTime = now;

        // Arrow keys turn the camera while held down (90 degrees per second).
        float turnSpeed = 90.0f * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)  cameraYaw -= turnSpeed;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) cameraYaw += turnSpeed;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)    cameraPitch += turnSpeed;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)  cameraPitch -= turnSpeed;
        if (cameraPitch > 89.0f)  cameraPitch = 89.0f;    // do not flip over the top
        if (cameraPitch < -89.0f) cameraPitch = -89.0f;

        // When the scene changes: update the title and explain the colours.
        if (sceneChanged) {
            std::string title = "Lab 2 - GLM visual  |  " + sceneTitle(currentScene) +
                                "  |  1-5 exercise, arrows rotate, Space random, R reset";
            glfwSetWindowTitle(window, title.c_str());
            printSceneLegend(currentScene);
            sceneChanged = false;
        }

        // Draw
        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        if (width > 0 && height > 0) {
            glClearColor(0.08f, 0.08f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // 3D scenes: closer lines hide farther ones (depth test).
            // 2D scene: everything is flat, so just draw in order (squares over the grid).
            if (sceneIs2D(currentScene)) {
                glDisable(GL_DEPTH_TEST);
            } else {
                glEnable(GL_DEPTH_TEST);
            }

            clearShapes();
            buildScene(currentScene);
            drawShapes(makeViewProjection(width, height));
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shutdownRenderer();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
