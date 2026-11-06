# Lab 2 - GLM
# This file is self-contained: the lab can be built on its own
# (open this folder) or as part of the main project (from the root folder).

cmake_minimum_required(VERSION 3.20)
project(lab2-glm LANGUAGES CXX)

if(NOT WIN32)
    message(FATAL_ERROR "This project only supports Windows.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Console program, no window.
add_executable(lab2-glm
    main.cpp
    helpers.cpp
    exercises.cpp
)

# GLM 1.0.1 is included in this lab's libraries\glm folder.
# It is header-only, so it only needs its include folder (nothing to link).
target_include_directories(lab2-glm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libraries/glm)

# Run from this lab's folder; show it under "labs" in the solution.
set_target_properties(lab2-glm PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    FOLDER "labs")

# When this folder is opened on its own, make lab2-glm the startup project.
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT lab2-glm)
