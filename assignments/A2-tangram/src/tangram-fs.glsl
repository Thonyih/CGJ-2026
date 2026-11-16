#version 460 core

// FRAGMENT SHADER - runs once for every pixel inside the triangle being drawn.
// Its job: decide the pixel's colour.

// UNIFORM (the same for the whole piece, set by Piece::draw):
// the piece's colour as red, green, blue, alpha (each 0..1).
uniform vec4 Color;

// OUTPUT: the colour written to the screen.
out vec4 outColor;

void main(void) {
    // Every pixel of the piece gets the same colour.
    outColor = Color;
}
