// Assignment A1 - Math Challenge, exercises 1 to 6

#include <cmath>
#include <iostream>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>   // glm::to_string, to print vectors

// Threshold for comparing floats.
const float THRESHOLD = (float)1.0e-5;

// Professor Headers
glm::vec3 randomVec(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return glm::vec3((float)dist(gen), (float)dist(gen), (float)dist(gen));
}

// Random 3x3 matrix with whole-number values between min and max.
// Whole numbers keep the float results precise enough for THRESHOLD.
glm::mat3 randomMat3(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);
    glm::mat3 randomMat;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            randomMat[i][j] = (float)dist(gen);
        }
    }
    return randomMat;
}

// Returns true if every entry of A and B is equal (within THRESHOLD).
bool matricesEqual(glm::mat3 A, glm::mat3 B)
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (!glm::epsilonEqual(A[i][j], B[i][j], THRESHOLD)) {
                return false;
            }
        }
    }
    return true;
}

// Prints "Equal: yes" or "Equal: no", then an empty line.
void printEqual(bool equal)
{
    if (equal) {
        std::cout << "Equal: yes" << std::endl;
    } else {
        std::cout << "Equal: no" << std::endl;
    }
    std::cout << std::endl;
}

// ---------------------------------------------------------------------------
// Exercise 1: i x (j x k) = j(i . k) - k(i . j)
// ---------------------------------------------------------------------------
// Returns true if both sides are equal.
bool tripleProduct(glm::vec3 i, glm::vec3 j, glm::vec3 k)
{
    glm::vec3 left = glm::cross(i, glm::cross(j, k));
    glm::vec3 right = j * glm::dot(i, k) - k * glm::dot(i, j);

    bool equal = glm::all(glm::epsilonEqual(left, right, THRESHOLD));

    std::cout << "i = " << glm::to_string(i) << std::endl;
    std::cout << "j = " << glm::to_string(j) << std::endl;
    std::cout << "k = " << glm::to_string(k) << std::endl;
    std::cout << "i x (j x k)         = " << glm::to_string(left) << std::endl;
    std::cout << "j(i . k) - k(i . j) = " << glm::to_string(right) << std::endl;
    printEqual(equal);

    return equal;
}

// ---------------------------------------------------------------------------
// Exercise 2: camera frame from view and up vectors
//   v = view / |view|
//   w = (up x v) / |up x v|
//   u = v x w
// ---------------------------------------------------------------------------
void cameraFrame(glm::vec3 view, glm::vec3 up)
{
    std::cout << "view = " << glm::to_string(view) << std::endl;
    std::cout << "up   = " << glm::to_string(up) << std::endl;

    glm::vec3 v = view / glm::length(view);
    glm::vec3 upCrossV = glm::cross(up, v);

    // If view and up are parallel, up x v is zero and we cannot divide by its length.
    if (glm::length(upCrossV) < THRESHOLD) {
        std::cout << "Error: view and up are parallel, the frame cannot be created." << std::endl;
        std::cout << std::endl;
        return;
    }

    glm::vec3 w = upCrossV / glm::length(upCrossV);
    glm::vec3 u = glm::cross(v, w);

    std::cout << "v = " << glm::to_string(v) << std::endl;
    std::cout << "w = " << glm::to_string(w) << std::endl;
    std::cout << "u = " << glm::to_string(u) << std::endl;
    std::cout << std::endl;
}

// ---------------------------------------------------------------------------
// Exercise 3: Rodrigues rotation formula
//   vrot = v cos(t) + (a x v) sin(t) + a (a . v)(1 - cos(t))
// ---------------------------------------------------------------------------
// The angle is in radians, like the rest of GLM (use glm::radians(degrees)).
glm::vec3 rodrigues(glm::vec3 v, glm::vec3 axis, float angle)
{
    glm::vec3 a = glm::normalize(axis);

    return v * std::cos(angle) + glm::cross(a, v) * std::sin(angle) + a * glm::dot(a, v) * (1.0f - std::cos(angle));
}

