#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

glm::mat4 Transform::GetMatrix() const
{
    const glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
    const glm::mat4 rotationMatrix = glm::mat4_cast(glm::quat(glm::radians(rotation)));
    const glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

    return translation * rotationMatrix * scaleMatrix;
}
