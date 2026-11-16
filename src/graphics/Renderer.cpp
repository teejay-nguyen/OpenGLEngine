#include "Renderer.h"

#include <glad/glad.h>

#include <iostream>

namespace
{
    void APIENTRY GLDebugCallback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam)
    {
        // notifications are very chatty (buffer usage hints, etc.)
        if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
        {
            return;
        }

        const char* severityName =
            severity == GL_DEBUG_SEVERITY_HIGH   ? "HIGH" :
            severity == GL_DEBUG_SEVERITY_MEDIUM ? "MEDIUM" :
                                                   "LOW";

        std::cerr << "[OpenGL " << severityName << "] (" << id << ") "
                  << message << std::endl;
    }

    void EnableDebugOutput()
    {
        GLint flags = 0;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

        // only active when the window was created with a debug context (Debug builds)
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(GLDebugCallback, nullptr);
        }
    }
}

bool Renderer::Initialize()
{
    EnableDebugOutput();

    std::vector<Vertex> vertices
    {
        {
            { -0.5f, -0.5f, 0.0f },
            {  1.0f,  0.0f, 0.0f }
        },
        {
            {  0.5f, -0.5f, 0.0f },
            {  0.0f,  1.0f, 0.0f }
        },
        {
            {  0.0f,  0.5f, 0.0f },
            {  0.0f,  0.0f, 1.0f }
        }
    };

    if (!m_Shader.Create(
        "assets/shaders/basic.vert",
        "assets/shaders/basic.frag"
    ))
    {
        return false;
    }

    if (!m_Triangle.Create(vertices))
    {
        return false;
    }

    glEnable(GL_DEPTH_TEST);

    return true;
}

void Renderer::BeginFrame()
{
    glClearColor(
        0.1f,
        0.15f,
        0.2f,
        1.0f
    );

    glClear(
        GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT
    );
}

void Renderer::Render()
{
    m_Shader.Bind();
    m_Triangle.Draw();
}

void Renderer::EndFrame()
{

}
