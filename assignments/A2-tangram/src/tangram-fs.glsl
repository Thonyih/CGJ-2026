#version 460 core

uniform vec4 Color; // colour of the piece being drawn

out vec4 outColor;

void main(void) {
    outColor = Color;
}
