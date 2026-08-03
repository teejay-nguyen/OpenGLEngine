#include "Renderer.h"

#include <glad/glad.h>

bool Renderer::Initialize() 
{
    if (!m_Shader.Create(
        "assets/shaders/basic.vert",
        "assets/shaders/basic.frag"
    ))
    {
        return false;
    }

    if (!m_Triangle.CreateTriangle())
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

    m_Shader.Bind();
    m_Triangle.Draw();
}

void Renderer::EndFrame()
{
    
}