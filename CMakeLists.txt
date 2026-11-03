cmake_minimum_required(VERSION 3.20)
project(my-engine LANGUAGES C CXX)

if(NOT WIN32)
    message(FATAL_ERROR "This project only supports Windows.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# GLFW: window, OpenGL context and input
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE)

# GLM: maths library
FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
    GIT_SHALLOW TRUE)

# GLEW: OpenGL function loader (the loader used by the course labs).
# The release archive ships pre-generated sources, so no Python is needed.
FetchContent_Declare(glew
    URL https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.tgz
    URL_HASH SHA256=d4fc82893cfb00109578d0a1a2337fb8ca335b3ceccf97b97e5cc7f08e4353e1
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)

FetchContent_MakeAvailable(glfw glm glew)

# Build GLEW as a static library from its single source file.
add_library(glew STATIC ${glew_SOURCE_DIR}/src/glew.c)
target_include_directories(glew PUBLIC ${glew_SOURCE_DIR}/include)
target_compile_definitions(glew PUBLIC GLEW_STATIC)
target_link_libraries(glew PUBLIC opengl32)

add_executable(my-engine
    src/main.cpp
)

target_link_libraries(my-engine PRIVATE
    glew
    glfw
    glm::glm
)

# Run from the project root in Visual Studio so relative paths
# (shaders/, assets/) resolve the same as in VS Code.
set_target_properties(my-engine PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT my-engine)
