cmake_minimum_required(VERSION 3.20)
project(my-engine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(my-engine
    src/main.cpp
    external/glad/src/glad.c
)

target_include_directories(my-engine PRIVATE external/glad/include)

target_link_libraries(my-engine PRIVATE
    glfw
    glm::glm
    "-framework OpenGL"
)

# Silence macOS OpenGL deprecation warnings
target_compile_definitions(my-engine PRIVATE GL_SILENCE_DEPRECATION)
