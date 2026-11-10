// Assignment A1 - Math Challenge, exercises 1 to 3

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

// Threshold for comparing floats (from the assignment).
const float THRESHOLD = (float)1.0e-5;

// Prints a vector as (x, y, z).
void printVector(const char* name, glm::vec3 v)
{
    std::cout << name << " = (" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
}

// Returns true if the two vectors are equal (within THRESHOLD).
bool isEqual(glm::vec3 a, glm::vec3 b)
{
    return glm::all(glm::epsilonEqual(a, b, THRESHOLD));
}

// Returns a random whole number between 1 and 5.
float randomValue()
{
    return (float)(rand() % 5 + 1);
}

// ---------------------------------------------------------------------------
// Exercise 1: i x (j x k) = j(i . k) - k(i . j)
// ---------------------------------------------------------------------------
void tripleProduct(glm::vec3 i, glm::vec3 j, glm::vec3 k)
{
    glm::vec3 left = glm::cross(i, glm::cross(j, k));
    glm::vec3 right = j * glm::dot(i, k) - k * glm::dot(i, j);

    printVector("i", i);
    printVector("j", j);
    printVector("k", k);
    printVector("i x (j x k)", left);
    printVector("j(i . k) - k(i . j)", right);

    if (isEqual(left, right)) {
        std::cout << "Equal: yes" << std::endl;
    } else {
        std::cout << "Equal: no" << std::endl;
    }
    std::cout << std::endl;
}

// ---------------------------------------------------------------------------
// Exercise 2: camera frame from view and up vectors
//   v = view / |view|
//   w = (up x v) / |up x v|
//   u = v x w
// ---------------------------------------------------------------------------
void cameraFrame(glm::vec3 view, glm::vec3 up)
{
    printVector("view", view);
    printVector("up", up);

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

    printVector("v", v);
    printVector("w", w);
    printVector("u", u);
    std::cout << std::endl;
}

// ---------------------------------------------------------------------------
// Exercise 3: Rodrigues rotation formula
//   vrot = v cos(t) + (a x v) sin(t) + a (a . v)(1 - cos(t))
// ---------------------------------------------------------------------------
glm::vec3 rodrigues(glm::vec3 v, glm::vec3 axis, float degrees)
{
    glm::vec3 a = glm::normalize(axis);
    float t = glm::radians(degrees);

    return v * std::cos(t) + glm::cross(a, v) * std::sin(t) + a * glm::dot(a, v) * (1.0f - std::cos(t));
}

void testRodrigues(glm::vec3 v, glm::vec3 axis, float degrees, glm::vec3 expected)
{
    glm::vec3 result = rodrigues(v, axis, degrees);

    printVector("v", v);
    printVector("axis", axis);
    std::cout << "angle = " << degrees << std::endl;
    printVector("expected", expected);
    printVector("result", result);

    if (isEqual(result, expected)) {
        std::cout << "Equal: yes" << std::endl;
    } else {
        std::cout << "Equal: no" << std::endl;
    }
    std::cout << std::endl;
}

int main()
{
    srand((unsigned int)time(nullptr));

    // Exercise 1: random vectors with values between 1 and 5.
    std::cout << "Exercise 1: i x (j x k) = j(i . k) - k(i . j)" << std::endl << std::endl;
    for (int n = 0; n < 5; n++) {
        glm::vec3 i(randomValue(), randomValue(), randomValue());
        glm::vec3 j(randomValue(), randomValue(), randomValue());
        glm::vec3 k(randomValue(), randomValue(), randomValue());
        tripleProduct(i, j, k);
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
