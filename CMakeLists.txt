cmake_minimum_required(VERSION 3.20)

project(GraphicsEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)

add_executable(GraphicsEngine
    src/main.cpp
    src/Application.cpp
    src/graphics/Window.cpp

    external/glad/src/glad.c
)

target_include_directories(GraphicsEngine PRIVATE
    external/glad/include
)

target_link_libraries(GraphicsEngine PRIVATE
    glfw
)