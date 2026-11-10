# Assignment A1 - Math Challenge (lab of 10 Nov)
# This file is self-contained: it can be built on its own
# (open this folder) or as part of the main project (from the root folder).

cmake_minimum_required(VERSION 3.20)
project(A1-lab10nov LANGUAGES CXX)

if(NOT WIN32)
    message(FATAL_ERROR "This project only supports Windows.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Console program, no window. All the code is in main.cpp.
add_executable(A1-lab10nov main.cpp)

# GLM 1.0.1 is included in this folder's libraries\glm.
# It is header-only, so it only needs its include folder (nothing to link).
target_include_directories(A1-lab10nov PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libraries/glm)

# Run from this folder; show it under "labs" in the solution.
set_target_properties(A1-lab10nov PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    FOLDER "labs")

# When this folder is opened on its own, make A1-lab10nov the startup project.
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT A1-lab10nov)
