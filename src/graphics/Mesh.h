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

    // A Mesh owns GPU handles: copying would double-delete them, so it is move-only.
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    bool Create(
        const std::vector<Vertex>& vertices,
        PrimitiveType primitiveType = PrimitiveType::Triangles
    );

    void Draw() const;

private:
    void Release();

private:
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;

    unsigned int m_VertexCount = 0;

    PrimitiveType m_PrimitiveType = PrimitiveType::Triangles;
};
