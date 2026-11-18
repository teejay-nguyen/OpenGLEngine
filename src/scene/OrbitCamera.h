#pragma once

#include <glm/glm.hpp>

// A camera that orbits a target point, like the viewport in Blender or a CAD viewer.
//
// Its state is spherical coordinates around the target:
//   - distance: radius of the sphere
//   - yaw:      angle around the world Y axis (degrees)
//   - pitch:    angle above/below the horizon (degrees)
// The eye position is derived from these each frame, so the camera always
// looks at the target and can never roll.
class OrbitCamera
{
public:
    OrbitCamera();

    // Rotate around the target. Inputs are mouse movement in pixels.
    void Orbit(float deltaX, float deltaY);

    // Slide the target (and camera) parallel to the view plane, so the scene
    // follows the cursor. viewportHeight is in the same units as the deltas.
    void Pan(float deltaX, float deltaY, float viewportHeight);

    // Move toward/away from the target. Positive = zoom in.
    void Zoom(float scrollSteps);

    // Back to the default view.
    void Reset();

    // Point the camera at a new target from a given distance (used later for auto-framing).
    void SetTarget(const glm::vec3& target, float distance);

    glm::vec3 GetPosition() const;
    const glm::vec3& GetTarget() const { return m_Target; }
    float GetDistance() const { return m_Distance; }

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix(float aspectRatio) const;

private:
    // camera basis vectors in world space
    glm::vec3 GetForward() const;
    glm::vec3 GetRight() const;
    glm::vec3 GetUp() const;

private:
    glm::vec3 m_Target { 0.0f };
    float m_Distance = 5.0f;
    float m_Yaw = 30.0f;
    float m_Pitch = 25.0f;

    float m_FieldOfView = 45.0f; // vertical, degrees
    float m_NearPlane = 0.01f;
    float m_FarPlane = 1000.0f;

    // tuning
    float m_OrbitSensitivity = 0.3f; // degrees per pixel
    float m_ZoomFactor = 0.9f;       // distance multiplier per scroll step
    float m_MinDistance = 0.05f;
    float m_MaxDistance = 500.0f;
};
