#pragma once

#include <glm/glm.hpp>

// Position / rotation / scale of an object in world space.
// Produces the "model" matrix that takes vertices from object space to world space.
struct Transform
{
    glm::vec3 position { 0.0f };

    // Euler angles in degrees: pitch (X), yaw (Y), roll (Z).
    // Human-friendly for now (and for a future ImGui panel); converted to a
    // quaternion when building the matrix.
    glm::vec3 rotation { 0.0f };

    glm::vec3 scale { 1.0f };

    // Model = Translate * Rotate * Scale (applied to vertices right-to-left:
    // scale first, then rotate, then translate).
    glm::mat4 GetMatrix() const;
};
