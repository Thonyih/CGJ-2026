// A2-assignment.cpp

#include <cstddef>
#include <memory>

#include "../mgl/mgl.hpp" // IWYU pragma: keep

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {
public:
    MyApp() = default;
    ~MyApp() override = default;

    void initCallback(GLFWwindow *win) override;
    void displayCallback(GLFWwindow *win, double elapsed) override;
    void windowCloseCallback(GLFWwindow *win) override;

    void windowSizeCallback(GLFWwindow* win, int width, int height) override; //Don't want to touch the original mgl code

private:
    const GLuint POSITION = 0;

    std::unique_ptr<mgl::ShaderProgram> Shaders = nullptr;
    GLint MatrixId;
    GLint ColorId;

    void createShaderProgram();
    void createBufferObjects();
    void destroyBufferObjects();
    void drawScene();
};

//////////////////////////////////////////////////////////////////////// SHADERs

void MyApp::createShaderProgram() {
    Shaders = std::make_unique<mgl::ShaderProgram>();
    Shaders->addShader(GL_VERTEX_SHADER, "clip-vs.glsl");
    Shaders->addShader(GL_FRAGMENT_SHADER, "clip-fs.glsl");

    Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, POSITION);
    Shaders->addUniform("Matrix");
    Shaders->addUniform("Color");

    Shaders->create();

    MatrixId = Shaders->Uniforms["Matrix"].index;
    ColorId = Shaders->Uniforms["Color"].index;
}

//////////////////////////////////////////////////////////////////// VAOs & VBOs

struct Vertex {
    GLfloat XYZW[4];
};

const Vertex Vertices[] = {
    {{0.0f, 0.0f, 0.0f, 1.0f}},
    {{1.00f, 0.0f, 0.0f, 1.0f}},
    {{0.5f, 0.5f, 0.0f, 1.0f}}
};

const GLubyte Indices[] = {0, 1, 2};

const Vertex SquareVertices[] = {
    {{0.0f, 0.0f, 0.0f, 1.0f}},
    {{0.25f, 0.25f, 0.0f, 1.0f}},
    {{0.0f, 0.5f, 0.0f, 1.0f}},
    {{-0.25f, 0.25f, 0.0f, 1.0f}}};

const GLubyte SquareIndices[] = {0, 1, 2, 0, 2, 3};

const Vertex ParallelogramVertices[] = {
    { { 0.0f, 0.0f, 0.0f, 1.0f } },     // corner 0: bottom left  (0, 0)
    { { 0.5f, 0.0f, 0.0f, 1.0f } },     // corner 1: bottom right (0.5, 0)
    { { 0.75f, 0.25f, 0.0f, 1.0f } },   // corner 2: top right    (0.75, 0.25)
    { { 0.25f, 0.25f, 0.0f, 1.0f } } };  // corner 3: top left     (0.25, 0.25)

const GLubyte ParallelogramIndices[] = { 0, 1, 2, 0, 2, 3 };


////////////////////////////////////////////////////////////////////////// Class abstraction for drawing the objets
class Shape {
    public:
        // Make the VAO, VBO and EBO for one shape
        void create(const Vertex* vertices, int vertexCount,
            const GLubyte* indices, int indexCount) {
            IndexCount = indexCount;
            glGenVertexArrays(1, &VaoId);
            glBindVertexArray(VaoId);
            {
                glGenBuffers(1, &VboId);
                glBindBuffer(GL_ARRAY_BUFFER, VboId);
                {
                    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);
                    glEnableVertexAttribArray(POSITION);
                    glVertexAttribPointer(POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<GLvoid*>(0));
                }
                glGenBuffers(1, &EboId);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
                {
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLubyte), indices, GL_STATIC_DRAW);
                }
            }
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        // Draw this shape
        void draw() {
            glBindVertexArray(VaoId);
            glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_BYTE,
                reinterpret_cast<GLvoid*>(0));
            glBindVertexArray(0);
        }

        // Free the buffers
        void destroy() {
            glDeleteVertexArrays(1, &VaoId);
            glDeleteBuffers(1, &VboId);
            glDeleteBuffers(1, &EboId);
        }

    private:
        const GLuint POSITION = 0;
        GLuint VaoId = 0, VboId = 0, EboId = 0;
        int IndexCount = 0;
};

Shape Triangle, Square, Parallelogram;

void MyApp::createBufferObjects()
{
    Triangle.create(Vertices, 3, Indices, 3);
    Square.create(SquareVertices, 4, SquareIndices, 6);
    Parallelogram.create(ParallelogramVertices, 4, ParallelogramIndices, 6);
}

void MyApp::destroyBufferObjects()
{
    Triangle.destroy();
    Square.destroy();
    Parallelogram.destroy();
}

////////////////////////////////////////////////////////////////////////// SCENE

const float Q = 1.41421356f / 4.0f; // This number is the square root of 2 divided by 4, which is the length of the side of a triangle with a hypotenuse of length 0.5.

