#include "exercises.hpp"
#include "helpers.hpp"

#include <cmath>                         // std::cos, std::sin
#include <iostream>                      // std::cout
#include <string>                        // std::string
#include <glm/gtc/matrix_transform.hpp>  // glm::rotate (used only to double-check our matrices)

// How many random tests to run in Exercises 1, 3 and 4.
const int RANDOM_TESTS = 5;

// ===========================================================================
// MATRIX BUILDERS
//
// IMPORTANT (Lab 2 slide 3): GLM is "column-major".
//   m[column][row]  is the number at that row and column.
// So I need write the matrix entry in row r, column c, we write  m[c][r].
// Every matrix starts as the identity (1s on the diagonal, 0s elsewhere)
// and then only the entries that are different are changed.
// ===========================================================================

// Rotation about the X axis:
//       | 1    0       0    |
//  Rx = | 0  cos(a) -sin(a) |
//       | 0  sin(a)  cos(a) |
glm::mat3 rotationX(float degrees)
{
    float angle = glm::radians(degrees);   // cos and sin need radians
    float c = std::cos(angle);
    float s = std::sin(angle);

    glm::mat3 m(1.0f);   // identity
    m[1][1] = c;         // row 1, column 1
    m[2][1] = -s;        // row 1, column 2
    m[1][2] = s;         // row 2, column 1
    m[2][2] = c;         // row 2, column 2
    return m;
}

// Rotation about the Y axis:
//       |  cos(a)  0  sin(a) |
//  Ry = |    0     1    0    |
//       | -sin(a)  0  cos(a) |
glm::mat3 rotationY(float degrees)
{
    float angle = glm::radians(degrees);
    float c = std::cos(angle);
    float s = std::sin(angle);

    glm::mat3 m(1.0f);   // identity
    m[0][0] = c;         // row 0, column 0
    m[2][0] = s;         // row 0, column 2
    m[0][2] = -s;        // row 2, column 0
    m[2][2] = c;         // row 2, column 2
    return m;
}

// 2D translation (homogeneous coordinates):
//      | 1  0  tx |
//  T = | 0  1  ty |
//      | 0  0  1  |
glm::mat3 translate2D(float tx, float ty)
{
    glm::mat3 m(1.0f);   // identity
    m[2][0] = tx;        // row 0, column 2
    m[2][1] = ty;        // row 1, column 2
    return m;
}

// 2D rotation about the origin (positive angle = counter-clockwise):
//      | cos(a) -sin(a)  0 |
//  R = | sin(a)  cos(a)  0 |
//      |   0       0     1 |
glm::mat3 rotate2D(float degrees)
{
    float angle = glm::radians(degrees);
    float c = std::cos(angle);
    float s = std::sin(angle);

    glm::mat3 m(1.0f);   // identity
    m[0][0] = c;         // row 0, column 0
    m[1][0] = -s;        // row 0, column 1
    m[0][1] = s;         // row 1, column 0
    m[1][1] = c;         // row 1, column 1
    return m;
}

// 2D scale:
//      | sx  0  0 |
//  S = | 0  sy  0 |
//      | 0   0  1 |
glm::mat3 scale2D(float sx, float sy)
{
    glm::mat3 m(1.0f);   // identity
    m[0][0] = sx;        // row 0, column 0
    m[1][1] = sy;        // row 1, column 1
    return m;
}

// ===========================================================================
// EXERCISE 1
// Show that the cross product is distributive over addition:
//     u x (v + w) = u x v + u x w
// Tested with random vectors, showing the intermediate results.
// ===========================================================================
void exercise1()
{
    printTitle("Exercise 1 - Cross product is distributive over addition");
    printText("Checking  u x (v + w) = u x v + u x w  with random vectors");

    for (int test = 1; test <= RANDOM_TESTS; test++) {
        std::cout << "\n  " << color(TEXT_YELLOW) << " Random test " << test << " " << color(TEXT_RESET) << "\n";

        // Three random vectors.
        glm::vec3 u = randomVec3();
        glm::vec3 v = randomVec3();
        glm::vec3 w = randomVec3();
        printVec3("u", u);
        printVec3("v", v);
        printVec3("w", w);

        // Left side: u x (v + w)
        glm::vec3 vPlusW = v + w;
        glm::vec3 leftSide = glm::cross(u, vPlusW);
        printVec3("v + w", vPlusW);
        printVec3("u x (v + w)", leftSide);

        // Right side: u x v + u x w
        glm::vec3 uCrossV = glm::cross(u, v);
        glm::vec3 uCrossW = glm::cross(u, w);
        glm::vec3 rightSide = uCrossV + uCrossW;
        printVec3("u x v", uCrossV);
        printVec3("u x w", uCrossW);
        printVec3("u x v + u x w", rightSide);

        check("u x (v + w) equals u x v + u x w", nearlyEqualVec3(leftSide, rightSide));
    }
}

