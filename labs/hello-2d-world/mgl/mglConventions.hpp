////////////////////////////////////////////////////////////////////////////////
//
// Shader Naming Conventions
//
// Copyright (c)2022-26 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_CONVENTIONS_HPP
#define MGL_CONVENTIONS_HPP

namespace mgl {

////////////////////////////////////////////////////////////////////////////////

inline constexpr const char *MODEL_MATRIX = "ModelMatrix";
inline constexpr const char *NORMAL_MATRIX = "NormalMatrix";
inline constexpr const char *VIEW_MATRIX = "ViewMatrix";
inline constexpr const char *PROJECTION_MATRIX = "ProjectionMatrix";
inline constexpr const char *TEXTURE_MATRIX = "TextureMatrix";
inline constexpr const char *CAMERA_BLOCK = "Camera";

inline constexpr const char *POSITION_ATTRIBUTE = "inPosition";
inline constexpr const char *NORMAL_ATTRIBUTE = "inNormal";
inline constexpr const char *TEXCOORD_ATTRIBUTE = "inTexcoord";
inline constexpr const char *TANGENT_ATTRIBUTE = "inTangent";
inline constexpr const char *BITANGENT_ATTRIBUTE = "inBitangent";
inline constexpr const char *COLOR_ATTRIBUTE = "inColor";

////////////////////////////////////////////////////////////////////////////////
} // namespace mgl

#endif /* MGL_CONVENTIONS_HPP */
