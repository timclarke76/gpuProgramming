/*
 * glo/scene/animators/ornament.cpp
 *
 * © Tim Clarke November 2025
 */

#include "ornament.h"

#include <random>

#include "../node.h"

void
OrnamentAnimator::animate(const GLfloat deltaTime, SceneNode & node)
{
    if (time < deltaTime)
    {
        // start a new spin

        const auto direction = speed < 0.0f ? 1.0f : -1.0f; // reverse direction
        speed = rand(MAX_SPEED) * direction;
        totalTime = time = rand(10.0f) + 10.0f;
    }

    GLfloat y;

    if (time > (totalTime - EASING)) y = speed * ((totalTime - time) / EASING);
    else if (time < EASING) y = speed * (time / EASING);
    else y = speed;

    node.rotate(glm::vec3(0.0f, y, 0.0f) * deltaTime);
    time -= deltaTime;
}

GLfloat
OrnamentAnimator::rand(const GLfloat range)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<GLfloat> dis(0.0f, range);
    return dis(gen);
}
