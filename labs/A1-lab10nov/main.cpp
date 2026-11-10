// Assignment A1 - Math Challenge, exercises 1 to 3

#include <cmath>
#include <iostream>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>   // glm::to_string, to print vectors

// Threshold for comparing floats (from the assignment).
const float THRESHOLD = (float)1.0e-5;

// Random vector with values between min and max.
glm::vec3 randomVec(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return glm::vec3(dist(gen), dist(gen), dist(gen));
}

// Random 3x3 matrix with values between min and max
glm::mat3 randomMat3(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);
    glm::mat3 randomMat;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            randomMat[i][j] = dist(gen);
        }
    }
    return randomMat;
}

// ---------------------------------------------------------------------------
// Exercise 1: i x (j x k) = j(i . k) - k(i . j)
// ---------------------------------------------------------------------------
// Returns true if both sides are equal.
//
// The calculation is done in double (dvec3) instead of float: with values up
// to 5 the results reach the hundreds, and float is not precise enough there
// for THRESHOLD = 1.0e-5 (about 1 in 4 correct results would fail).
bool tripleProduct(glm::vec3 i, glm::vec3 j, glm::vec3 k)
{
    glm::dvec3 di = i;
    glm::dvec3 dj = j;
    glm::dvec3 dk = k;

    glm::dvec3 left = glm::cross(di, glm::cross(dj, dk));
    glm::dvec3 right = dj * glm::dot(di, dk) - dk * glm::dot(di, dj);

    bool equal = glm::all(glm::epsilonEqual(left, right, (double)THRESHOLD));

    std::cout << "i = " << glm::to_string(i) << std::endl;
    std::cout << "j = " << glm::to_string(j) << std::endl;
    std::cout << "k = " << glm::to_string(k) << std::endl;
    std::cout << "i x (j x k)         = " << glm::to_string(glm::vec3(left)) << std::endl;
    std::cout << "j(i . k) - k(i . j) = " << glm::to_string(glm::vec3(right)) << std::endl;
    if (equal) {
        std::cout << "Equal: yes" << std::endl;
    } else {
        std::cout << "Equal: no" << std::endl;
    }
    std::cout << std::endl;

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

void testRodrigues(glm::vec3 v, glm::vec3 axis, float degrees, glm::vec3 expected)
{
    glm::vec3 result = rodrigues(v, axis, glm::radians(degrees));

    std::cout << "v        = " << glm::to_string(v) << std::endl;
    std::cout << "axis     = " << glm::to_string(axis) << std::endl;
    std::cout << "angle    = " << degrees << " degrees" << std::endl;
    std::cout << "expected = " << glm::to_string(expected) << std::endl;
    std::cout << "result   = " << glm::to_string(result) << std::endl;
    if (glm::all(glm::epsilonEqual(result, expected, THRESHOLD))) {
        std::cout << "Equal: yes" << std::endl;
    } else {
        std::cout << "Equal: no" << std::endl;
    }
    std::cout << std::endl;
}

int main()
{
    // Exercise 1: random vectors with values between 1 and 5.
    std::cout << "Exercise 1: i x (j x k) = j(i . k) - k(i . j)" << std::endl << std::endl;
    for (int n = 0; n < 5; n++) {
        tripleProduct(randomVec(1.0f, 5.0f), randomVec(1.0f, 5.0f), randomVec(1.0f, 5.0f));
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

    return 0;
}
