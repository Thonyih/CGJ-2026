////////////////////////////////////////////////////////////////////////////////
//
// Shader Program Class
//
// Copyright (c)2022-26 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#include "./mglShader.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace mgl {

////////////////////////////////////////////////////////////////// ShaderProgram

const std::string ShaderProgram::read(const std::string &filename) {
  std::ifstream ifile(filename);
  if (!ifile.is_open()) {
    std::cerr << "[ERROR] Failed to open shader file: " << filename
              << std::endl;
    throw std::runtime_error("Failed to open shader file.");
  }
  std::stringstream buffer;
  buffer << ifile.rdbuf();
  return buffer.str();
}

void ShaderProgram::checkCompilation(const GLuint shader_id,
                                     const std::string &filename) {
  GLint compiled;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);
  if (compiled == GL_FALSE) {
    GLint length;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
    if (length > 0) {
      std::vector<char> log(length);
      glGetShaderInfoLog(shader_id, length, &length, log.data());
      std::cerr << "[" << filename << "] " << std::endl
                << log.data() << std::endl;
    }
    throw std::runtime_error("Failed to compile shader.");
  }
}

void ShaderProgram::checkLinkage() {
  GLint linked;
  glGetProgramiv(ProgramId, GL_LINK_STATUS, &linked);
  if (linked == GL_FALSE) {
    GLint length;
    glGetProgramiv(ProgramId, GL_INFO_LOG_LENGTH, &length);
    if (length > 0) {
      std::vector<char> log(length);
      glGetProgramInfoLog(ProgramId, length, &length, log.data());
      std::cerr << "[LINK] " << std::endl << log.data() << std::endl;
    }
    throw std::runtime_error("Failed to link shader program.");
  }
}

ShaderProgram::ShaderProgram() {
  assert(glfwGetCurrentContext() != nullptr &&
         "OpenGL context must be active before creating a ShaderProgram.");
  ProgramId = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
  if (glfwGetCurrentContext() != nullptr) {
    for (auto &[type, info] : Shaders) {
      if (ProgramId != 0) {
        glDetachShader(ProgramId, info.index);
      }
      glDeleteShader(info.index);
    }
    if (ProgramId != 0) {
      // Only unbind if THIS program is currently active
      GLint current_program = 0;
      glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);
      if (static_cast<GLuint>(current_program) == ProgramId) {
        glUseProgram(0);
      }
      glDeleteProgram(ProgramId);
    }
  }
  ProgramId = 0;
}

void ShaderProgram::addShader(const GLenum shader_type,
                              const std::string &filename) {
  const GLuint shader_id = glCreateShader(shader_type);
  try {
    std::string scode = read(filename);
    const GLchar *code = scode.c_str();
    glShaderSource(shader_id, 1, &code, nullptr);
    glCompileShader(shader_id);
    checkCompilation(shader_id, filename);

    // If a shader of this type was already added, detach and delete it first
    auto it = Shaders.find(shader_type);
    if (it != Shaders.end()) {
      glDetachShader(ProgramId, it->second.index);
      glDeleteShader(it->second.index);
    }

    glAttachShader(ProgramId, shader_id);
    Shaders[shader_type] = {shader_id};
  } catch (...) {
    glDeleteShader(shader_id);
    throw;
  }
}

void ShaderProgram::addAttribute(const std::string &name, const GLuint index) {
#ifdef DEBUG
  bool failed = isAttribute(name);
  if (failed) {
    std::cerr << "[ASSERTION FAILED] Attribute '" << name
              << "' is already registered in ShaderProgram." << std::endl;
  }
  assert(!failed && "Attribute already registered in ShaderProgram.");
#endif
  // not required if using layout(location = X)
  glBindAttribLocation(ProgramId, index, name.c_str());
  Attributes[name] = {index};
}

bool ShaderProgram::isAttribute(const std::string &name) const {
  return Attributes.find(name) != Attributes.end();
}

void ShaderProgram::addUniform(const std::string &name) {
#ifdef DEBUG
  bool failed = isUniform(name);
  if (failed) {
    std::cerr << "[ASSERTION FAILED] Uniform '" << name
              << "' is already registered in ShaderProgram." << std::endl;
  }
  assert(!failed && "Uniform already registered in ShaderProgram.");
#endif
  Uniforms[name] = {-1};
}

bool ShaderProgram::isUniform(const std::string &name) const {
  return Uniforms.find(name) != Uniforms.end();
}

void ShaderProgram::addUniformBlock(const std::string &name,
                                    const GLuint binding_point) {
#ifdef DEBUG
  bool failed = isUniformBlock(name);
  if (failed) {
    std::cerr << "[ASSERTION FAILED] Uniform block '" << name
              << "' is already registered in ShaderProgram." << std::endl;
  }
  assert(!failed && "Uniform block already registered in ShaderProgram.");
#endif
  Ubos[name] = {0, binding_point};
}

bool ShaderProgram::isUniformBlock(const std::string &name) const {
  return Ubos.find(name) != Ubos.end();
}

void ShaderProgram::create() {
  glLinkProgram(ProgramId);
  for (auto &[type, info] : Shaders) {
    glDetachShader(ProgramId, info.index);
    glDeleteShader(info.index);
  }
  Shaders.clear();
  checkLinkage();

  for (auto &[name, info] : Uniforms) {
    info.index = glGetUniformLocation(ProgramId, name.c_str());
    if (info.index < 0)
      std::cerr << "WARNING: Uniform " << name << " not found." << std::endl;
  }
  for (auto &[name, info] : Ubos) {
    info.index = glGetUniformBlockIndex(ProgramId, name.c_str());
    if (info.index == GL_INVALID_INDEX)
      std::cerr << "WARNING: UBO " << name << " not found." << std::endl;
    else
      glUniformBlockBinding(ProgramId, info.index, info.binding_point);
  }
}

void ShaderProgram::bind() { glUseProgram(ProgramId); }

void ShaderProgram::unbind() { glUseProgram(0); }

////////////////////////////////////////////////////////////////////////////////
} // namespace mgl
