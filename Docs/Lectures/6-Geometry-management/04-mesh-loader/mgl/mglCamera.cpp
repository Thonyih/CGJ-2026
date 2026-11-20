////////////////////////////////////////////////////////////////////////////////
//
// Camera Abstraction Class
//
// Copyright (c)2023-26 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#include "./mglCamera.hpp"

#include <cassert>
#include <glm/gtc/type_ptr.hpp>
#include <utility>

namespace mgl {

///////////////////////////////////////////////////////////////////////// Camera

Camera::Camera(GLuint bindingpoint) {
  assert(glfwGetCurrentContext() != nullptr &&
         "OpenGL context must be active before creating a Camera.");
  glGenBuffers(1, &UboId);
  glBindBuffer(GL_UNIFORM_BUFFER, UboId);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, nullptr,
               GL_STREAM_DRAW);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
                  glm::value_ptr(ViewMatrix));
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
                  glm::value_ptr(ProjectionMatrix));
  glBindBufferBase(GL_UNIFORM_BUFFER, bindingpoint, UboId);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Camera::~Camera() {
  if (UboId != 0 && glfwGetCurrentContext() != nullptr) {
    glDeleteBuffers(1, &UboId);
  }
}

Camera::Camera(Camera &&other) noexcept
    : UboId(std::exchange(other.UboId, 0)), ViewMatrix(other.ViewMatrix),
      ProjectionMatrix(other.ProjectionMatrix) {}

Camera &Camera::operator=(Camera &&other) noexcept {
  if (this != &other) {
    if (UboId != 0 && glfwGetCurrentContext() != nullptr) {
      glDeleteBuffers(1, &UboId);
    }
    UboId = std::exchange(other.UboId, 0);
    ViewMatrix = other.ViewMatrix;
    ProjectionMatrix = other.ProjectionMatrix;
  }
  return *this;
}
const glm::mat4 &Camera::getViewMatrix() const { return ViewMatrix; }

void Camera::setViewMatrix(const glm::mat4 &viewmatrix) {
  ViewMatrix = viewmatrix;
  glBindBuffer(GL_UNIFORM_BUFFER, UboId);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
                  glm::value_ptr(ViewMatrix));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

const glm::mat4 &Camera::getProjectionMatrix() const {
  return ProjectionMatrix;
}

void Camera::setProjectionMatrix(const glm::mat4 &projectionmatrix) {
  ProjectionMatrix = projectionmatrix;
  glBindBuffer(GL_UNIFORM_BUFFER, UboId);
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
                  glm::value_ptr(ProjectionMatrix));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

////////////////////////////////////////////////////////////////////////////////
} // namespace mgl
