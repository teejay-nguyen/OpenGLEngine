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

    // A quad: 4 unique vertices shared by 2 triangles via 6 indices.
    // Without an index buffer this would need 6 vertices (2 duplicated).
    std::vector<Vertex> vertices
    {
        { { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } }, // 0: bottom-left  (red)
        { {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f } }, // 1: bottom-right (green)
        { {  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f } }, // 2: top-right    (blue)
        { { -0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 0.0f } }  // 3: top-left     (yellow)
    };

    // Counter-clockwise winding (OpenGL's default front face)
    std::vector<std::uint32_t> indices
    {
        0, 1, 2,
        2, 3, 0
    };

    if (!m_Shader.Create(
        "assets/shaders/basic.vert",
        "assets/shaders/basic.frag"
    ))
    {
        return false;
    }

    if (!m_Quad.Create(vertices, indices))
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
    m_Quad.Draw();
}

void Renderer::EndFrame()
{

}
