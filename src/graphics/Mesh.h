#pragma once

#include "Vertex.h"

class Mesh
{
public:
    Mesh();
    ~Mesh();

    bool Create(const std::vector<Vertex>& vertices);

    void Draw() const;

private:
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;

    unsigned int m_VertexCount = 0;
};