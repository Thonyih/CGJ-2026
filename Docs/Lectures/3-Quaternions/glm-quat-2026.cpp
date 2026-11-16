///////////////////////////////////////////////////////////////////////////////
//
// Quaternion Operations and Testing Suite
//
// This file provides a comprehensive demonstration and testing of quaternion
// operations using the GLM mathematics library. It includes tests for:
// - Basic quaternion rotations
// - Conversion between quaternions and rotation matrices
// - Quaternion composition and equivalent rotations
// - Angle-axis to quaternion conversions
// - Linear interpolation (LERP) between quaternions
// - Spherical linear interpolation (SLERP) between quaternions
//
// Each test is self-contained and includes assertions to verify correct
// behavior. The code is intended as both a learning resource and a validation
// suite for common quaternion operations in 3D graphics and animation.
//
// Dependencies:
// - GLM (OpenGL Mathematics Library)
// - C++ Standard Library
//
// Copyright (c) 2013-26 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>             // Non-experimental extensions
#include <glm/glm.hpp>             // Core GLM functionality
#include <glm/gtx/string_cast.hpp> // glm::to_string
//
#include <cassert>
#include <iomanip>  // for std::setw, std::setfill, etc.
#include <iostream> // for std::cout

////////////////////////////////////////////////////////////////////////////////

constexpr float THRESHOLD = static_cast<float>(1.0e-5);

// Example of a macro; could be changed to an inline function
#define BLANKS(n) std::setfill(' ') << std::setw(n) << ' '

// Prints text with newlines before and after to create a section header
inline void print_header(const std::string &text) {
  std::cout << '\n' << text << '\n' << '\n';
}

// Verifies that a quaternion has unit length
inline void check_unit_quaternion(const glm::quat &q) {
  assert(std::abs(glm::length(q) - 1.0f) < THRESHOLD);
}

// Verifies that two quaterions are equal
inline bool equal_quat(const glm::quat &q1, const glm::quat &q2,
                       float epsilon = THRESHOLD) {
  return std::abs(glm::dot(q1, q2)) > 1.0f - epsilon;
}

inline glm::quat clean_quat(const glm::quat &q) {
  return glm::quat(std::abs(q.w) < THRESHOLD ? 0.0f : q.w,
                   std::abs(q.x) < THRESHOLD ? 0.0f : q.x,
                   std::abs(q.y) < THRESHOLD ? 0.0f : q.y,
                   std::abs(q.z) < THRESHOLD ? 0.0f : q.z);
}

/*
 * Prints a 4x4 GLM matrix with pretty formatting
 * @param m The GLM matrix to print
 * @param indent Number of spaces to indent the matrix output (default: 0)
 * @param precision Number of decimal places to show (default: 3)
 * @param column_width Width of each column in characters (default: 7)
 *
 * This function takes a glm::mat4 and prints it in a bordered box format, with:
 * - Box drawing characters to create a border
 * - Configurable indentation before each line
 * - Configurable decimal precision
 * - Configurable column widths
 * The matrix is transposed before printing to show in row-major order
 */
void print_mat4(const glm::mat4 &m, int indent = 0, int precision = 3,
                int column_width = 7) {
  glm::mat4 t = glm::transpose(m);
  std::cout << BLANKS(indent) << "┌" << BLANKS(column_width * 4 + 1) << "┐\n";
  for (int i = 0; i < 4; ++i) {
    std::cout << BLANKS(indent) << "│";
    std::cout << std::fixed << std::setprecision(precision) << std::right;
    for (int j = 0; j < 4; ++j) {
      std::cout << std::setw(column_width) << t[i][j];
    }
    std::cout << " │\n";
  }
  std::cout << BLANKS(indent) << "└" << BLANKS(column_width * 4 + 1) << "┘\n";
}

////////////////////////////////////////////////////////////////////////////////

/*
 * Test rotation of 90 degrees about the y-axis using quaternions.
 * Demonstrates:
 * - Creating a rotation quaternion using angleAxis
 * - Unit quaternion length verification
 * - Quaternion multiplication for rotation
 * - Inverse and conjugate operations (both valid for unit quaternions)
 * - Verifying results with glm::equal assertions
 */
