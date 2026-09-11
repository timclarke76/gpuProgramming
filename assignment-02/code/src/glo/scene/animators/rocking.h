/*
 * glo/scene/animators/ornament.h
 *
 * The rocking animator gently rocks a cradle back and forth.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <GL/glew.h>

#include "animator.h"

class SceneNode;

class RockingAnimator : public Animator
{
  private:
    static constexpr auto MAX_ANGLE = 0.05f;
    static constexpr auto ROCK_SPEED = 0.02f;
    static constexpr auto MAX_TIME = MAX_ANGLE / ROCK_SPEED;

  public:
    void animate(const GLfloat deltaTime, SceneNode & node);

  private:
    GLfloat accumulatedTime = MAX_TIME / 2.0f;
    bool reverse = false;
};
