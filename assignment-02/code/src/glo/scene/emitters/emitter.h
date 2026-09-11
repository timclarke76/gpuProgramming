/*
 * glo/scene/emitters/emitter.h
 *
 * Emitters create particles at a location based on the SceneNode location.
 * These should be rendered after the rest of the scene has been rendered.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <GL/glew.h>

class SceneNode;

class Emitter
{
  public:
    virtual ~Emitter() {}
    virtual void emit(const GLfloat deltaTime, SceneNode & node) = 0;
    virtual void render() = 0;
};
