#include "scenes.hpp"
#include "renderer.hpp"

// The maths: a copy of Lab 2's exercises.cpp / helpers.cpp (same functions the console program tests).
#include "exercises.hpp"
#include "helpers.hpp"

#include <iostream>

// Random values used by Exercises 1 and 3 (changed with the Space key).
glm::vec3 randomU, randomV, randomW;   // Exercise 1
glm::vec3 randomP;                      // Exercise 3

void newRandomValues()
{
    randomU = randomVec3();
    randomV = randomVec3();
    randomW = randomVec3();
    randomP = randomVec3();
}

// ===========================================================================
// Exercise 1: u x (v + w) = u x v + u x w
// Both sides are drawn; their tips land on the same spot.
// ===========================================================================
void buildScene1()
{
    glm::vec3 origin(0.0f);
    glm::vec3 u = randomU;
    glm::vec3 v = randomV;
    glm::vec3 w = randomW;

    // Inputs
    addArrow(origin, u, RED);
    addArrow(origin, v, GREEN);
    addArrow(origin, w, BLUE);
    addArrow(origin, v + w, CYAN);          // v + w
    addLine(v, v + w, DARK_GREY);           // shows how v + w is built
    addLine(w, v + w, DARK_GREY);

    // Left side: u x (v + w)
    glm::vec3 leftSide = glm::cross(u, v + w);
    addArrow(origin, leftSide, YELLOW);

    // Right side: u x v, then u x w added on the end of it.
    glm::vec3 uCrossV = glm::cross(u, v);
    glm::vec3 uCrossW = glm::cross(u, w);
    addArrow(origin, uCrossV, ORANGE);
    addArrow(uCrossV, uCrossV + uCrossW, MAGENTA);

    // Tips of both sides (they overlap when the property holds).
    addPoint(leftSide, YELLOW);
    addPoint(uCrossV + uCrossW, WHITE);
}

// ===========================================================================
// Exercise 2: Rx = 45 deg and Ry = 90 deg applied to u, v, w
// ===========================================================================
void buildScene2()
{
    glm::vec3 origin(0.0f);
    glm::vec3 vectors[3] = {
        glm::vec3(1.0f, 1.0f, 1.0f),     // u
        glm::vec3(-1.0f, -1.0f, 0.0f),   // v
        glm::vec3(1.0f, 0.0f, 0.0f)      // w
    };

    glm::mat3 Rx = rotationX(45.0f);
    glm::mat3 Ry = rotationY(90.0f);

    for (int i = 0; i < 3; i++) {
        glm::vec3 original = vectors[i];
        addArrow(origin, original, WHITE);       // before
        addArrow(origin, Rx * original, ORANGE); // after Rx
        addArrow(origin, Ry * original, CYAN);   // after Ry
    }
}

// ===========================================================================
// Exercise 3: 3D rotations are not commutative.
// Three copies of a "die" (a cube with its own little axes):
//   left = original, middle = rotate about x then y, right = y then x.
// ===========================================================================

// Draws a unit cube, its local axes and the point p, all transformed by
// "rotation" and moved to "center".
void addRotatedDie(glm::vec3 center, glm::mat3 rotation, glm::vec3 p)
{
    // The 8 corners of a cube from -0.5 to 0.5.
    glm::vec3 corners[8];
    int index = 0;
    for (int x = 0; x < 2; x++) {
        for (int y = 0; y < 2; y++) {
            for (int z = 0; z < 2; z++) {
                glm::vec3 corner(x - 0.5f, y - 0.5f, z - 0.5f);
                corners[index] = center + rotation * corner;
                index++;
            }
        }
    }

    // The 12 edges: connect corners that differ in exactly one coordinate.
    // (Corner index bits: x = 4, y = 2, z = 1.)
    for (int a = 0; a < 8; a++) {
        for (int b = a + 1; b < 8; b++) {
            int difference = a ^ b;
            if (difference == 1 || difference == 2 || difference == 4) {
                addLine(corners[a], corners[b], GREY);
            }
        }
    }

    // The die's own axes, so you can see which way it is facing.
    addArrow(center, center + rotation * glm::vec3(0.9f, 0.0f, 0.0f), RED);
    addArrow(center, center + rotation * glm::vec3(0.0f, 0.9f, 0.0f), GREEN);
    addArrow(center, center + rotation * glm::vec3(0.0f, 0.0f, 0.9f), BLUE);

    // The random point, rotated together with the die.
    addPoint(center + rotation * (p * 0.5f), YELLOW);
}

void buildScene3()
{
    glm::mat3 Rx = rotationX(90.0f);
    glm::mat3 Ry = rotationY(90.0f);
    glm::mat3 noRotation(1.0f);

    addRotatedDie(glm::vec3(-2.5f, 0.0f, 0.0f), noRotation, randomP);   // original
    addRotatedDie(glm::vec3(0.0f, 0.0f, 0.0f), Ry * Rx, randomP);       // x then y
    addRotatedDie(glm::vec3(2.5f, 0.0f, 0.0f), Rx * Ry, randomP);       // y then x
}

