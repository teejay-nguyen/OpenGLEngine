#include "Renderer.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

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

    // A unit cube: 8 corners, colored by position (RGB = XYZ mapped to 0..1).
    // Corners are shared between faces through the index buffer.
    std::vector<Vertex> vertices
    {
        { { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, 0.0f } }, // 0
        { {  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } }, // 1
        { {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f, 0.0f } }, // 2
        { { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } }, // 3
        { { -0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f } }, // 4
        { {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f } }, // 5
        { {  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f } }, // 6
        { { -0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f, 1.0f } }  // 7
    };

    // Counter-clockwise winding when viewed from outside each face,
    // which is what back-face culling relies on.
    std::vector<std::uint32_t> indices
    {
        4, 5, 6,  6, 7, 4, // front  (+Z)
        1, 0, 3,  3, 2, 1, // back   (-Z)
        0, 4, 7,  7, 3, 0, // left   (-X)
        5, 1, 2,  2, 6, 5, // right  (+X)
        7, 6, 2,  2, 3, 7, // top    (+Y)
        0, 1, 5,  5, 4, 0  // bottom (-Y)
    };

    if (!m_Shader.Create(
        "assets/shaders/basic.vert",
        "assets/shaders/basic.frag"
    ))
    {
        return false;
    }

    if (!m_Cube.Create(vertices, indices))
    {
        return false;
    }

    // Two instances of the same mesh, placed differently
    m_CubeTransforms[0].position = { -0.9f, 0.0f, 0.0f };

    m_CubeTransforms[1].position = { 0.9f, 0.0f, 0.0f };
    m_CubeTransforms[1].scale = glm::vec3(0.6f);

    // Rest the cubes on the ground grid (unit cube is centered, so lift by half its size)
    m_CubeTransforms[0].position.y = 0.5f;
    m_CubeTransforms[1].position.y = 0.3f;

    if (!CreateGrid(10, 1.0f))
    {
        return false;
    }

    glEnable(GL_DEPTH_TEST);

    // Skip triangles facing away from the camera (half the cube's faces at any time)
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    return true;
}

void Renderer::Update(float deltaTime)
{
    // Rotation speeds in degrees per second; scaling by deltaTime keeps the
    // motion independent of frame rate.
    // Only the small cube spins; a static scene makes the camera motion easy to judge.
    m_CubeTransforms[1].rotation.y += 30.0f * deltaTime;
}

bool Renderer::CreateGrid(int halfExtent, float spacing)
{
    // A flat grid on the XZ plane (y = 0), drawn as GL_LINES: every pair of
    // vertices is one line segment. The center lines are colored as axes:
    // X = red, Z = blue (Y is "up" and does not lie in the grid).
    const glm::vec3 lineColor { 0.35f, 0.38f, 0.42f };
    const glm::vec3 xAxisColor { 0.85f, 0.25f, 0.25f };
    const glm::vec3 zAxisColor { 0.25f, 0.45f, 0.90f };

    const float extent = static_cast<float>(halfExtent) * spacing;

    std::vector<Vertex> vertices;
    vertices.reserve(static_cast<std::size_t>(halfExtent * 2 + 1) * 4);

    for (int i = -halfExtent; i <= halfExtent; ++i)
    {
        const float offset = static_cast<float>(i) * spacing;

        // line parallel to X (constant z); the one at z = 0 is the X axis
        const glm::vec3 xColor = (i == 0) ? xAxisColor : lineColor;
        vertices.push_back({ { -extent, 0.0f, offset }, xColor });
        vertices.push_back({ {  extent, 0.0f, offset }, xColor });

        // line parallel to Z (constant x); the one at x = 0 is the Z axis
        const glm::vec3 zColor = (i == 0) ? zAxisColor : lineColor;
        vertices.push_back({ { offset, 0.0f, -extent }, zColor });
        vertices.push_back({ { offset, 0.0f,  extent }, zColor });
    }

    return m_Grid.Create(vertices, PrimitiveType::Lines);
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

void Renderer::Render(const OrbitCamera& camera, float aspectRatio)
{
    // Per-frame uniforms: shared by every object
    m_Shader.SetMat4("uView", camera.GetViewMatrix());
    m_Shader.SetMat4("uProjection", camera.GetProjectionMatrix(aspectRatio));

    m_Shader.Bind();

    // Grid lives at the world origin, so its model matrix is identity
    m_Shader.SetMat4("uModel", glm::mat4(1.0f));
    m_Grid.Draw();

    // Per-object uniform: only the model matrix changes between draws
    for (const Transform& transform : m_CubeTransforms)
    {
        m_Shader.SetMat4("uModel", transform.GetMatrix());
        m_Cube.Draw();
    }
}

void Renderer::EndFrame()
{

}