// Prints one rotation test (used by exercises 3 and 4).
void printRotationTest(glm::vec3 v, glm::vec3 axis, float degrees, glm::vec3 expected, glm::vec3 result)
{
    std::cout << "v        = " << glm::to_string(v) << std::endl;
    std::cout << "axis     = " << glm::to_string(axis) << std::endl;
    std::cout << "angle    = " << degrees << " degrees" << std::endl;
    std::cout << "expected = " << glm::to_string(expected) << std::endl;
    std::cout << "result   = " << glm::to_string(result) << std::endl;
    printEqual(glm::all(glm::epsilonEqual(result, expected, THRESHOLD)));
}

void testRodrigues(glm::vec3 v, glm::vec3 axis, float degrees, glm::vec3 expected)
{
    glm::vec3 result = rodrigues(v, axis, glm::radians(degrees));
    printRotationTest(v, axis, degrees, expected, result);
}

// ---------------------------------------------------------------------------
// Exercise 4: Rodrigues rotation formula, matrix form
//   R = I + sin(t) A + (1 - cos(t)) A^2
// where A is the "dual matrix" of the unit axis a = (ax, ay, az):
//       |  0  -az   ay |
//   A = |  az   0  -ax |       (A v = a x v)
//       | -ay  ax    0 |
// ---------------------------------------------------------------------------
// The angle is in radians, like the rest of GLM.
glm::mat3 rodriguesMatrix(glm::vec3 axis, float angle)
{
    glm::vec3 a = glm::normalize(axis);

    // GLM matrices are column-major: A[column][row].
    glm::mat3 A(0.0f);
    A[1][0] = -a.z;   // row 0, column 1
    A[2][0] = a.y;    // row 0, column 2
    A[0][1] = a.z;    // row 1, column 0
    A[2][1] = -a.x;   // row 1, column 2
    A[0][2] = -a.y;   // row 2, column 0
    A[1][2] = a.x;    // row 2, column 1

    glm::mat3 I(1.0f);
    return I + std::sin(angle) * A + (1.0f - std::cos(angle)) * (A * A);
}

void testRodriguesMatrix(glm::vec3 v, glm::vec3 axis, float degrees, glm::vec3 expected)
{
    glm::mat3 R = rodriguesMatrix(axis, glm::radians(degrees));
    glm::vec3 result = R * v;
    printRotationTest(v, axis, degrees, expected, result);
}

// ---------------------------------------------------------------------------
// Exercise 5: (AB)^T = B^T A^T
// ---------------------------------------------------------------------------
// Returns true if both sides are equal.
bool transposeProperty(glm::mat3 A, glm::mat3 B)
{
    glm::mat3 left = glm::transpose(A * B);
    glm::mat3 right = glm::transpose(B) * glm::transpose(A);

    bool equal = matricesEqual(left, right);

    std::cout << "A         = " << glm::to_string(A) << std::endl;
    std::cout << "B         = " << glm::to_string(B) << std::endl;
    std::cout << "(AB)^T    = " << glm::to_string(left) << std::endl;
    std::cout << "B^T A^T   = " << glm::to_string(right) << std::endl;
    printEqual(equal);

    return equal;
}

// ---------------------------------------------------------------------------
// Exercise 6: (AB)^-1 = B^-1 A^-1
// A and B must be invertible: det(A) != 0 and det(B) != 0.
// ---------------------------------------------------------------------------
// Returns true if both sides are equal.
bool inverseProperty(glm::mat3 A, glm::mat3 B)
{
    std::cout << "A         = " << glm::to_string(A) << std::endl;
    std::cout << "B         = " << glm::to_string(B) << std::endl;
    float detA = glm::determinant(A);
    float detB = glm::determinant(B);
    std::cout << "det(A)    = " << detA << std::endl;
    std::cout << "det(B)    = " << detB << std::endl;

    // A matrix with determinant 0 has no inverse.
    if (detA == 0.0f || detB == 0.0f) {
        std::cout << "Error: A or B is not invertible (determinant is 0)." << std::endl;
        std::cout << std::endl;
        return false;
    }

    glm::mat3 left = glm::inverse(A * B);
    glm::mat3 right = glm::inverse(B) * glm::inverse(A);

    bool equal = matricesEqual(left, right);

    std::cout << "(AB)^-1   = " << glm::to_string(left) << std::endl;
    std::cout << "B^-1 A^-1 = " << glm::to_string(right) << std::endl;
    printEqual(equal);

    return equal;
}

