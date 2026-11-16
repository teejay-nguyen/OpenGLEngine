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

    double lastTime = window.GetTime();

    while (!window.ShouldClose())
    {
        // time since the previous frame, used for frame-rate independent motion
        const double currentTime = window.GetTime();
        const float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        renderer.Update(deltaTime);

        renderer.BeginFrame();
        renderer.Render(window.GetAspectRatio());
        renderer.EndFrame();

        // present the frame and poll events
        window.Update();
    }
}