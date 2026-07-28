#pragma once

struct GLFWwindow;

class Window
{
public:
    Window();
    ~Window();

    bool Create();

    void Update();

    bool ShouldClose() const;

private:
    GLFWwindow* m_window = nullptr;
};