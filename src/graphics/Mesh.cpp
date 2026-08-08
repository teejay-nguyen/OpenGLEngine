#include "Mesh.h"

#include <glad/glad.h>

Mesh::Mesh() = default;

Mesh::~Mesh()
{
    if (m_VBO != 0)
    {
        glDeleteBuffers(1, &m_VBO);
    }

    if (m_VAO != 0)
    {
        glDeleteVertexArrays(1, &m_VAO);
    }
}

bool Mesh::CreateTriangle()
{
    // Vertex data
    float vertices[]
    {
        // Position          // Color
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // Red
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // Green
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // Blue
    };

    // Generate and bind VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Generate and bind VBO
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // Set VBO data
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    // Describe vertex layout
    // Position attribute
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)(3 * sizeof(float))
    );

    glEnableVertexAttribArray(1);

    // Unbind VBO & VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}

void Mesh::Draw() const
{
    // Bind VAO
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}