/*
 * glo/camera.cpp
 *
 * © Tim Clarke November 2025
 */

#include <cassert>
#include <cmath>

#include "glm.h"

#include "camera.h"

std::ostream &
operator<<(std::ostream & os, const Camera & obj)
{
    // clang-format off
    os << "{"
       <<     "\"_type\":\"Camera\","
       <<     "\"position\":" << obj.position << ","
       <<     "\"up\":" << obj.up << ","
       <<     "\"front\":" << obj.front << ","
       <<     "\"right\":" << obj.right << ","
       <<     "\"yaw\":" << obj.yaw << ","
       <<     "\"pitch\":" << obj.pitch
       << "}";
    // clang-format on

    return os;
}

Camera &
Camera::translate(const GLfloat x, const GLfloat y, const GLfloat z)
{
    position += x * right;
    position += y * up;
    position -= z * front;

    return *this;
}

Camera &
Camera::rotate(const GLfloat xOffset, const GLfloat yOffset)
{
    yaw = std::fmod(yaw + xOffset, 360.0f);
    pitch = glm::clamp((pitch + yOffset), -89.0f, 89.0f);
    updateVectors();

    return *this;
}

Camera &
Camera::lookAt(const glm::vec3 & target)
{
    assert(target != position);

    const auto direction = glm::normalize(target - position);

    yaw = glm::degrees(atan2(direction.z, direction.x));

    pitch = glm::degrees(asin(glm::clamp(direction.y, -1.0f, 1.0f)));
    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    updateVectors();

    return *this;
}

void
Camera::updateVectors()
{
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, up));
}
