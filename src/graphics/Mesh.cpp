#include "Mesh.h"

#include <glad/glad.h>

Mesh::~Mesh()
{
    if (m_VBO != 0)
    {
        glDeleteBuffers(1, &m_VBO);
    }
}

bool Mesh::CreateTriangle()
{
    // Vertex data
    float vertices[]
    {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    // Generate VAO
    glGenVertexArrays(1, &m_VAO);

    // Bind VAO
    glBindVertexArray(
        m_VAO
    );

    glBindBuffer(
        GL_ARRAY_BUFFER,
        m_VBO
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    // Generate VBO
    glGenBuffers(1, &m_VBO);

    // Bind VBO
    glBindBuffer(
        GL_ARRAY_BUFFER,
        m_VBO
    );
    
    // Set VBO data
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    // Unbind VBO
    glBindBuffer(
        GL_ARRAY_BUFFER,
        0
    );

    // Unbind VAO
    glBindVertexArray(
        0
    );

    return true;
}