// ===========================================================================
// EXERCISE 2
// Implement the rotations Rx = 45 deg and Ry = 90 deg as matrices and
// apply them to u = (1, 1, 1), v = (-1, -1, 0), w = (1, 0, 0).
// ===========================================================================
void exercise2()
{
    printTitle("Exercise 2 - Rotation matrices");

    // The vectors from the lab.
    glm::vec3 u(1.0f, 1.0f, 1.0f);
    glm::vec3 v(-1.0f, -1.0f, 0.0f);
    glm::vec3 w(1.0f, 0.0f, 0.0f);

    // Build the two rotation matrices.
    glm::mat3 Rx = rotationX(45.0f);
    glm::mat3 Ry = rotationY(90.0f);
    printMat3("Rx (45 deg)", Rx);
    printMat3("Ry (90 deg)", Ry);

    // Double-check our hand-built matrices against GLM's own glm::rotate.
    // glm::rotate makes a 4x4 matrix; glm::mat3(...) keeps its top-left 3x3 part.
    glm::mat3 glmRx = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    glm::mat3 glmRy = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    check("Rx matches glm::rotate", nearlyEqualMat3(Rx, glmRx));
    check("Ry matches glm::rotate", nearlyEqualMat3(Ry, glmRy));

    // Apply Rx (45 deg about X) to each vector.
    // X stays the same; Y and Z turn around the X axis.
    printText("");
    printText("Applying Rx (45 deg):");
    glm::vec3 RxU = Rx * u;
    glm::vec3 RxV = Rx * v;
    glm::vec3 RxW = Rx * w;
    printVec3("Rx * u", RxU);
    printVec3("Rx * v", RxV);
    printVec3("Rx * w", RxW);

    // Expected results worked out by hand (cos 45 = sin 45 = 0.7071):
    //   Rx*u = (1, 0.7071 - 0.7071, 0.7071 + 0.7071) = (1, 0, 1.4142)
    //   Rx*v = (-1, -0.7071, -0.7071)
    //   Rx*w = (1, 0, 0)   (w lies on the X axis, so it does not move)
    float h = std::sqrt(2.0f) / 2.0f;   // 0.7071
    check("Rx * u = (1, 0, 1.4142)", nearlyEqualVec3(RxU, glm::vec3(1.0f, 0.0f, 2.0f * h)));
    check("Rx * v = (-1, -0.7071, -0.7071)", nearlyEqualVec3(RxV, glm::vec3(-1.0f, -h, -h)));
    check("Rx * w = (1, 0, 0)", nearlyEqualVec3(RxW, glm::vec3(1.0f, 0.0f, 0.0f)));

    // Apply Ry (90 deg about Y) to each vector.
    // With cos 90 = 0 and sin 90 = 1, (x, y, z) becomes (z, y, -x).
    printText("");
    printText("Applying Ry (90 deg):");
    glm::vec3 RyU = Ry * u;
    glm::vec3 RyV = Ry * v;
    glm::vec3 RyW = Ry * w;
    printVec3("Ry * u", RyU);
    printVec3("Ry * v", RyV);
    printVec3("Ry * w", RyW);

    check("Ry * u = (1, 1, -1)", nearlyEqualVec3(RyU, glm::vec3(1.0f, 1.0f, -1.0f)));
    check("Ry * v = (0, -1, 1)", nearlyEqualVec3(RyV, glm::vec3(0.0f, -1.0f, 1.0f)));
    check("Ry * w = (0, 0, -1)", nearlyEqualVec3(RyW, glm::vec3(0.0f, 0.0f, -1.0f)));

    // A rotation never changes the length of a vector.
    check("Rx and Ry keep the length of u",
          nearlyEqualFloat(glm::length(RxU), glm::length(u)) &&
          nearlyEqualFloat(glm::length(RyU), glm::length(u)));
}