int main()
{
    // Exercise 1: random vectors with values between 1 and 5. 
    std::cout << "Exercise 1: i x (j x k) = j(i . k) - k(i . j)" << std::endl << std::endl;
    for (int n = 0; n < 5; n++) {
        tripleProduct(randomVec(1, 5), randomVec(1, 5), randomVec(1, 5));
    }

    // Exercise 2: the three test cases from the assignment.
    std::cout << "Exercise 2: camera frame" << std::endl << std::endl;
    cameraFrame(glm::vec3(2.0f, 3.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    cameraFrame(glm::vec3(3.0f, 2.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    cameraFrame(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // Exercise 3: three tests with the expected output.
    std::cout << "Exercise 3: Rodrigues rotation" << std::endl << std::endl;
    // 120 degrees around (2, 2, 2) moves x -> y -> z, so (1, 2, 3) becomes (3, 1, 2).
    testRodrigues(glm::vec3(1.0f, 2.0f, 3.0f), glm::vec3(2.0f, 2.0f, 2.0f), 120.0f, glm::vec3(3.0f, 1.0f, 2.0f));
    // 180 degrees around (1, 1, 1): vrot = -v + 2a(a . v) = (3, 2, 1).
    testRodrigues(glm::vec3(1.0f, 2.0f, 3.0f), glm::vec3(1.0f, 1.0f, 1.0f), 180.0f, glm::vec3(3.0f, 2.0f, 1.0f));
    // (4, 4, 4) lies on the axis (3, 3, 3), so it does not move.
    testRodrigues(glm::vec3(4.0f, 4.0f, 4.0f), glm::vec3(3.0f, 3.0f, 3.0f), 90.0f, glm::vec3(4.0f, 4.0f, 4.0f));

    // Exercise 4: rotate each axis by 90 degrees around x, y and z.
    //   around x: (x, y, z) -> (x, -z, y)
    //   around y: (x, y, z) -> (z, y, -x)
    //   around z: (x, y, z) -> (-y, x, z)
    std::cout << "Exercise 4: Rodrigues rotation, matrix form" << std::endl << std::endl;
    glm::vec3 xAxis(1.0f, 0.0f, 0.0f);
    glm::vec3 yAxis(0.0f, 1.0f, 0.0f);
    glm::vec3 zAxis(0.0f, 0.0f, 1.0f);
 
    // x axis rotated around x, y and z
    testRodriguesMatrix(xAxis, xAxis, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    testRodriguesMatrix(xAxis, yAxis, 90.0f, glm::vec3(0.0f, 0.0f, -1.0f));
    testRodriguesMatrix(xAxis, zAxis, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    // y axis rotated around x, y and z
    testRodriguesMatrix(yAxis, xAxis, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    testRodriguesMatrix(yAxis, yAxis, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    testRodriguesMatrix(yAxis, zAxis, 90.0f, glm::vec3(-1.0f, 0.0f, 0.0f));
    // z axis rotated around x, y and z
    testRodriguesMatrix(zAxis, xAxis, 90.0f, glm::vec3(0.0f, -1.0f, 0.0f));
    testRodriguesMatrix(zAxis, yAxis, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    testRodriguesMatrix(zAxis, zAxis, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));

    // Exercise 5: random matrices with values between -5 and 5.
    std::cout << "Exercise 5: (AB)^T = B^T A^T" << std::endl << std::endl;
    for (int n = 0; n < 5; n++) {
        transposeProperty(randomMat3(-5, 5), randomMat3(-5, 5));
    }

    // Exercise 6: random invertible matrices with values between -5 and 5.
    std::cout << "Exercise 6: (AB)^-1 = B^-1 A^-1" << std::endl << std::endl;
    for (int n = 0; n < 5; n++) {
        // Pick new random matrices until both have a determinant different from 0.
        glm::mat3 A = randomMat3(-5, 5);
        while (glm::determinant(A) == 0.0f) {
            A = randomMat3(-5, 5);
        }
        glm::mat3 B = randomMat3(-5, 5);
        while (glm::determinant(B) == 0.0f) {
            B = randomMat3(-5, 5);
        }
        inverseProperty(A, B);
    }

    return 0;
}
