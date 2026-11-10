cmake_minimum_required(VERSION 3.20)
project(my-engine LANGUAGES C CXX)

if(NOT WIN32)
    message(FATAL_ERROR "This project only supports Windows.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------------------------------------------------------------------------
# Libraries: all included in the libraries\ folder (no downloads needed).
#   libraries\glfw  GLFW 3.4    window, OpenGL context and input
#   libraries\glew  GLEW 2.2.0  OpenGL function loader
#   libraries\glm   GLM 1.0.1   maths (header-only)
# ---------------------------------------------------------------------------

# GLFW: built from source as a static library (no DLL to copy).
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(libraries/glfw)

# GLEW: built from its single source file as a static library (no DLL to copy).
add_library(glew STATIC libraries/glew/src/glew.c)
target_include_directories(glew PUBLIC libraries/glew/include)
target_compile_definitions(glew PUBLIC GLEW_STATIC)
target_link_libraries(glew PUBLIC opengl32)

# ---------------------------------------------------------------------------
# my-engine
# ---------------------------------------------------------------------------
add_executable(my-engine
    src/main.cpp
)

# GLM is header-only: it only needs its include folder.
target_include_directories(my-engine PRIVATE libraries/glm)

target_link_libraries(my-engine PRIVATE
    glew
    glfw
)

# Run from the project root in Visual Studio so relative paths
# (shaders/, assets/) resolve the same as in VS Code.
set_target_properties(my-engine PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT my-engine)

# ---------------------------------------------------------------------------
# Labs: each one is self-contained (own CMakeLists.txt and libraries\ folder).
# ---------------------------------------------------------------------------
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
add_subdirectory(labs/lab2-glm)
add_subdirectory(labs/lab2-visual)
add_subdirectory(labs/A1-lab10nov)
