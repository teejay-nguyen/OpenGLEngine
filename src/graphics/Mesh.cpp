#include "Mesh.h"

#include <glad/glad.h>
#include <cstddef>

namespace
{
    GLenum ToOpenGLPrimitive(PrimitiveType type)
    {
        switch (type)
        {
            case PrimitiveType::Points:
                return GL_POINTS;

            case PrimitiveType::Lines:
                return GL_LINES;

            case PrimitiveType::Triangles:
                return GL_TRIANGLES;
        }

        return GL_TRIANGLES;
    }
}

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

bool Mesh::Create(const std::vector<Vertex>& vertices, PrimitiveType primitiveType)
{
    // Vertex data
    if (vertices.empty())
    {
        return false;
    }

    m_VertexCount = static_cast<unsigned int>(vertices.size());

    // Primitive type
    m_PrimitiveType = primitiveType;

    // Generate and bind VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Generate and bind VBO
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // Set VBO data
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        vertices.data(),
        GL_STATIC_DRAW
    );

    // Describe vertex layout
    // Position attribute
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, position)
    );

    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, color)
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

    glDrawArrays(
        ToOpenGLPrimitive(m_PrimitiveType), 
        0, 
        m_VertexCount
    );

    glBindVertexArray(0);
}

