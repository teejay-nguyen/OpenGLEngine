#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Window::ProcessInput()
{
    if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(m_Window, true);
    }
}

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

    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(m_Window, FramebufferSizeCallback);

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
    ProcessInput();

    glfwSwapBuffers(m_Window);
    
    glfwPollEvents();
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(m_Window);
}