// ===========================================================================
// EXERCISE 3
// Show that 3D rotations are NOT commutative (the order matters),
// unlike 2D rotations. Tested with random vectors.
// ===========================================================================
void exercise3()
{
    printTitle("Exercise 3 - 3D rotations are not commutative");

    // Same rotations as the dice picture: 90 deg about X and 90 deg about Y.
    glm::mat3 Rx = rotationX(90.0f);
    glm::mat3 Ry = rotationY(90.0f);

    // Matrices are applied right to left:
    //   Ry * Rx * p  means "first rotate about X, then about Y".
    glm::mat3 xThenY = Ry * Rx;
    glm::mat3 yThenX = Rx * Ry;
    printMat3("Ry * Rx  (rotate about x, then y)", xThenY);
    printMat3("Rx * Ry  (rotate about y, then x)", yThenX);
    check("The two combined matrices are different", !nearlyEqualMat3(xThenY, yThenX));

    // 3D: random vectors give different results depending on the order.
    for (int test = 1; test <= RANDOM_TESTS; test++) {
        std::cout << "\n  " << color(TEXT_YELLOW) << " Random 3D test " << test << " " << color(TEXT_RESET) << "\n";

        glm::vec3 p = randomVec3();
        glm::vec3 resultXThenY = Ry * (Rx * p);
        glm::vec3 resultYThenX = Rx * (Ry * p);
        printVec3("p", p);
        printVec3("x then y: Ry * (Rx * p)", resultXThenY);
        printVec3("y then x: Rx * (Ry * p)", resultYThenX);

        check("3D: changing the order gives a different result",
              !nearlyEqualVec3(resultXThenY, resultYThenX));
    }

    // 2D: rotations about the same point always give the same result,
    // whatever the order (rotating by a then b = rotating by a + b).
    for (int test = 1; test <= RANDOM_TESTS; test++) {
        std::cout << "\n  " << color(TEXT_YELLOW) << " Random 2D test " << test << " " << color(TEXT_RESET) << "\n";

        float angleA = randomFloat(0.0f, 360.0f);
        float angleB = randomFloat(0.0f, 360.0f);
        glm::vec3 p = glm::vec3(randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f), 1.0f); // 2D point (x, y, 1)

        glm::mat3 Ra = rotate2D(angleA);
        glm::mat3 Rb = rotate2D(angleB);
        glm::vec3 resultAThenB = Rb * (Ra * p);
        glm::vec3 resultBThenA = Ra * (Rb * p);
        printFloat("angle a", angleA);
        printFloat("angle b", angleB);
        printVec3("p (x, y, 1)", p);
        printVec3("a then b: Rb * (Ra * p)", resultAThenB);
        printVec3("b then a: Ra * (Rb * p)", resultBThenA);

        check("2D: changing the order gives the same result",
              nearlyEqualVec3(resultAThenB, resultBThenA));
    }
}

// ===========================================================================
// EXERCISE 4
// Plane with basis vectors a = (1, 2, 3) and b = (4, 5, 6).
//
// Analytic answer (by hand):
//   n = a x b = (2*6 - 3*5,  3*4 - 1*6,  1*5 - 2*4) = (-3, 6, -3)
//   Verify:  a . n = -3 + 12 - 9  = 0
//            b . n = -12 + 30 - 18 = 0   -> n is orthogonal to the plane.
//   Unit vector: |n| = sqrt(9 + 36 + 9) = sqrt(54)
//                n / |n| = (-0.4082, 0.8165, -0.4082)
// ===========================================================================

// Returns a unit vector orthogonal to both a and b.
// The cross product a x b is orthogonal to both; normalize makes its length 1.
// (The lab says a and b are nonzero and not collinear, so a x b is never zero.)
glm::vec3 orthogonalVector(glm::vec3 a, glm::vec3 b)
{
    glm::vec3 n = glm::cross(a, b);
    glm::vec3 unitN = glm::normalize(n);
    return unitN;
}

// Returns true if c is orthogonal to both a and b.
// Two vectors are orthogonal when their dot product is 0.
bool areOrthogonal(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
    float aDotC = glm::dot(a, c);
    float bDotC = glm::dot(b, c);
    bool orthogonalToA = nearlyEqualFloat(aDotC, 0.0f);
    bool orthogonalToB = nearlyEqualFloat(bDotC, 0.0f);
    return orthogonalToA && orthogonalToB;
}

