# Lab 2 - GLM, visual version
# An OpenGL window that draws what each Lab 2 exercise computes.
# This file is self-contained: it can be built on its own
# (open this folder) or as part of the main project (from the root folder).

cmake_minimum_required(VERSION 3.20)
project(lab2-visual LANGUAGES C CXX)

if(NOT WIN32)
    message(FATAL_ERROR "This project only supports Windows.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------------------------------------------------------------------------
# Libraries: all included in this folder's libraries\ folder.
#   libraries\glm   GLM 1.0.1   maths (header-only)
#   libraries\glfw  GLFW 3.4    window, OpenGL context and input
#   libraries\glew  GLEW 2.2.0  OpenGL function loader
# When built from the main project, GLFW and GLEW already exist there
# (same versions), so they are not added a second time.
# ---------------------------------------------------------------------------
if(NOT TARGET glfw)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    add_subdirectory(libraries/glfw)
endif()

if(NOT TARGET glew)
    add_library(glew STATIC libraries/glew/src/glew.c)
    target_include_directories(glew PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libraries/glew/include)
    target_compile_definitions(glew PUBLIC GLEW_STATIC)
    target_link_libraries(glew PUBLIC opengl32)
endif()

# ---------------------------------------------------------------------------
# lab2-visual
# exercises.cpp / helpers.cpp are copies of Lab 2's files (same maths).
# ---------------------------------------------------------------------------
add_executable(lab2-visual
    main.cpp
    renderer.cpp
    scenes.cpp
    exercises.cpp
    helpers.cpp
)
target_include_directories(lab2-visual PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libraries/glm)
target_link_libraries(lab2-visual PRIVATE glew glfw)

# Run from this folder; show it under "labs" in the solution.
set_target_properties(lab2-visual PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    FOLDER "labs")

# When this folder is opened on its own, make lab2-visual the startup project.
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT lab2-visual)
