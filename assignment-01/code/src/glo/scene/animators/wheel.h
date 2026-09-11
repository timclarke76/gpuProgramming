/*
 * glo/scene/animators/wheel.h
 *
 * Rotate a train wheel at a constant speed.
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

class WheelAnimator : public Animator
{
    static constexpr GLfloat SPEED = -1.0f;

  public:
    virtual void animate(const GLfloat deltaTime, SceneNode & node)
        { node.rotate(glm::vec3(SPEED, 0.0f, 0.0f) * deltaTime); }
};
