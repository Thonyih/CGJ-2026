# Lecture 4 - Hello 2D World (teacher's example, mgl framework)
# Starting point for Assignment A2.
# This file is self-contained: it can be built on its own
# (open this folder) or as part of the main project (from the root folder).

cmake_minimum_required(VERSION 3.20)
project(hello-2d-world LANGUAGES C CXX)

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
# hello-2d-world: the teacher's code, unchanged (mgl\ and src\).
# ---------------------------------------------------------------------------
add_executable(hello-2d-world
    src/hello-2d-world.cpp
    mgl/mglApp.cpp
    mgl/mglError.cpp
    mgl/mglShader.cpp
)
target_include_directories(hello-2d-world PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libraries/glm)
target_link_libraries(hello-2d-world PRIVATE glew glfw)

# mgl uses "#ifdef DEBUG" for the OpenGL debug output and version info.
# MSVC only defines _DEBUG, so define DEBUG in Debug builds.
target_compile_definitions(hello-2d-world PRIVATE $<$<CONFIG:Debug>:DEBUG>)

# The shaders are opened by file name ("clip-vs.glsl"), so the program
# must run from the folder that has them:
#  - Visual Studio solution: run from src\
#  - Open Folder / VS Code: the shaders are copied next to the .exe
set_target_properties(hello-2d-world PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src"
    FOLDER "labs")
add_custom_command(TARGET hello-2d-world POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/src/clip-vs.glsl
        ${CMAKE_CURRENT_SOURCE_DIR}/src/clip-fs.glsl
        $<TARGET_FILE_DIR:hello-2d-world>)

# When this folder is opened on its own, make hello-2d-world the startup project.
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT hello-2d-world)
