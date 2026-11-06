#pragma once

// Helper functions used by all the exercises:
// random numbers, comparing floats, printing, and PASS/FAIL checks.

#include <string>
#include <glm/glm.hpp>

// Floats are never exactly equal after maths (rounding), so two numbers
// count as "equal" when they differ by less than this small value.
const float EPSILON = 0.0001f;

// ---- Colours ----
// Text colour codes (ANSI escape codes). Print one, then the text, then TEXT_RESET.
const std::string TEXT_RESET = "\033[0m";
const std::string TEXT_GREEN = "\033[32m";
const std::string TEXT_RED = "\033[31m";
const std::string TEXT_YELLOW = "\033[33m";
const std::string TEXT_GREY = "\033[90m";
const std::string TEXT_BOLD_CYAN = "\033[1;36m";

void enableColors();                  // call once at the start of main
std::string color(std::string code);  // the code, or "" when colours are off

// ---- Random numbers ----
float randomFloat(float min, float max);   // a random number between min and max
glm::vec3 randomVec3();                     // a vector with random x, y, z in [-1, 1]

// ---- Comparing ----
bool nearlyEqualFloat(float a, float b);
bool nearlyEqualVec3(glm::vec3 a, glm::vec3 b);
bool nearlyEqualMat3(glm::mat3 a, glm::mat3 b);

// ---- Printing ----
void printTitle(std::string title);
void printText(std::string text);
void printFloat(std::string name, float value);
void printVec3(std::string name, glm::vec3 v);
void printMat3(std::string name, glm::mat3 m);

// ---- Checks ----
// Prints [PASS] or [FAIL] followed by the description, and counts the result.
void check(std::string description, bool passed);
int getChecksRun();
int getChecksFailed();
