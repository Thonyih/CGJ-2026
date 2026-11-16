#version 460 core

// Each vertex only has a position. The colour is the same for the whole
// piece, so it comes from a uniform in the fragment shader.

in vec4 inPosition;

uniform mat4 Matrix; // model matrix of the piece (scale, rotation, translation)

void main(void) {
    gl_Position = Matrix * inPosition;
}
