#version 460 core

// VERTEX SHADER - runs once for every corner (vertex) of the piece being drawn.
// Its job: decide where the corner goes in clip space (the -1..+1 box).

// INPUT (one per corner, from the VBO): the corner's position (x, y, z, w).
// It is linked to attribute number 0 in tangram.cpp (POSITION).
in vec4 inPosition;

// UNIFORM (the same for every corner of one piece, set by Piece::draw):
// the piece's model matrix, SCENE * T * R * S (scale, rotate, translate).
uniform mat4 Matrix;

void main(void) {
    // Move the corner to its place. gl_Position is OpenGL's built-in output.
    gl_Position = Matrix * inPosition;
}