void exercise4()
{
    printTitle("Exercise 4 - Vector orthogonal to a plane");

    // --- The plane from the lab ---
    glm::vec3 a(1.0f, 2.0f, 3.0f);
    glm::vec3 b(4.0f, 5.0f, 6.0f);
    printVec3("a", a);
    printVec3("b", b);

    glm::vec3 n = glm::cross(a, b);
    printVec3("a x b", n);
    printFloat("a . (a x b)", glm::dot(a, n));
    printFloat("b . (a x b)", glm::dot(b, n));
    check("a x b = (-3, 6, -3), as computed by hand", nearlyEqualVec3(n, glm::vec3(-3.0f, 6.0f, -3.0f)));

    glm::vec3 unitN = orthogonalVector(a, b);
    printVec3("orthogonalVector(a, b)", unitN);
    printFloat("length", glm::length(unitN));
    check("orthogonalVector(a, b) has length 1", nearlyEqualFloat(glm::length(unitN), 1.0f));
    check("areOrthogonal(a, b, orthogonalVector(a, b)) is true", areOrthogonal(a, b, unitN));
    check("areOrthogonal(a, b, a) is false (a is not orthogonal to itself)", !areOrthogonal(a, b, a));

    // --- Swapping the inputs ---
    // The cross product is anti-commutative: b x a = -(a x b).
    // So swapping the inputs gives the SAME line but the OPPOSITE direction.
    glm::vec3 swapped = orthogonalVector(b, a);
    printText("");
    printVec3("orthogonalVector(b, a)", swapped);
    check("Swapping the inputs flips the vector: orthogonalVector(b, a) = -orthogonalVector(a, b)",
          nearlyEqualVec3(swapped, -unitN));
    check("The flipped vector is still orthogonal to the plane", areOrthogonal(a, b, swapped));

    // --- The 2D cartesian basis (written in 3D with z = 0) ---
    glm::vec3 e1(1.0f, 0.0f, 0.0f);
    glm::vec3 e2(0.0f, 1.0f, 0.0f);
    glm::vec3 e1e2 = orthogonalVector(e1, e2);
    glm::vec3 e2e1 = orthogonalVector(e2, e1);
    printText("");
    printVec3("e1", e1);
    printVec3("e2", e2);
    printVec3("orthogonalVector(e1, e2)", e1e2);
    printVec3("orthogonalVector(e2, e1)", e2e1);
    check("orthogonalVector(e1, e2) = (0, 0, 1)", nearlyEqualVec3(e1e2, glm::vec3(0.0f, 0.0f, 1.0f)));
    check("orthogonalVector(e2, e1) = (0, 0, -1)", nearlyEqualVec3(e2e1, glm::vec3(0.0f, 0.0f, -1.0f)));
    check("areOrthogonal(e1, e2, (0, 0, 1)) is true", areOrthogonal(e1, e2, glm::vec3(0.0f, 0.0f, 1.0f)));

    // --- Random vectors ---
    for (int test = 1; test <= RANDOM_TESTS; test++) {
        std::cout << "\n  " << color(TEXT_YELLOW) << " Random test " << test << " " << color(TEXT_RESET) << "\n";

        glm::vec3 p = randomVec3();
        glm::vec3 q = randomVec3();
        glm::vec3 r = orthogonalVector(p, q);
        printVec3("p", p);
        printVec3("q", q);
        printVec3("orthogonalVector(p, q)", r);

        check("result has length 1 and is orthogonal to p and q",
              nearlyEqualFloat(glm::length(r), 1.0f) && areOrthogonal(p, q, r));
    }
}

// ===========================================================================
// EXERCISE 5
// The center unit square ABCD and two transformed copies (see the diagram).
//
// Encoding the square as a matrix:
//   Each corner is a 2D point written as (x, y, 1) (homogeneous coordinates).
//   The 4 corners are the 4 COLUMNS of a 3x4 matrix (3 rows, 4 columns),
//   which GLM calls glm::mat4x3 (4 columns, 3 rows).
//        A     B     C     D
//     | -0.5  0.5   0.5  -0.5 |  x
//     |  0.5  0.5  -0.5  -0.5 |  y
//     |  1    1     1     1   |  1
//   One matrix multiplication then transforms all 4 corners at once.
//
// Transformations (applied right to left):
//   A'B'C'D'   = T(2, 1) * R(-45)
//                Rotate 45 deg CLOCKWISE (A ends at the top, as in the diagram),
//                then move the center to (2, 1). Size stays 1.
//   A''B''C''D'' = T(-2, -2) * R(180) * S(2, 2)
//                Scale to 2x2, rotate 180 deg (A ends at the bottom-right,
//                as in the diagram), then move the center to (-2, -2).
//                (Scale then rotate 180 is the same as scaling by -2.)
// ===========================================================================