// ===========================================================================
// Exercise 4: plane with basis a = (1, 2, 3), b = (4, 5, 6) and its normal.
// ===========================================================================
void buildScene4()
{
    glm::vec3 origin(0.0f);
    glm::vec3 a(1.0f, 2.0f, 3.0f);
    glm::vec3 b(4.0f, 5.0f, 6.0f);

    // The plane, drawn as the parallelogram made by a and b.
    addArrow(origin, a, RED);
    addArrow(origin, b, GREEN);
    addLine(a, a + b, DARK_GREY);
    addLine(b, a + b, DARK_GREY);

    // The unit normal (drawn 3 times longer so it is easy to see),
    // and the one from the swapped inputs, which points the other way.
    float showLength = 3.0f;
    glm::vec3 n = orthogonalVector(a, b);
    glm::vec3 swapped = orthogonalVector(b, a);
    addArrow(origin, n * showLength, YELLOW);
    addArrow(origin, swapped * showLength, MAGENTA);
}

// ===========================================================================
// Exercise 5: the unit square and its two transformed copies (2D).
// Corners are marked with dots: A yellow, B red, C green, D blue.
// ===========================================================================

// Draws a square stored as a matrix (one corner (x, y, 1) per column).
void addSquare(glm::mat4x3 square, glm::vec3 color)
{
    glm::vec3 cornerColors[4] = { YELLOW, RED, GREEN, BLUE };   // A, B, C, D
    for (int corner = 0; corner < 4; corner++) {
        int next = (corner + 1) % 4;
        // (x, y, 1) is a 2D point: draw it at height z = 0.
        glm::vec3 from(square[corner].x, square[corner].y, 0.0f);
        glm::vec3 to(square[next].x, square[next].y, 0.0f);
        addLine(from, to, color);
        addPoint(from, cornerColors[corner]);
    }
}

void buildScene5()
{
    addGrid2D(5);

    // The center unit square (same as the console program).
    glm::mat4x3 square;
    square[0] = glm::vec3(-0.5f,  0.5f, 1.0f);   // A
    square[1] = glm::vec3( 0.5f,  0.5f, 1.0f);   // B
    square[2] = glm::vec3( 0.5f, -0.5f, 1.0f);   // C
    square[3] = glm::vec3(-0.5f, -0.5f, 1.0f);   // D

    glm::mat3 M1 = translate2D(2.0f, 1.0f) * rotate2D(-45.0f);
    glm::mat3 M2 = translate2D(-2.0f, -2.0f) * rotate2D(180.0f) * scale2D(2.0f, 2.0f);

    addSquare(square, WHITE);
    addSquare(M1 * square, CYAN);
    addSquare(M2 * square, ORANGE);
}

// ===========================================================================
// Scene list
// ===========================================================================

void buildScene(int scene)
{
    if (scene == 1) buildScene1();
    if (scene == 2) buildScene2();
    if (scene == 3) buildScene3();
    if (scene == 4) buildScene4();
    if (scene == 5) buildScene5();

    // 3D scenes also show the X (red), Y (green), Z (blue) axes.
    if (!sceneIs2D(scene)) {
        addAxes(1.5f);
    }
}

bool sceneIs2D(int scene)
{
    return scene == 5;
}

float sceneCameraDistance(int scene)
{
    if (scene == 1) return 3.5f;
    if (scene == 3) return 8.0f;
    if (scene == 4) return 24.0f;
    return 5.0f;
}

std::string sceneTitle(int scene)
{
    if (scene == 1) return "Exercise 1 - u x (v + w) = u x v + u x w";
    if (scene == 2) return "Exercise 2 - Rx(45) and Ry(90)";
    if (scene == 3) return "Exercise 3 - 3D rotations are not commutative";
    if (scene == 4) return "Exercise 4 - Normal to the plane of a and b";
    return "Exercise 5 - Transforming the unit square";
}

void printSceneLegend(int scene)
{
    std::cout << "\n" << sceneTitle(scene) << "\n";
    if (scene == 1) {
        std::cout << "  red u, green v, blue w, cyan v + w\n"
                  << "  yellow = u x (v + w)\n"
                  << "  orange = u x v, then magenta = u x w added on its tip\n"
                  << "  -> the yellow tip and the white dot meet: both sides are equal\n"
                  << "  Space = new random vectors\n";
    }
    if (scene == 2) {
        std::cout << "  white = u (1,1,1), v (-1,-1,0), w (1,0,0)\n"
                  << "  orange = after Rx (45 deg about the red X axis)\n"
                  << "  cyan   = after Ry (90 deg about the green Y axis)\n";
    }
    if (scene == 3) {
        std::cout << "  left = original die, middle = rotate about x then y (Ry * Rx),\n"
                  << "  right = rotate about y then x (Rx * Ry)\n"
                  << "  -> middle and right face different ways: order matters\n"
                  << "  yellow dot = a random point rotated with the die (Space = new point)\n";
    }
    if (scene == 4) {
        std::cout << "  red a (1,2,3), green b (4,5,6), grey = the plane they span\n"
                  << "  yellow  = orthogonalVector(a, b)  (shown 3x longer)\n"
                  << "  magenta = orthogonalVector(b, a)  -> same line, opposite direction\n";
    }
    if (scene == 5) {
        std::cout << "  white = center unit square, cyan = T(2,1) * R(-45),\n"
                  << "  orange = T(-2,-2) * R(180) * S(2,2)\n"
                  << "  corner dots: A yellow, B red, C green, D blue\n";
    }
}
