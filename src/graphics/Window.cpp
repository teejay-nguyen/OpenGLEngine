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

#ifndef NDEBUG
    // debug contexts report errors through glDebugMessageCallback (see Renderer)
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

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
    glfwSetCursorPosCallback(m_Window, CursorPosCallback);
    glfwSetScrollCallback(m_Window, ScrollCallback);
    glfwSetKeyCallback(m_Window, KeyCallback);

    // start delta tracking from the current cursor position (avoids a jump on the first move)
    glfwGetCursorPos(m_Window, &m_LastCursorPos.x, &m_LastCursorPos.y);

    // query the real framebuffer size instead of assuming the requested window size
    glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);
    glViewport(0, 0, m_Width, m_Height);

    return true;
}

void Window::Update()
{
    ProcessInput();

    glfwSwapBuffers(m_Window);

    // clear last frame's input, then let the callbacks accumulate this frame's
    m_MouseDelta = glm::vec2(0.0f);
    m_ScrollDelta = 0.0f;
    m_ResetKeyPressed = false;

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

double Window::GetTime() const
{
    return glfwGetTime();
}

bool Window::IsMouseButtonDown(MouseButton button) const
{
    int glfwButton = GLFW_MOUSE_BUTTON_LEFT;

    switch (button)
    {
        case MouseButton::Left:   glfwButton = GLFW_MOUSE_BUTTON_LEFT;   break;
        case MouseButton::Right:  glfwButton = GLFW_MOUSE_BUTTON_RIGHT;  break;
        case MouseButton::Middle: glfwButton = GLFW_MOUSE_BUTTON_MIDDLE; break;
    }

    return glfwGetMouseButton(m_Window, glfwButton) == GLFW_PRESS;
}

bool Window::WasKeyPressed(Key key) const
{
    switch (key)
    {
        case Key::R: return m_ResetKeyPressed;
    }

    return false;
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

void Window::CursorPosCallback(GLFWwindow* window, double x, double y)
{
    auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (!self)
    {
        return;
    }

    // several cursor events can arrive in one poll; sum them
    self->m_MouseDelta += glm::vec2(
        static_cast<float>(x - self->m_LastCursorPos.x),
        static_cast<float>(y - self->m_LastCursorPos.y)
    );

    self->m_LastCursorPos = { x, y };
}

void Window::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (self)
    {
        self->m_ScrollDelta += static_cast<float>(yOffset);
    }
}

void Window::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (!self || action != GLFW_PRESS)
    {
        return;
    }

    if (key == GLFW_KEY_R)
    {
        self->m_ResetKeyPressed = true;
    }
}