const glm::mat4 I(1.0f);

// The shark is way to big for the scene. Shrink it down and move it to the left and down a bit.
const glm::mat4 SCENE =
glm::scale(I, glm::vec3(0.7f, 0.7f, 1.0f)) *
glm::translate(I, glm::vec3(0.25f - 1.5f * Q, -0.25f, 0.0f));

glm::mat4 Aspect(1.0f);   // Stop my shark from streching when resising the window with the mouse

// Called by mgl everytime the window is resized, to keep the shark shape.
void MyApp::windowSizeCallback(GLFWwindow* win [[maybe_unused]], int width, int height) {
    float aspect = (float)width / (float)height;            // e.g. 800/600 = 1.33
    if (aspect >= 1.0f) {
        Aspect = glm::scale(I, glm::vec3(1.0f / aspect, 1.0f, 1.0f)); // wide: shrink x
    }
    else {
        Aspect = glm::scale(I, glm::vec3(1.0f, aspect, 1.0f));        // tall: shrink y
    }
}

glm::mat4 modelMatrix(float x, float y, float angle, float size) {
    glm::mat4 T = glm::translate(I, glm::vec3(x, y, 0.0f));
    glm::mat4 R = glm::rotate(I, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 S = glm::scale(I, glm::vec3(size, size, 1.0f));
    return Aspect * SCENE * T * R * S;
}

// A class to draw the objects in the scene, instead of just doing the code for each one.
class Piece {
public:
    // Remember everything about this piece
    Piece(Shape* shape, glm::vec4 color, float x, float y, float angle, float size) {
        BaseShape = shape;
        Color = color;
        X = x;
        Y = y;
        Angle = angle;
        Size = size;
    }

    // Send this piece's matrix and colour, then draw its shape
    void draw(GLint matrixId, GLint colorId) {
        glm::mat4 model = modelMatrix(X, Y, Angle, Size);
        glUniformMatrix4fv(matrixId, 1, GL_FALSE, glm::value_ptr(model));
        glUniform4fv(colorId, 1, glm::value_ptr(Color));
        BaseShape->draw();
    }

private:
    Shape* BaseShape;            // which of the 3 shapes (shared, not copied)
    glm::vec4 Color;             // red, green, blue, alpha
    float X, Y, Angle, Size;     // where corner 0 goes, how much to turn, how big
};

const float SQRT2 = 1.41421356f;

Piece Pieces[] = {
    //     shape           colour (R, G, B, A)                      x                y       angle    size
    Piece(&Triangle,      glm::vec4(0.07f, 0.51f, 0.94f, 1.0f),   -1.0f,           0.0f,      0.0f,  1.0f),         // head, blue
    Piece(&Triangle,      glm::vec4(0.80f, 0.06f, 0.39f, 1.0f),   -Q,              Q,       -45.0f,  1.0f),         // body, magenta
    Piece(&Triangle,      glm::vec4(0.00f, 0.61f, 0.65f, 1.0f),    Q,              2.0f * Q, -135.0f, 0.5f),        // top fin, teal
    Piece(&Triangle,      glm::vec4(0.92f, 0.27f, 0.16f, 1.0f),    0.0f,           0.0f,    180.0f,  0.5f),         // lower fin, red
    Piece(&Square,        glm::vec4(0.14f, 0.67f, 0.14f, 1.0f),    2.0f * Q,       0.0f,     45.0f,  1.0f),         // square, green
    Piece(&Parallelogram, glm::vec4(1.00f, 0.55f, 0.00f, 1.0f),    2.0f * Q,       0.0f,     45.0f,  1.0f),         // tail stalk, orange
    Piece(&Triangle,      glm::vec4(0.43f, 0.23f, 0.75f, 1.0f),    3.0f * Q + 0.5f, Q,      135.0f,  SQRT2 / 2.0f)  // tail, purple
};


void MyApp::drawScene() {
  // Drawing directly in clip space

    Shaders->bind();
    for (int i = 0; i < 7; i++) {
        Pieces[i].draw(MatrixId, ColorId);   // each piece sends its own matrix + colour
    }
    Shaders->unbind();
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win [[maybe_unused]]) {
    createBufferObjects();
    createShaderProgram();
}

void MyApp::windowCloseCallback(GLFWwindow *win [[maybe_unused]]) {
    destroyBufferObjects();
}

void MyApp::displayCallback(GLFWwindow *win [[maybe_unused]],
                            double elapsed [[maybe_unused]]) {
    drawScene();
}

/////////////////////////////////////////////////////////////////////////// MAIN

int main() {
    mgl::Engine &engine = mgl::Engine::getInstance();
    engine.setApp(std::make_unique<MyApp>());
    engine.setOpenGL(4, 6);
    engine.setWindow(600, 600, "A2 - Tangram - Group 3", 0, 1);
    engine.init();
    engine.run();
    return EXIT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////// END
