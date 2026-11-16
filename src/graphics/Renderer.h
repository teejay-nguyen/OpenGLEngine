#pragma once

#include "graphics/Shader.h"
#include "graphics/Mesh.h"

class Renderer
{
public:
    bool Initialize();

    // clears the framebuffer
    void BeginFrame();

    // draws the scene
    void Render();

    // reserved for end-of-frame work (post-processing, UI, etc.)
    void EndFrame();

private:
    Shader m_Shader;

    Mesh m_Triangle;
};
