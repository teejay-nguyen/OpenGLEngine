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
    m_CubeTransforms[0].rotation.y += 45.0f * deltaTime;
    m_CubeTransforms[0].rotation.x += 20.0f * deltaTime;

    m_CubeTransforms[1].rotation.z -= 90.0f * deltaTime;
    m_CubeTransforms[1].rotation.y += 30.0f * deltaTime;
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

void Renderer::Render(float aspectRatio)
{
    // Fixed camera for now; milestone 3 replaces this with an orbit camera.
    const glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 1.5f, 4.0f), // eye
        glm::vec3(0.0f, 0.0f, 0.0f), // target
        glm::vec3(0.0f, 1.0f, 0.0f)  // up
    );

    const glm::mat4 projection = glm::perspective(
        glm::radians(45.0f), // vertical field of view
        aspectRatio,
        0.1f,                // near plane
        100.0f               // far plane
    );

    // Per-frame uniforms: shared by every object
    m_Shader.SetMat4("uView", view);
    m_Shader.SetMat4("uProjection", projection);

    m_Shader.Bind();

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
