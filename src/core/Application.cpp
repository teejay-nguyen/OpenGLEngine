#include "Application.h"
#include "../graphics/Window.h"
#include "../graphics/Renderer.h"

void Application::Run()
{
    Window window;

    // window creation, encapsulated within the Window class
    if (!window.Create())
        return;

    Renderer renderer;

    if (!renderer.Initialize())
        return;

    while (!window.ShouldClose())
    {
        renderer.BeginFrame();

        window.Update();

        renderer.EndFrame();
    }
}