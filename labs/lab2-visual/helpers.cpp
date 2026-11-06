#include "helpers.hpp"

#include <cmath>     // std::fabs
#include <cstdlib>   // rand, RAND_MAX
#include <iomanip>   // std::setprecision, std::setw
#include <iostream>  // std::cout

// Windows console functions (only used to switch colours on).
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

// Counters for the checks (how many ran, how many failed).
int checksRun = 0;
int checksFailed = 0;

// ---------------------------------------------------------------------------
// Colours
//
// The console understands special text codes ("ANSI escape codes"):
// printing "\033[32m" makes the following text green, "\033[0m" goes back
// to normal. Windows only understands them after they are switched on.
// ---------------------------------------------------------------------------

bool useColors = false;

void enableColors()
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;

    // If the output is not a console (for example it is saved to a file),
    // GetConsoleMode fails and we simply print without colours.
    if (!GetConsoleMode(console, &mode)) {
        useColors = false;
        return;
    }

    mode = mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    useColors = SetConsoleMode(console, mode) != 0;
}

// Returns the colour code, or nothing if colours are off.
std::string color(std::string code)
{
    if (useColors) {
        return code;
    }
    return "";
}

// ---------------------------------------------------------------------------
// Random numbers
// ---------------------------------------------------------------------------

float randomFloat(float min, float max)
{
    // rand() gives a whole number from 0 to RAND_MAX.
    // Dividing by RAND_MAX turns it into a number from 0.0 to 1.0,
    // which is then stretched to the range [min, max].
    float zeroToOne = (float)rand() / (float)RAND_MAX;
    return min + zeroToOne * (max - min);
}

glm::vec3 randomVec3()
{
    // Values stay small ([-1, 1]) so rounding errors stay much smaller than EPSILON.
    float x = randomFloat(-1.0f, 1.0f);
    float y = randomFloat(-1.0f, 1.0f);
    float z = randomFloat(-1.0f, 1.0f);
    return glm::vec3(x, y, z);
}

// ---------------------------------------------------------------------------
// Comparing
// ---------------------------------------------------------------------------

bool nearlyEqualFloat(float a, float b)
{
    float difference = std::fabs(a - b);
    return difference < EPSILON;
}

bool nearlyEqualVec3(glm::vec3 a, glm::vec3 b)
{
    // Two vectors are equal when every component is equal.
    bool xEqual = nearlyEqualFloat(a.x, b.x);
    bool yEqual = nearlyEqualFloat(a.y, b.y);
    bool zEqual = nearlyEqualFloat(a.z, b.z);
    return xEqual && yEqual && zEqual;
}

bool nearlyEqualMat3(glm::mat3 a, glm::mat3 b)
{
    // Compare all 9 numbers one by one.
    for (int column = 0; column < 3; column++) {
        for (int row = 0; row < 3; row++) {
            if (!nearlyEqualFloat(a[column][row], b[column][row])) {
                return false;
            }
        }
    }
    return true;
}

// ---------------------------------------------------------------------------
// Printing
// ---------------------------------------------------------------------------

void printTitle(std::string title)
{
    std::cout << "\n" << color(TEXT_BOLD_CYAN) << "=== " << title << " ===" << color(TEXT_RESET) << "\n";
}

void printText(std::string text)
{
    std::cout << "  " << text << "\n";
}

void printFloat(std::string name, float value)
{
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "  " << name << " = " << value << "\n";
}

void printVec3(std::string name, glm::vec3 v)
{
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "  " << name << " = (" << v.x << ", " << v.y << ", " << v.z << ")\n";
}

void printMat3(std::string name, glm::mat3 m)
{
    // IMPORTANT: GLM stores matrices column by column ("column-major"),
    // so m[column][row] is the number in that row and column.
    // Here we print row by row, the way matrices are written on paper.
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "  " << name << " =\n";
    for (int row = 0; row < 3; row++) {
        std::cout << "    [";
        for (int column = 0; column < 3; column++) {
            std::cout << std::setw(9) << m[column][row];
        }
        std::cout << " ]\n";
    }
}

// ---------------------------------------------------------------------------
// Checks
// ---------------------------------------------------------------------------

void check(std::string description, bool passed)
{
    checksRun = checksRun + 1;

    if (passed) {
        std::cout << "  " << color(TEXT_GREEN) << "[PASS]" << color(TEXT_RESET) << " " << description << "\n";
    } else {
        std::cout << "  " << color(TEXT_RED) << "[FAIL]" << color(TEXT_RESET) << " " << description << "\n";
        checksFailed = checksFailed + 1;
    }
}

int getChecksRun()
{
    return checksRun;
}

int getChecksFailed()
{
    return checksFailed;
}
