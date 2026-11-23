////////////////////////////////////////////////////////////////////////////////
//
// Mesh Loader Class
//
// Copyright (c)2022-26 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#include "./mglMesh.hpp"

#include <cassert>
#include <iostream> // IWYU pragma: keep
#include <stdexcept>

namespace mgl {

////////////////////////////////////////////////////////////////////////////////

Mesh &Mesh::operator=(Mesh &&other) noexcept {
  if (this != &other) {
    destroyBufferObjects();

    VaoId = other.VaoId;
    AssimpFlags = other.AssimpFlags;
    NormalsLoaded = other.NormalsLoaded;
    TexcoordsLoaded = other.TexcoordsLoaded;
    TangentsAndBitangentsLoaded = other.TangentsAndBitangentsLoaded;
    Meshes = std::move(other.Meshes);
    Positions = std::move(other.Positions);
    Normals = std::move(other.Normals);
    Texcoords = std::move(other.Texcoords);
    Tangents = std::move(other.Tangents);
#ifdef CREATE_BITANGENT
    Bitangents = std::move(other.Bitangents);
#endif
    Indices = std::move(other.Indices);

    other.VaoId = 0;
  }
  return *this;
}

Mesh::Mesh(Mesh &&other) noexcept
    : VaoId(other.VaoId), AssimpFlags(other.AssimpFlags),
      NormalsLoaded(other.NormalsLoaded),
      TexcoordsLoaded(other.TexcoordsLoaded),
      TangentsAndBitangentsLoaded(other.TangentsAndBitangentsLoaded),
      Meshes(std::move(other.Meshes)), Positions(std::move(other.Positions)),
      Normals(std::move(other.Normals)), Texcoords(std::move(other.Texcoords)),
      Tangents(std::move(other.Tangents)),
#ifdef CREATE_BITANGENT
      Bitangents(std::move(other.Bitangents)),
#endif
      Indices(std::move(other.Indices)) {
  other.VaoId = 0;
}

Mesh::~Mesh() { destroyBufferObjects(); }

void Mesh::setAssimpFlags(unsigned int flags) {
  AssimpFlags = flags | aiProcess_Triangulate;
}

void Mesh::joinIdenticalVertices() {
  AssimpFlags |= aiProcess_JoinIdenticalVertices;
}

void Mesh::generateNormals() { AssimpFlags |= aiProcess_GenNormals; }

void Mesh::generateSmoothNormals() {
  AssimpFlags |= aiProcess_GenSmoothNormals;
}

void Mesh::generateTexcoords() { AssimpFlags |= aiProcess_GenUVCoords; }

void Mesh::calculateTangentSpace() {
  AssimpFlags |= aiProcess_CalcTangentSpace;
}

void Mesh::flipUVs() { AssimpFlags |= aiProcess_FlipUVs; }

////////////////////////////////////////////////////////////////////////////////

void Mesh::processMesh(const aiMesh *mesh) {

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    const aiVector3D &aiPosition = mesh->mVertices[i];
    Positions.push_back(glm::vec3(aiPosition.x, aiPosition.y, aiPosition.z));
    if (NormalsLoaded) {
      const aiVector3D &aiNormal = mesh->mNormals[i];
      Normals.push_back(glm::vec3(aiNormal.x, aiNormal.y, aiNormal.z));
    }
    if (TexcoordsLoaded) {
      const aiVector3D &aiTexcoord = mesh->mTextureCoords[0][i];
      Texcoords.push_back(glm::vec2(aiTexcoord.x, aiTexcoord.y));
    }
    if (TangentsAndBitangentsLoaded) {
      const aiVector3D &aiTangent = mesh->mTangents[i];
      Tangents.push_back(glm::vec3(aiTangent.x, aiTangent.y, aiTangent.z));

#ifdef CREATE_BITANGENT
      const aiVector3D &aiBitangent = mesh->mBitangents[i];
      Bitangents.push_back(
          glm::vec3(aiBitangent.x, aiBitangent.y, aiBitangent.z));
#endif
    }
  }
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    const aiFace &face = mesh->mFaces[i];
    if (face.mNumIndices != 3) {
      throw std::runtime_error("Non-triangular face encountered in mesh");
    }
    Indices.push_back(face.mIndices[0]);
    Indices.push_back(face.mIndices[1]);
    Indices.push_back(face.mIndices[2]);
  }
}

void Mesh::clear() {
  Positions.clear();
  Normals.clear();
  Texcoords.clear();
  Tangents.clear();
#ifdef CREATE_BITANGENT
  Bitangents.clear();
#endif
  Indices.clear();
  Meshes.clear();

  NormalsLoaded = false;
  TexcoordsLoaded = false;
  TangentsAndBitangentsLoaded = false;
}

