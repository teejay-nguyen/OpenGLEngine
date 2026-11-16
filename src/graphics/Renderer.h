#pragma once

#include "graphics/Shader.h"
#include "graphics/Mesh.h"
#include "scene/Transform.h"

#include <array>

class Renderer
{
public:
    bool Initialize();

    // advances the demo scene (temporary until a real Scene exists)
    void Update(float deltaTime);

    // clears the framebuffer
    void BeginFrame();

    // draws the scene; aspectRatio = framebuffer width / height
    void Render(float aspectRatio);

    // reserved for end-of-frame work (post-processing, UI, etc.)
    void EndFrame();

private:
    Shader m_Shader;

    // One mesh on the GPU, drawn several times with different model matrices
    Mesh m_Cube;
    std::array<Transform, 2> m_CubeTransforms;
};
