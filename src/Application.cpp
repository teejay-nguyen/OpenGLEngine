#include "Application.h"
#include "graphics/Window.h"

void Application::Run()
{
    Window window;

    // window creation, encapsulated within the Window class
    if (!window.Create())
        return;

    while (!window.ShouldClose())
    {
        window.Update();
    }
}