// Prints the 4 corners stored in the columns of a square matrix.
void printSquare(std::string name, glm::mat4x3 square)
{
    printText(name + ":");
    printVec3("  A", square[0]);
    printVec3("  B", square[1]);
    printVec3("  C", square[2]);
    printVec3("  D", square[3]);
}

// Checks that every corner of a square matches the expected corner.
bool nearlyEqualSquare(glm::mat4x3 a, glm::mat4x3 b)
{
    for (int corner = 0; corner < 4; corner++) {
        if (!nearlyEqualVec3(a[corner], b[corner])) {
            return false;
        }
    }
    return true;
}

void exercise5()
{
    printTitle("Exercise 5 - Transforming the unit square");

    // --- The center unit square as a matrix (one corner per column) ---
    glm::mat4x3 square;
    square[0] = glm::vec3(-0.5f,  0.5f, 1.0f);   // A (top-left)
    square[1] = glm::vec3( 0.5f,  0.5f, 1.0f);   // B (top-right)
    square[2] = glm::vec3( 0.5f, -0.5f, 1.0f);   // C (bottom-right)
    square[3] = glm::vec3(-0.5f, -0.5f, 1.0f);   // D (bottom-left)
    printSquare("Center square ABCD", square);

    // --- First copy: A'B'C'D', rotated 45 deg, centered at (2, 1) ---
    glm::mat3 R1 = rotate2D(-45.0f);          // clockwise
    glm::mat3 T1 = translate2D(2.0f, 1.0f);
    glm::mat3 M1 = T1 * R1;                    // rotate first, then translate
    printText("");
    printMat3("M1 = T(2, 1) * R(-45)", M1);

    glm::mat4x3 square1 = M1 * square;
    printSquare("A'B'C'D' = M1 * ABCD", square1);

    // Expected corners from the diagram (0.7071 = half the diagonal of a unit square).
    float h = std::sqrt(2.0f) / 2.0f;
    glm::mat4x3 expected1;
    expected1[0] = glm::vec3(2.0f,     1.0f + h, 1.0f);   // A' top
    expected1[1] = glm::vec3(2.0f + h, 1.0f,     1.0f);   // B' right
    expected1[2] = glm::vec3(2.0f,     1.0f - h, 1.0f);   // C' bottom
    expected1[3] = glm::vec3(2.0f - h, 1.0f,     1.0f);   // D' left
    check("A'B'C'D' matches the diagram", nearlyEqualSquare(square1, expected1));

    // --- Second copy: A''B''C''D'', size 2, rotated 180 deg, centered at (-2, -2) ---
    glm::mat3 S2 = scale2D(2.0f, 2.0f);
    glm::mat3 R2 = rotate2D(180.0f);
    glm::mat3 T2 = translate2D(-2.0f, -2.0f);
    glm::mat3 M2 = T2 * R2 * S2;               // scale first, then rotate, then translate
    printText("");
    printMat3("M2 = T(-2, -2) * R(180) * S(2, 2)", M2);

    glm::mat4x3 square2 = M2 * square;
    printSquare("A''B''C''D'' = M2 * ABCD", square2);

    glm::mat4x3 expected2;
    expected2[0] = glm::vec3(-1.0f, -3.0f, 1.0f);   // A'' bottom-right
    expected2[1] = glm::vec3(-3.0f, -3.0f, 1.0f);   // B'' bottom-left
    expected2[2] = glm::vec3(-3.0f, -1.0f, 1.0f);   // C'' top-left
    expected2[3] = glm::vec3(-1.0f, -1.0f, 1.0f);   // D'' top-right
    check("A''B''C''D'' matches the diagram", nearlyEqualSquare(square2, expected2));

    // Order matters here too: translating BEFORE rotating moves the square somewhere else.
    glm::mat3 wrongOrder = R1 * T1;
    glm::mat4x3 squareWrong = wrongOrder * square;
    printText("");
    printSquare("Wrong order: R(-45) * T(2, 1) * ABCD", squareWrong);
    check("Translating before rotating gives a different square", !nearlyEqualSquare(squareWrong, expected1));
}
