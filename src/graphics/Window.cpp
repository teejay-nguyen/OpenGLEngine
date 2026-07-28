#include "Window.h"

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
    return true;
}

void Window::Update()
{
    // TODO:: implement
}

bool Window::ShouldClose() const
{
    return false;
}