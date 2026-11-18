#include "Application.h"
#include "../graphics/Window.h"
#include "../graphics/Renderer.h"
#include "../scene/OrbitCamera.h"

namespace
{
    // Viewer-style controls:
    //   left drag          -> orbit
    //   right/middle drag  -> pan
    //   scroll             -> zoom
    //   R                  -> reset view
    void UpdateCameraFromInput(const Window& window, OrbitCamera& camera)
    {
        const glm::vec2 mouseDelta = window.GetMouseDelta();

        if (window.IsMouseButtonDown(MouseButton::Left))
        {
            camera.Orbit(mouseDelta.x, mouseDelta.y);
        }
        else if (window.IsMouseButtonDown(MouseButton::Right) ||
                 window.IsMouseButtonDown(MouseButton::Middle))
        {
            camera.Pan(mouseDelta.x, mouseDelta.y, static_cast<float>(window.GetHeight()));
        }

        const float scroll = window.GetScrollDelta();

        if (scroll != 0.0f)
        {
            camera.Zoom(scroll);
        }

        if (window.WasKeyPressed(Key::R))
        {
            camera.Reset();
        }
    }
}

void Application::Run()
{
    Window window;

    // window creation, encapsulated within the Window class
    if (!window.Create())
        return;

    Renderer renderer;

    if (!renderer.Initialize())
        return;

    OrbitCamera camera;

    double lastTime = window.GetTime();

    while (!window.ShouldClose())
    {
        // time since the previous frame, used for frame-rate independent motion
        const double currentTime = window.GetTime();
        const float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        UpdateCameraFromInput(window, camera);
        renderer.Update(deltaTime);

        renderer.BeginFrame();
        renderer.Render(camera, window.GetAspectRatio());
        renderer.EndFrame();

        // present the frame and poll events (gathers next frame's input)
        window.Update();
    }
}
