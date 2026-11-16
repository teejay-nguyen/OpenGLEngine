#pragma once

struct GLFWwindow;

class Window
{
public:
    Window();
    ~Window();

    // A Window owns the GLFW window and context, so it is non-copyable.
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool Create(int width = 1280, int height = 720, const char* title = "Graphics Engine");

    void Update();

    bool ShouldClose() const;

    // Framebuffer size in pixels (can differ from window size on high-DPI displays)
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    float GetAspectRatio() const;

private:
    void ProcessInput();

    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

private:
    GLFWwindow* m_Window = nullptr;

    int m_Width = 0;
    int m_Height = 0;
};
