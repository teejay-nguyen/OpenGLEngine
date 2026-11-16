#pragma once

#include "Vertex.h"

#include <cstdint>
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

    // Non-indexed: every group of vertices forms a primitive (glDrawArrays).
    bool Create(
        const std::vector<Vertex>& vertices,
        PrimitiveType primitiveType = PrimitiveType::Triangles
    );

    // Indexed: primitives are assembled from vertices referenced by index (glDrawElements).
    bool Create(
        const std::vector<Vertex>& vertices,
        const std::vector<std::uint32_t>& indices,
        PrimitiveType primitiveType = PrimitiveType::Triangles
    );

    void Draw() const;

    bool IsIndexed() const { return m_IndexCount > 0; }

private:
    void Release();

private:
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    unsigned int m_EBO = 0;

    unsigned int m_VertexCount = 0;
    unsigned int m_IndexCount = 0;

    PrimitiveType m_PrimitiveType = PrimitiveType::Triangles;
};
