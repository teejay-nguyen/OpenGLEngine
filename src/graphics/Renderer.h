#pragma once

#include "graphics/Shader.h"
#include "graphics/Mesh.h"
#include "scene/OrbitCamera.h"
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

    // draws the scene from the camera; aspectRatio = framebuffer width / height
    void Render(const OrbitCamera& camera, float aspectRatio);

    // reserved for end-of-frame work (post-processing, UI, etc.)
    void EndFrame();

private:
    // builds a (2 * halfExtent) x (2 * halfExtent) line grid on the XZ plane
    bool CreateGrid(int halfExtent, float spacing);

private:
    Shader m_Shader;

    // ground reference grid + X/Z axes
    Mesh m_Grid;

    // One mesh on the GPU, drawn several times with different model matrices
    Mesh m_Cube;
    std::array<Transform, 2> m_CubeTransforms;
};
