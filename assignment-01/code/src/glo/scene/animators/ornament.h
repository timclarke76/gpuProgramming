/*
 * glo/scene/animators/ornament.h
 *
 * The ornament animator rotates a SceneNode on the y axis only, for a random
 * length of time between ten and twenty seconds. It then rotates again in the
 * opposite direction, and so on. To avoid abrupt changes in direction, an
 * easing of 2.5 seconds is used to slow down and speed up the rotation.
 *
 * The speed is randomised between -1.0 and 1.0 (effectively 0.0 to 1.0, in a
 * random direction) --- providing a "natural" spin, without it looking
 * mechanical in nature.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "animator.h"

class OrnamentAnimator : public Animator
{
  private:
    static constexpr GLfloat EASING = 2.5f;
    static constexpr GLfloat MAX_SPEED = 0.5f;

  public:
    // Executes the animation transformation.
    virtual void animate(const GLfloat deltaTime, SceneNode & node) override;

  private:
    // Returns a random number between 0.0 and range
    static GLfloat rand(const GLfloat range);

    GLfloat speed = (rand(2.0f) - 1.0f); // randomise start direction
    GLfloat totalTime = 0.0f;
    GLfloat time = 0.0f;
};
