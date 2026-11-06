#pragma once

// A very small line renderer.
// Every frame: clearShapes(), then add lines/arrows/points, then drawShapes().

#include <glm/glm.hpp>

// Some colours (red, green, blue between 0 and 1).
const glm::vec3 RED(1.0f, 0.35f, 0.35f);
const glm::vec3 GREEN(0.35f, 1.0f, 0.35f);
const glm::vec3 BLUE(0.4f, 0.6f, 1.0f);
const glm::vec3 YELLOW(1.0f, 1.0f, 0.3f);
const glm::vec3 ORANGE(1.0f, 0.6f, 0.2f);
const glm::vec3 CYAN(0.3f, 1.0f, 1.0f);
const glm::vec3 MAGENTA(1.0f, 0.4f, 1.0f);
const glm::vec3 WHITE(1.0f, 1.0f, 1.0f);
const glm::vec3 GREY(0.5f, 0.5f, 0.5f);
const glm::vec3 DARK_GREY(0.25f, 0.25f, 0.25f);

void initRenderer();      // call once, after OpenGL is ready
void shutdownRenderer();  // call once, before closing the window

void clearShapes();
void addLine(glm::vec3 from, glm::vec3 to, glm::vec3 color);
void addArrow(glm::vec3 from, glm::vec3 to, glm::vec3 color);   // line with an arrow head
void addPoint(glm::vec3 position, glm::vec3 color);
void addAxes(float length);                                     // X red, Y green, Z blue
void addGrid2D(int halfSize);                                   // grid on the XY plane

void drawShapes(glm::mat4 viewProjection);
