#version 460 core

// Vertex shader: same as the lecture's (hello-3d-world / mesh-loader).
// The mesh gives each corner a position and a normal (Mesh::POSITION = 1,
// Mesh::NORMAL = 2). The camera's matrices come from the "Camera" block (UBO).

layout(location = 1) in vec3 inPosition;
layout(location = 2) in vec3 inNormal;

out vec3 exNormal; // the face's normal, in model space, for the tint

uniform mat4 ModelMatrix;

uniform Camera {
   mat4 ViewMatrix;
   mat4 ProjectionMatrix;
};

void main(void) {
  exNormal = inNormal;
  gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inPosition, 1.0);
}
