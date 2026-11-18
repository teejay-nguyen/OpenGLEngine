#include "OrbitCamera.h"

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <cmath>

namespace
{
    // Stop just short of straight up/down: at exactly +-90 degrees the view
    // direction is parallel to the world up vector and lookAt() breaks down.
    constexpr float kMaxPitch = 89.0f;

    const glm::vec3 kWorldUp { 0.0f, 1.0f, 0.0f };
}

OrbitCamera::OrbitCamera() = default;

void OrbitCamera::Orbit(float deltaX, float deltaY)
{
    // dragging right spins the scene right (camera moves left around it)
    m_Yaw -= deltaX * m_OrbitSensitivity;

    // dragging down tilts the camera to look from above
    m_Pitch += deltaY * m_OrbitSensitivity;
    m_Pitch = std::clamp(m_Pitch, -kMaxPitch, kMaxPitch);

    // keep yaw in [0, 360) so it never grows without bound
    m_Yaw = std::fmod(m_Yaw, 360.0f);
    if (m_Yaw < 0.0f)
    {
        m_Yaw += 360.0f;
    }
}

void OrbitCamera::Pan(float deltaX, float deltaY, float viewportHeight)
{
    if (viewportHeight <= 0.0f)
    {
        return;
    }

    // Height of the visible region at the target's depth, in world units.
    // Converting pixels -> world units with it makes the point under the
    // cursor stay under the cursor, at any zoom level.
    const float visibleHeight = 2.0f * m_Distance * std::tan(glm::radians(m_FieldOfView) * 0.5f);
    const float worldPerPixel = visibleHeight / viewportHeight;

    // cursor right -> scene moves right -> target moves left (screen +y is down)
    m_Target += (-deltaX * GetRight() + deltaY * GetUp()) * worldPerPixel;
}

void OrbitCamera::Zoom(float scrollSteps)
{
    // Multiplicative zoom: each step covers the same *fraction* of the distance,
    // so it feels identical whether the model is 1 mm or 1 km away.
    m_Distance *= std::pow(m_ZoomFactor, scrollSteps);
    m_Distance = std::clamp(m_Distance, m_MinDistance, m_MaxDistance);
}

void OrbitCamera::Reset()
{
    *this = OrbitCamera();
}

void OrbitCamera::SetTarget(const glm::vec3& target, float distance)
{
    m_Target = target;
    m_Distance = std::clamp(distance, m_MinDistance, m_MaxDistance);
}

glm::vec3 OrbitCamera::GetPosition() const
{
    // spherical -> Cartesian, offset from the target
    const float yaw = glm::radians(m_Yaw);
    const float pitch = glm::radians(m_Pitch);

    const glm::vec3 offset {
        std::cos(pitch) * std::sin(yaw),
        std::sin(pitch),
        std::cos(pitch) * std::cos(yaw)
    };

    return m_Target + offset * m_Distance;
}

glm::mat4 OrbitCamera::GetViewMatrix() const
{
    return glm::lookAt(GetPosition(), m_Target, kWorldUp);
}

glm::mat4 OrbitCamera::GetProjectionMatrix(float aspectRatio) const
{
    return glm::perspective(glm::radians(m_FieldOfView), aspectRatio, m_NearPlane, m_FarPlane);
}

glm::vec3 OrbitCamera::GetForward() const
{
    return glm::normalize(m_Target - GetPosition());
}

glm::vec3 OrbitCamera::GetRight() const
{
    return glm::normalize(glm::cross(GetForward(), kWorldUp));
}

glm::vec3 OrbitCamera::GetUp() const
{
    return glm::cross(GetRight(), GetForward());
}
