#pragma once

#include <glm/glm.hpp>

// ---- The five exercises ----
// Each one prints its intermediate results and runs its PASS/FAIL checks.
void exercise1(); // u x (v + w) = u x v + u x w
void exercise2(); // Rx = 45 deg and Ry = 90 deg applied to u, v, w
void exercise3(); // 3D rotations are not commutative (2D rotations are)
void exercise4(); // orthogonalVector and areOrthogonal
void exercise5(); // unit square as a matrix, then transformed

// ---- 3D rotation matrices (Exercises 2 and 3) ----
glm::mat3 rotationX(float degrees);
glm::mat3 rotationY(float degrees);

// ---- 2D transformation matrices in homogeneous coordinates (Exercises 3 and 5) ----
// A 2D point (x, y) is written as (x, y, 1) so that translation can be a matrix too.
glm::mat3 translate2D(float tx, float ty);
glm::mat3 rotate2D(float degrees);
glm::mat3 scale2D(float sx, float sy);

// ---- Exercise 4 functions (names from the lab) ----
glm::vec3 orthogonalVector(glm::vec3 a, glm::vec3 b);
bool areOrthogonal(glm::vec3 a, glm::vec3 b, glm::vec3 c);