void qtest1() {
  print_header("TEST 1 : Rotation of 90 degrees about the y-axis");

  const glm::vec3 axis = {0.0f, 1.0f, 0.0f};
  glm::quat q = glm::angleAxis(glm::radians(90.0f), axis);
  check_unit_quaternion(q);
  std::cout << "    q=" << glm::to_string(q) << '\n';

  glm::quat qi = {0.0f, 7.0f, 0.0f, 0.0f};
  std::cout << "   qi=" << glm::to_string(qi) << '\n';

  glm::quat qe = {0.0f, 0.0f, 0.0f, -7.0f};
  std::cout << "   qe=" << glm::to_string(qe) << '\n';

  glm::quat qinv = glm::inverse(q);
  std::cout << " qinv=" << glm::to_string(qinv) << '\n';

  glm::quat qf1 = q * qi * qinv;
  std::cout << "  qf1=" << glm::to_string(qf1) << '\n';

  assert(equal_quat(qf1, qe));

  glm::quat qconj = glm::conjugate(q);
  std::cout << "qconj=" << glm::to_string(qconj) << '\n';

  glm::quat qf2 = q * qi * qconj;
  std::cout << "  qf2=" << glm::to_string(qf2) << '\n';

  assert(equal_quat(qf2, qe));
}

/*
 * Test rotation of 90 degrees about the y-axis using rotation matrix.
 * Demonstrates:
 * - Converting quaternion to rotation matrix
 * - Matrix multiplication for rotation
 * - Comparing results with expected values
 */
void qtest2() {
  print_header("TEST 2 : Rotation of 90 degrees about the y-axis with matrix");

  const glm::vec3 axis = {0.0f, 1.0f, 0.0f};
  glm::quat q = glm::angleAxis(glm::radians(90.0f), axis);
  std::cout << "    q=" << glm::to_string(q) << '\n';

  glm::vec4 vi = {7.0f, 0.0f, 0.0f, 1.0f};
  std::cout << "   vi=" << glm::to_string(vi) << '\n';

  glm::vec4 ve = {0.0f, 0.0f, -7.0f, 1.0f};
  std::cout << "   ve=" << glm::to_string(ve) << '\n';

  glm::mat4 m = glm::mat4_cast(q);
  std::cout << "    m=" << std::endl;
  print_mat4(m, 7);

  glm::vec4 vf = m * vi;
  std::cout << "   vf=" << glm::to_string(vf) << '\n';

  assert(glm::all(glm::equal(vf, ve, THRESHOLD)));
}

/*
 * Test equivalent rotations using different combinations. Demonstrates that a
 * 900-degree yaw is equivalent to a 180-degree roll followed by a 180-degree
 * pitch. Shows quaternion composition and order of operations. The cleaned
 * output displays the results with -0.0 values converted to 0.0 for clarity.
 */
void qtest3() {
  print_header("TEST 3 : Yaw 900 = Roll 180 + Pitch 180");

  const glm::vec3 axis_x = {1.0f, 0.0f, 0.0f};
  const glm::vec3 axis_y = {0.0f, 1.0f, 0.0f};
  const glm::vec3 axis_z = {0.0f, 0.0f, 1.0f};

  glm::quat qyaw900 = glm::angleAxis(glm::radians(900.0f), axis_y);
  check_unit_quaternion(qyaw900);
  std::cout << "   qyaw900=" << glm::to_string(qyaw900) << '\n';

  glm::quat qroll180 = glm::angleAxis(glm::radians(180.0f), axis_x);
  check_unit_quaternion(qroll180);
  std::cout << "  qroll180=" << glm::to_string(qroll180) << '\n';
  glm::quat qpitch180 = glm::angleAxis(glm::radians(180.0f), axis_z);
  check_unit_quaternion(qpitch180);
  std::cout << " qpitch180=" << glm::to_string(qpitch180) << '\n';
  glm::quat qrp = qpitch180 * qroll180;
  glm::quat qpr = qroll180 * qpitch180;

  glm::quat qi = {0.0f, 1.0f, 0.0f, 0.0f}; // x-axis
  std::cout << "        qi=" << glm::to_string(qi) << '\n';
  glm::quat qe = {0.0f, -1.0f, 0.0f, 0.0f};
  std::cout << "        qe=" << glm::to_string(qe) << '\n';

  glm::quat qfy = qyaw900 * qi * glm::conjugate(qyaw900);
  std::cout << "       qfy=" << glm::to_string(qfy) << '\n';
  assert(equal_quat(qfy, qe));

  glm::quat qfrp = qrp * qi * glm::conjugate(qrp);
  std::cout << "      qfrp=" << glm::to_string(qfrp) << '\n';
  assert(equal_quat(qfrp, qe));

  glm::quat qfpr = qpr * qi * glm::conjugate(qpr);
  std::cout << "      qfpr=" << glm::to_string(qfpr) << '\n';
  assert(equal_quat(qfpr, qe));

  std::cout << "(cln)  qfy=" << glm::to_string(clean_quat(qfy)) << '\n';
  std::cout << "(cln) qfrp=" << glm::to_string(clean_quat(qfrp)) << '\n';
  std::cout << "(cln) qfpr=" << glm::to_string(clean_quat(qfpr)) << '\n';
}

