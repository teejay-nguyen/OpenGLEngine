#include "Mesh.h"

#include <glad/glad.h>

#include <cstddef>
#include <iostream>
#include <utility>

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
    Release();
}

Mesh::Mesh(Mesh&& other) noexcept
    : m_VAO(std::exchange(other.m_VAO, 0))
    , m_VBO(std::exchange(other.m_VBO, 0))
    , m_EBO(std::exchange(other.m_EBO, 0))
    , m_VertexCount(std::exchange(other.m_VertexCount, 0))
    , m_IndexCount(std::exchange(other.m_IndexCount, 0))
    , m_PrimitiveType(other.m_PrimitiveType)
{
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    if (this != &other)
    {
        Release();

        m_VAO = std::exchange(other.m_VAO, 0);
        m_VBO = std::exchange(other.m_VBO, 0);
        m_EBO = std::exchange(other.m_EBO, 0);
        m_VertexCount = std::exchange(other.m_VertexCount, 0);
        m_IndexCount = std::exchange(other.m_IndexCount, 0);
        m_PrimitiveType = other.m_PrimitiveType;
    }

    return *this;
}

void Mesh::Release()
{
    if (m_EBO != 0)
    {
        glDeleteBuffers(1, &m_EBO);
        m_EBO = 0;
    }

    if (m_VBO != 0)
    {
        glDeleteBuffers(1, &m_VBO);
        m_VBO = 0;
    }

    if (m_VAO != 0)
    {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }

    m_VertexCount = 0;
    m_IndexCount = 0;
}

bool Mesh::Create(const std::vector<Vertex>& vertices, PrimitiveType primitiveType)
{
    return Create(vertices, {}, primitiveType);
}

bool Mesh::Create(
    const std::vector<Vertex>& vertices,
    const std::vector<std::uint32_t>& indices,
    PrimitiveType primitiveType)
{
    // Vertex data
    if (vertices.empty())
    {
        return false;
    }

    // An out-of-range index makes the GPU read past the end of the vertex buffer,
    // so reject bad data here (this will matter once indices come from model files).
    for (std::uint32_t index : indices)
    {
        if (index >= vertices.size())
        {
            std::cerr << "Mesh index " << index << " is out of range (vertex count "
                      << vertices.size() << ")" << std::endl;
            return false;
        }
    }

    // Allow re-creating a mesh without leaking the previous GPU buffers
    Release();

    m_VertexCount = static_cast<unsigned int>(vertices.size());
    m_IndexCount = static_cast<unsigned int>(indices.size());

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

    // Generate and fill EBO (optional)
    // The GL_ELEMENT_ARRAY_BUFFER binding is stored *inside* the bound VAO,
    // so binding it here is what associates the indices with this mesh.
    if (!indices.empty())
    {
        glGenBuffers(1, &m_EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(std::uint32_t),
            indices.data(),
            GL_STATIC_DRAW
        );
    }

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

    // Unbind the VAO *first*: unbinding the EBO while the VAO is still bound
    // would remove the index buffer from the VAO.
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return true;
}

void Mesh::Draw() const
{
    if (m_VAO == 0)
    {
        return;
    }

    // Bind VAO (restores VBO layout and EBO binding)
    glBindVertexArray(m_VAO);

    if (IsIndexed())
    {
        glDrawElements(
            ToOpenGLPrimitive(m_PrimitiveType),
            m_IndexCount,
            GL_UNSIGNED_INT,
            nullptr
        );
    }
    else
    {
        glDrawArrays(
            ToOpenGLPrimitive(m_PrimitiveType),
            0,
            m_VertexCount
        );
    }

    glBindVertexArray(0);
}
