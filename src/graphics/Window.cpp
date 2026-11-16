#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace
{
    void GlfwErrorCallback(int error, const char* description)
    {
        std::cerr << "GLFW error " << error << ": " << description << std::endl;
    }
}

Window::Window() = default;

Window::~Window()
{
    if (m_Window)
    {
        glfwDestroyWindow(m_Window);
    }

    glfwTerminate();
}

bool Window::Create(int width, int height, const char* title)
{
    glfwSetErrorCallback(GlfwErrorCallback);

    if (!glfwInit())
    {
        return false;
    }

    // tell GLFW to create an OpenGL 4.6 core context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // window creation
    m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (!m_Window)
    {
        return false;
    }

    glfwMakeContextCurrent(m_Window);

    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    // lets the static GLFW callbacks find this Window instance
    glfwSetWindowUserPointer(m_Window, this);
    glfwSetFramebufferSizeCallback(m_Window, FramebufferSizeCallback);

    // query the real framebuffer size instead of assuming the requested window size
    glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);
    glViewport(0, 0, m_Width, m_Height);

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

float Window::GetAspectRatio() const
{
    // a minimized window reports 0x0; avoid dividing by zero
    if (m_Height == 0)
    {
        return 1.0f;
    }

    return static_cast<float>(m_Width) / static_cast<float>(m_Height);
}

void Window::ProcessInput()
{
    if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(m_Window, true);
    }
}

void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (self)
    {
        self->m_Width = width;
        self->m_Height = height;
    }

    glViewport(0, 0, width, height);
}
