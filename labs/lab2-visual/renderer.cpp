#include "renderer.hpp"

#include <GL/glew.h>
#include <cmath>     // std::abs
#include <iostream>
#include <vector>

// One corner of a line (or one point): where it is and what colour it has.
struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};

// The shapes added this frame.
std::vector<Vertex> lineVertices;    // every 2 vertices make one line
std::vector<Vertex> pointVertices;   // every vertex is one dot

// OpenGL objects.
GLuint shaderProgram = 0;
GLuint vertexArray = 0;    // VAO: remembers how a Vertex is laid out
GLuint vertexBuffer = 0;   // VBO: holds the vertices on the graphics card

// ---------------------------------------------------------------------------
// Shaders: tiny programs that run on the graphics card.
// Written for GLSL 3.30 (OpenGL 3.3) so they also work on PCs without OpenGL 4.6.
// The vertex shader places each vertex on screen (multiplying by the camera
// matrix); the fragment shader colours the pixels.
// ---------------------------------------------------------------------------
const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
uniform mat4 viewProjection;
out vec3 color;
void main()
{
    gl_Position = viewProjection * vec4(inPosition, 1.0);
    color = inColor;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
in vec3 color;
out vec4 outColor;
void main()
{
    outColor = vec4(color, 1.0);
}
)";

// Compiles one shader and prints the error message if it fails.
GLuint compileShader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint ok = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, log);
        std::cerr << "Shader compile error:\n" << log << "\n";
    }
    return shader;
}

void initRenderer()
{
    // Build the shader program from the two shaders.
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);     // no longer needed once linked
    glDeleteShader(fragmentShader);

    // Create the buffer and describe the Vertex layout:
    //   attribute 0 = position (3 floats), attribute 1 = colour (3 floats).
    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &vertexBuffer);
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));
    glBindVertexArray(0);
}

void shutdownRenderer()
{
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteVertexArrays(1, &vertexArray);
    glDeleteProgram(shaderProgram);
}

// ---------------------------------------------------------------------------
// Adding shapes
// ---------------------------------------------------------------------------

void clearShapes()
{
    lineVertices.clear();
    pointVertices.clear();
}

void addLine(glm::vec3 from, glm::vec3 to, glm::vec3 color)
{
    Vertex start = { from, color };
    Vertex end = { to, color };
    lineVertices.push_back(start);
    lineVertices.push_back(end);
}

void addArrow(glm::vec3 from, glm::vec3 to, glm::vec3 color)
{
    addLine(from, to, color);

    float length = glm::length(to - from);
    if (length < 0.0001f) {
        return;   // too short to draw a head
    }

    // Direction of the arrow, and two directions perpendicular to it.
    glm::vec3 direction = (to - from) / length;
    glm::vec3 helper(0.0f, 1.0f, 0.0f);
    if (std::abs(direction.y) > 0.9f) {
        helper = glm::vec3(1.0f, 0.0f, 0.0f);   // avoid a helper parallel to the arrow
    }
    glm::vec3 side1 = glm::normalize(glm::cross(direction, helper));
    glm::vec3 side2 = glm::normalize(glm::cross(direction, side1));

    // The head: 4 short lines going back from the tip.
    float headSize = 0.15f * length;
    if (headSize > 0.25f) {
        headSize = 0.25f;
    }
    glm::vec3 back = to - direction * headSize;
    addLine(to, back + side1 * headSize * 0.5f, color);
    addLine(to, back - side1 * headSize * 0.5f, color);
    addLine(to, back + side2 * headSize * 0.5f, color);
    addLine(to, back - side2 * headSize * 0.5f, color);
}

void addPoint(glm::vec3 position, glm::vec3 color)
{
    Vertex point = { position, color };
    pointVertices.push_back(point);
}

void addAxes(float length)
{
    glm::vec3 origin(0.0f, 0.0f, 0.0f);
    addArrow(origin, glm::vec3(length, 0.0f, 0.0f), glm::vec3(0.6f, 0.2f, 0.2f));   // X
    addArrow(origin, glm::vec3(0.0f, length, 0.0f), glm::vec3(0.2f, 0.6f, 0.2f));   // Y
    addArrow(origin, glm::vec3(0.0f, 0.0f, length), glm::vec3(0.2f, 0.3f, 0.7f));   // Z
}

void addGrid2D(int halfSize)
{
    float size = (float)halfSize;
    for (int i = -halfSize; i <= halfSize; i++) {
        float position = (float)i;
        glm::vec3 color = DARK_GREY;
        if (i == 0) {
            color = GREY;   // the X and Y axes are brighter
        }
        addLine(glm::vec3(position, -size, 0.0f), glm::vec3(position, size, 0.0f), color);
        addLine(glm::vec3(-size, position, 0.0f), glm::vec3(size, position, 0.0f), color);
    }
}

// ---------------------------------------------------------------------------
// Drawing: send this frame's vertices to the graphics card and draw them.
// (Uploading everything every frame is slow, but simple, and fine here.)
// ---------------------------------------------------------------------------
void drawShapes(glm::mat4 viewProjection)
{
    glUseProgram(shaderProgram);
    GLint location = glGetUniformLocation(shaderProgram, "viewProjection");
    glUniformMatrix4fv(location, 1, GL_FALSE, &viewProjection[0][0]);

    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    // Lines
    if (!lineVertices.empty()) {
        glBufferData(GL_ARRAY_BUFFER, lineVertices.size() * sizeof(Vertex), lineVertices.data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_LINES, 0, (GLsizei)lineVertices.size());
    }

    // Points
    if (!pointVertices.empty()) {
        glPointSize(9.0f);
        glBufferData(GL_ARRAY_BUFFER, pointVertices.size() * sizeof(Vertex), pointVertices.data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_POINTS, 0, (GLsizei)pointVertices.size());
    }

    glBindVertexArray(0);
}
