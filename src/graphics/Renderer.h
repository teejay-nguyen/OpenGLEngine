#pragma once

#include "graphics/Shader.h"
#include "graphics/Mesh.h"

class Renderer
{
public:
    bool Initialize();

    void BeginFrame();

    void EndFrame();

private:

    Shader m_Shader;

    Mesh m_Triangle;
};