void Mesh::processScene(const aiScene *scene) {

  if (scene->mNumMeshes == 0) {
    return;
  }

  // Assuming same structure over all meshes
  NormalsLoaded = scene->mMeshes[0]->HasNormals();
  // Check if mesh has texture coordinates in the primary (0th) UV channel
  TexcoordsLoaded = scene->mMeshes[0]->HasTextureCoords(0);
  TangentsAndBitangentsLoaded = scene->mMeshes[0]->HasTangentsAndBitangents();

  Meshes.resize(scene->mNumMeshes);
  unsigned int n_vertices = 0;
  unsigned int n_indices = 0;

  for (unsigned int i = 0; i < Meshes.size(); i++) {
    const aiMesh *mesh = scene->mMeshes[i];

    if (mesh->HasNormals() != NormalsLoaded ||
        mesh->HasTextureCoords(0) != TexcoordsLoaded ||
        mesh->HasTangentsAndBitangents() != TangentsAndBitangentsLoaded) {
      throw std::runtime_error(
          "Inconsistent vertex attributes across sub-meshes");
    }
    // Assuming all mesh faces are triangles
    Meshes[i].nIndices = mesh->mNumFaces * 3;
    Meshes[i].baseVertex = n_vertices;
    Meshes[i].baseIndex = n_indices;

    n_vertices += mesh->mNumVertices;
    n_indices += Meshes[i].nIndices;
  }

  Positions.reserve(n_vertices);
  if (NormalsLoaded)
    Normals.reserve(n_vertices);
  if (TexcoordsLoaded)
    Texcoords.reserve(n_vertices);
  if (TangentsAndBitangentsLoaded) {
    Tangents.reserve(n_vertices);
#ifdef CREATE_BITANGENT
    Bitangents.reserve(n_vertices);
#endif
  }
  Indices.reserve(n_indices);

  for (unsigned int i = 0; i < Meshes.size(); i++) {
    processMesh(scene->mMeshes[i]);
  }

#ifdef DEBUG
  std::cout << "Loaded " << Meshes.size() << " mesh(es) [" << n_vertices
            << " vertices, " << n_indices << " indices, " << n_indices / 3
            << " triangles]" << std::endl;
#endif
}

void Mesh::create(const std::string &filename) {
  clear();
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(filename, AssimpFlags);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    throw std::runtime_error(std::string("Error while loading: ") +
                             importer.GetErrorString());
  }

#ifdef DEBUG
  std::cout << "Processing [" << filename << "]" << std::endl;
#endif

  processScene(scene);
  createBufferObjects();
}

void Mesh::createBufferObjects() {

  if (Positions.empty() || Indices.empty()) {
    return;
  }
  assert(glfwGetCurrentContext() != nullptr &&
         "OpenGL context must be active before creating Mesh buffers.");
  destroyBufferObjects();

#ifdef CREATE_BITANGENT
  constexpr GLsizei NUM_BUFFERS = 6;
#else
  constexpr GLsizei NUM_BUFFERS = 5;
#endif
  GLuint boId[NUM_BUFFERS];

  glGenVertexArrays(1, &VaoId);
  glBindVertexArray(VaoId);
  {
    glGenBuffers(NUM_BUFFERS, boId);

    glBindBuffer(GL_ARRAY_BUFFER, boId[POSITION]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(),
                 Positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION);
    glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    if (NormalsLoaded) {
      glBindBuffer(GL_ARRAY_BUFFER, boId[NORMAL]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(),
                   Normals.data(), GL_STATIC_DRAW);
      glEnableVertexAttribArray(NORMAL);
      glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    if (TexcoordsLoaded) {
      glBindBuffer(GL_ARRAY_BUFFER, boId[TEXCOORD]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(Texcoords[0]) * Texcoords.size(),
                   Texcoords.data(), GL_STATIC_DRAW);
      glEnableVertexAttribArray(TEXCOORD);
      glVertexAttribPointer(TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }

    if (TangentsAndBitangentsLoaded) {
      glBindBuffer(GL_ARRAY_BUFFER, boId[TANGENT]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(Tangents[0]) * Tangents.size(),
                   Tangents.data(), GL_STATIC_DRAW);
      glEnableVertexAttribArray(TANGENT);
      glVertexAttribPointer(TANGENT, 3, GL_FLOAT, GL_FALSE, 0, 0);

#ifdef CREATE_BITANGENT
      glBindBuffer(GL_ARRAY_BUFFER, boId[BITANGENT]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(Bitangents[0]) * Bitangents.size(),
                   Bitangents.data(), GL_STATIC_DRAW);
      glEnableVertexAttribArray(BITANGENT);
      glVertexAttribPointer(BITANGENT, 3, GL_FLOAT, GL_FALSE, 0, 0);
#endif
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boId[INDEX]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(),
                 Indices.data(), GL_STATIC_DRAW);
  }
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(NUM_BUFFERS, boId);
}

void Mesh::destroyBufferObjects() {
  if (VaoId != 0) {
    if (glfwGetCurrentContext() != nullptr) {
      glDeleteVertexArrays(1, &VaoId);
    }
    VaoId = 0;
  }
}

void Mesh::draw() {

  if (VaoId == 0) {
    return;
  }

  glBindVertexArray(VaoId);
  for (const MeshData &mesh : Meshes) {
    if (mesh.nIndices == 0) {
      continue;
    }
    glDrawElementsBaseVertex(
        GL_TRIANGLES, mesh.nIndices, GL_UNSIGNED_INT,
        reinterpret_cast<void *>(sizeof(unsigned int) * mesh.baseIndex),
        mesh.baseVertex);
    // GLenum mode, GLsizei count, GLenum type, void *indices, GLint
    // basevertex
  }
  glBindVertexArray(0);
}

////////////////////////////////////////////////////////////////////////////////
} // namespace mgl
