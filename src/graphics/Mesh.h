#pragma once

#include "Vertex.h"

#include <vector>

enum class PrimitiveType
{
    Points,
    Lines,
    Triangles
};

class Mesh
{
public:
    Mesh();
    ~Mesh();

    bool Create(
        const std::vector<Vertex>& vertices,
        PrimitiveType primitiveType = PrimitiveType::Triangles
    );

    void Draw() const;

private:
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;

    unsigned int m_VertexCount = 0;

    PrimitiveType m_PrimitiveType = PrimitiveType::Triangles;
};