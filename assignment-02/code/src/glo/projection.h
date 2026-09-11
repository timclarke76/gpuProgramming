/*
 * glo/projection.h
 *
 * Records the screen aspect, the camera's FOV, and the near and far planes.
 * These are used to calculate the perspective for shader vertex calculations.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Projection
{
    // sensible near and far plane defaults for our requirements
    static constexpr GLfloat DEFAULT_NEAR = 0.1f;
    static constexpr GLfloat DEFAULT_FAR = 100.0f;

    // override the ostream operator for debug messages
    friend std::ostream & operator<<(std::ostream & os, const Projection & obj);

  public:
    Projection(const GLint width, const GLint height, const GLfloat fov,
        const GLfloat nearPlane = DEFAULT_NEAR,
        const GLfloat farPlane = DEFAULT_FAR)
    {
        initialise(width, height, fov, nearPlane, farPlane);
    }

    Projection(const Projection &) = default;
    Projection & operator=(const Projection &) = default;

    Projection & initialise(const GLint width, const GLint height,
        const GLfloat fov, const GLfloat nearPlane = DEFAULT_NEAR,
        const GLfloat farPlane = DEFAULT_FAR);

    // Set the viewport width and height. Typically called when a new window is
    // created, or the window's size is changed.
    Projection & setViewport(const GLint width, const GLint height)
    {
        if ((width != this->width) || (height != this->height))
        {
            initialise(width, height, fov);
        }

        return *this;
    }

    // Typically, the camera's FOV will be set once, and not changed. May though
    // be required for lens zooming simulation.
    Projection & setFov(const GLfloat fov)
    {
        if (fov == this->fov) return *this;
        return initialise(width, height, fov, nearPlane, farPlane);
    }

    // Acessors for the viewport's width, height, and aspect
    GLint getWidth() const { return width; }
    GLint getHeight() const { return height; }
    GLfloat getAspect() const { return aspect; }

    // Accessors for the camera's FOV, and the scene's near and far planes
    GLfloat getFov() const { return fov; }
    GLfloat getNearPlane() const { return nearPlane; }
    GLfloat getFarPlane() const { return farPlane; }

    // Accessor for the calculated perspective, for use in the shaders
    glm::mat4 getPerspective() const { return perspective; }

  private:
    // viewport's width, height, and aspect
    GLint width;
    GLint height;
    GLfloat aspect;

    // camera's FOV
    GLfloat fov;

    // scene's rendered near and far planes
    GLfloat nearPlane;
    GLfloat farPlane;

    // the calculated perspective
    glm::mat4 perspective;
};
