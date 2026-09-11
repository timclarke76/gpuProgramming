/*
 * glo/scene/animators/rocking.cpp
 *
 * © Tim Clarke November 2025
 */

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "rocking.h"

#include "../node.h"

void
RockingAnimator::animate(const GLfloat deltaTime, SceneNode & node)
{
    accumulatedTime += deltaTime;

    const auto targetAngle = reverse ? -MAX_ANGLE : MAX_ANGLE;
    const auto progress = accumulatedTime / MAX_TIME;
    const auto currentAngle = targetAngle * sin(progress * glm::pi<GLfloat>());

    node.setRotation(glm::vec3(currentAngle, 0.0f, 0.0f));

    if (accumulatedTime >= MAX_TIME)
    {
        reverse = !reverse;
        accumulatedTime = 0.0f;
    }
}
