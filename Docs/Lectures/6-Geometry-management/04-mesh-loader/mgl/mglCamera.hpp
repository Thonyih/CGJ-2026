////////////////////////////////////////////////////////////////////////////////
//
// Camera Abstraction Class
//
// Copyright (c)2023-26 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_CAMERA_HPP
#define MGL_CAMERA_HPP

#include <glm/glm.hpp>

#include "./mglGL.hpp" // IWYU pragma: keep

namespace mgl {

class Camera;

///////////////////////////////////////////////////////////////////////// Camera
// layout(std140) uniform Camera {
//   mat4 ViewMatrix;
//   mat4 ProjectionMatrix;
// };

class Camera {
private:
  GLuint UboId = 0;
  glm::mat4 ViewMatrix{1.0f};
  glm::mat4 ProjectionMatrix{1.0f};

public:
  explicit Camera(GLuint bindingpoint);
  virtual ~Camera();

  Camera(const Camera &) = delete;
  Camera &operator=(const Camera &) = delete;
  Camera(Camera &&other) noexcept;
  Camera &operator=(Camera &&other) noexcept;

  const glm::mat4 &getViewMatrix() const;
  void setViewMatrix(const glm::mat4 &viewmatrix);
  const glm::mat4 &getProjectionMatrix() const;
  void setProjectionMatrix(const glm::mat4 &projectionmatrix);
};

////////////////////////////////////////////////////////////////////////////////
} // namespace mgl

#endif /* MGL_CAMERA_HPP */
