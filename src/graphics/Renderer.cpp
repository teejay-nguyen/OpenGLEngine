#include "Renderer.h"

#include <glad/glad.h>

void Renderer::Initialize() 
{
    
}

void Renderer::BeginFrame()
{
    glClearColor(
        0.1f,
        0.15f,
        0.2f,
        1.0f
    );

    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::EndFrame()
{
}