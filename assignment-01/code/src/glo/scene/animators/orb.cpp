/*
 * glo/scene/animators/orb.cpp
 *
 * © Tim Clarke November 2025
 */

#include "orb.h"

#include <random>

#include "../node.h"

void
OrbAnimator::animate(const GLfloat deltaTime, SceneNode & node)
{
    if (elapsedTime >= totalTime)
    {
        startPosition = node.getTranslation();

        targetPosition = glm::vec3();
        targetPosition.x = rand(-X_RADIUS, X_RADIUS);
        targetPosition.y = rand(MIN_Y, MAX_Y);
        targetPosition.z = rand(-Z_RADIUS, Z_RADIUS);

        // CITE: DeekSeek
        const glm::vec3 midpoint = (startPosition + targetPosition) * 0.5f;
        bezierControlPosition = midpoint + glm::vec3(0, rand(1.0f, 2.0f), 0);

        totalTime = rand(MIN_TIME, MAX_TIME);
        elapsedTime = 0.0f;
    }

    // Quadratic Bezier curve calculation
    // CITE: DeekSeek
    GLfloat time = elapsedTime / totalTime;
    time = time * time * (CUBIC_COEFFICIENT - QUADRATIC_FACTOR * time);
    // clang-format off
    const glm::vec3 position = (1 - time) * (1 - time) * startPosition
        + 2 * (1 - time) * time * bezierControlPosition
        + time * time * targetPosition;
    // clang-format on

    node.setTranslation(position);
    elapsedTime += deltaTime;
}

GLfloat
OrbAnimator::rand(const GLfloat min, const GLfloat max)
{
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<GLfloat> dis(min, max);
    return dis(gen);
}
