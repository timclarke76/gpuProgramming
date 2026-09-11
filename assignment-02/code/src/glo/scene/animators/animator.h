/*
 * glo/scene/animators/animator.h
 *
 * Animators transform the given SceneNode in some way when a call to animate is
 * made.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <GL/glew.h>

class SceneNode;

class Animator
{
  public:
    virtual void animate(const GLfloat deltaTime, SceneNode & node) = 0;
};
