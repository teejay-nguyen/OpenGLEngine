#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Window::Window()
{
    // TODO:: implement
}

Window::~Window()
{
    glfwTerminate();
}

bool Window::Create()
{
    if (!glfwInit())
    {
        return false;
    }

    // tell GLFW to create an OpenGL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // window creation
    m_Window = glfwCreateWindow(
        1280,
        720,
        "Graphics Engine",
        nullptr,
        nullptr
    );

    if (!m_Window)
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_Window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return false;
    }

    // hardcoded viewport TODO: make it dynamic
    glViewport(0, 0, 1280, 720);

    return true;
}

void Window::Update()
{
    glClearColor(0.1f, 0.15f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(m_Window);
}