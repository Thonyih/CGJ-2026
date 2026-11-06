// Lab 2 - GLM
// Runs every exercise, printing intermediate results and PASS/FAIL checks.

#include "exercises.hpp"
#include "helpers.hpp"

#include <cstdlib>   // srand
#include <ctime>     // time
#include <iostream>  // std::cout

int main()
{
    // Start the random numbers from the current time, so every run tests
    // different random vectors. The seed is printed so a run can be repeated:
    // replace time(nullptr) with the printed number to get the same vectors.
    unsigned int seed = (unsigned int)time(nullptr);
    srand(seed);

    // Turn on coloured text in the console (PASS green, FAIL red, ...).
    enableColors();

    std::cout << color(TEXT_BOLD_CYAN) << "Lab 2 - GLM" << color(TEXT_RESET)
              << color(TEXT_GREY) << "   (random seed: " << seed << ")" << color(TEXT_RESET) << "\n";

    exercise1();
    exercise2();
    exercise3();
    exercise4();
    exercise5();

    // Summary of all checks: green if everything passed, red otherwise.
    int passed = getChecksRun() - getChecksFailed();
    std::string summaryColor = TEXT_GREEN;
    if (getChecksFailed() > 0) {
        summaryColor = TEXT_RED;
    }
    std::cout << "\n" << color(summaryColor) << "Checks passed: " << passed << " / " << getChecksRun()
              << color(TEXT_RESET) << "\n";

    // Exit code 0 means "everything passed", 1 means "something failed".
    if (getChecksFailed() == 0) {
        return 0;
    } else {
        return 1;
    }
}
