#version 460 core

// Fragment shader: the piece's colour, made lighter or darker per face.
//
// Each face of a piece has its own normal (the meshes are flat-shaded), so a
// dot product of the normal with a fixed direction gives each face a different
// number. That number scales the colour between 55% and 95%, so every face is
// a different tint of the same colour and the 3D shape is visible without
// any lighting. The normal is in model space, so a face keeps its tint while
// the piece moves.

in vec3 exNormal;
out vec4 FragmentColor;

uniform vec4 Color; // the piece's colour (one per scene node)

void main(void) {
  vec3 N = normalize(exNormal);
  float tint = 0.75 + 0.2 * dot(N, normalize(vec3(0.2, 0.5, 0.8)));
  FragmentColor = vec4(Color.rgb * tint, 1.0);
}
