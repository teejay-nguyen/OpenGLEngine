cmake_minimum_required(VERSION 3.20)

project(OpenGLEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(OpenGLEngine
    src/main.cpp
    external/glad/src/glad.c
)

target_include_directories(OpenGLEngine PRIVATE
    external/glad/include
)

target_link_libraries(OpenGLEngine PRIVATE
    glfw
    glm::glm
)