/*
 * glo/scene/animators/mobile.h
 *
 * Rotate the mobile at a constant speed on the y axis.
 *
 * TODO: make this animator more reusable, by allowing the speed and rotation
 * axis to be specified by the client on construction.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../node.h"
#include "animator.h"

class MobileAnimator : public Animator
{
    static constexpr GLfloat SPEED = 0.25f;

  public:
    // clang-format off
    MobileAnimator & togglePause() { isPaused = !isPaused; return *this; }
    // clang-format on

    virtual void animate(const GLfloat deltaTime, SceneNode & node)
    {
        if (!isPaused) node.rotate(glm::vec3(0.0f, SPEED, 0.0f) * deltaTime);
    }

  private:
    bool isPaused = false;
};
