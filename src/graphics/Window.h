#pragma once

#include <glm/vec2.hpp>

struct GLFWwindow;

// Engine-side input names, so code outside Window never needs GLFW headers.
enum class MouseButton
{
    Left,
    Right,
    Middle
};

enum class Key
{
    R
};

class Window
{
public:
    Window();
    ~Window();

    // A Window owns the GLFW window and context, so it is non-copyable.
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool Create(int width = 1280, int height = 720, const char* title = "Graphics Engine");

    // Presents the frame, then polls events (which fills in next frame's input).
    void Update();

    bool ShouldClose() const;

    // Framebuffer size in pixels (can differ from window size on high-DPI displays)
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    float GetAspectRatio() const;

    // seconds since the window system was initialized
    double GetTime() const;

    // --- Input (state gathered during the most recent event poll) ---

    bool IsMouseButtonDown(MouseButton button) const;

    // true only on the frame the key went down
    bool WasKeyPressed(Key key) const;

    // cursor movement in screen coordinates since the last frame (+y = down)
    glm::vec2 GetMouseDelta() const { return m_MouseDelta; }

    // scroll wheel movement since the last frame (+y = scroll up / away from user)
    float GetScrollDelta() const { return m_ScrollDelta; }

private:
    void ProcessInput();

    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void CursorPosCallback(GLFWwindow* window, double x, double y);
    static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
    GLFWwindow* m_Window = nullptr;

    int m_Width = 0;
    int m_Height = 0;

    // Per-frame input. Callbacks accumulate into these during glfwPollEvents();
    // they are reset right before the next poll.
    glm::dvec2 m_LastCursorPos { 0.0 };
    glm::vec2 m_MouseDelta { 0.0f };
    float m_ScrollDelta = 0.0f;
    bool m_ResetKeyPressed = false;
};