/*
 * Test conversion between angle-axis representation and quaternions.
 * Demonstrates:
 * - Converting from angle-axis to quaternion
 * - Extracting angle and axis from quaternion
 * - Verifying the conversion is reversible
 */
void qtest4() {
  print_header("TEST 4: Q ↔ (angle, axis)");

  float theta_i = glm::radians(45.0f);
  glm::vec3 axis_i = {0.0f, 1.0f, 0.0f};
  glm::quat q = glm::angleAxis(theta_i, axis_i);
  std::cout << "theta_i=" << theta_i << " ";
  std::cout << "axis_i=" << glm::to_string(axis_i) << '\n';
  std::cout << "q=" << glm::to_string(q) << '\n';

  float theta_f = glm::angle(q);
  glm::vec3 axis_f = glm::axis(q);
  std::cout << "theta_f=" << theta_f << " ";
  std::cout << "axis_f=" << glm::to_string(axis_f) << '\n';

  assert((std::abs(theta_i - theta_f) < THRESHOLD) &&
         glm::all(glm::equal(axis_i, axis_f, THRESHOLD)));
}

/*
 * Test Linear interpolation (LERP) between quaternions.
 * Demonstrates:
 * - LERP between 0° and 90° degree rotations
 * - Tests boundary conditions (t=0 and t=1)
 * - Compares results with expected intermediate rotation
 * - Shows that LERP doesn't preserve constant angular velocity
 */
void qtest5() {
  print_header("TEST 5: LERP");

  const glm::vec3 axis = {0.0f, 1.0f, 0.0f};
  glm::quat q0 = glm::angleAxis(glm::radians(0.0f), axis);
  std::cout << "     q0=" << glm::to_string(q0) << '\n';
  glm::quat q1 = glm::angleAxis(glm::radians(90.0f), axis);
  std::cout << "     q1=" << glm::to_string(q1) << '\n';
  glm::quat qe = glm::angleAxis(glm::radians(30.0f), axis);
  std::cout << "     qe=" << glm::to_string(qe) << '\n';

  glm::quat qLerp0 = glm::lerp(q0, q1, 0.0f);
  std::cout << " qLerp0=" << glm::to_string(qLerp0) << '\n';
  assert(equal_quat(qLerp0, q0));

  glm::quat qLerp1 = glm::lerp(q0, q1, 1.0f);
  std::cout << " qLerp1=" << glm::to_string(qLerp1) << '\n';
  assert(equal_quat(qLerp1, q1));

  glm::quat qLerp = glm::lerp(q0, q1, 1.0f / 3.0f);
  std::cout << "  qLerp=" << glm::to_string(qLerp) << '\n';
  assert(!equal_quat(qLerp, qe));
}

/*
 * Test Spherical Linear interpolation (SLERP) between quaternions.
 * Demonstrates:
 * - SLERP between 0° and 90° degree rotations
 * - Tests boundary conditions (t=0 and t=1)
 * - Compares results with expected intermediate rotation
 * - Shows that SLERP preserves constant angular velocity
 */
void qtest6() {
  print_header("TEST 6: SLERP");

  const glm::vec3 axis = {0.0f, 1.0f, 0.0f};
  glm::quat q0 = glm::angleAxis(glm::radians(0.0f), axis);
  std::cout << "      q0=" << glm::to_string(q0) << '\n';
  glm::quat q1 = glm::angleAxis(glm::radians(90.0f), axis);
  std::cout << "      q1=" << glm::to_string(q1) << '\n';
  glm::quat qe = glm::angleAxis(glm::radians(30.0f), axis);
  std::cout << "      qe=" << glm::to_string(qe) << '\n';

  glm::quat qSlerp0 = glm::slerp(q0, q1, 0.0f);
  std::cout << " qSlerp0=" << glm::to_string(qSlerp0) << '\n';
  assert(equal_quat(qSlerp0, q0));

  glm::quat qSlerp1 = glm::slerp(q0, q1, 1.0f);
  std::cout << " qSlerp1=" << glm::to_string(qSlerp1) << '\n';
  assert(equal_quat(qSlerp1, q1));

  glm::quat qSlerp = glm::slerp(q0, q1, 1 / 3.0f);
  std::cout << "  qSlerp=" << glm::to_string(qSlerp) << '\n';
  assert(equal_quat(qSlerp, qe));
}

////////////////////////////////////////////////////////////////////////////////

/**
 * Main function that executes a series of quaternion tests
 *
 * Runs through a collection of test functions that verify various
 * quaternion operations and functionality using GLM library
 *
 * @return EXIT_SUCCESS on successful execution
 */
int main() {
  const auto tests = {qtest1, qtest2, qtest3, qtest4, qtest5, qtest6};
  for (const auto &test : tests) {
    test();
  }
  std::cout << std::endl;
  return EXIT_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
