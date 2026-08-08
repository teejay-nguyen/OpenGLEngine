#pragma once

#include "Vertex.h"

class Mesh
{
public:
    Mesh();
    ~Mesh();

    bool CreateTriangle();

    void Draw() const;

private:
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
};