/*
 * glo/projection.cpp
 *
 * © Tim Clarke November 2025
 */

#include <cassert>

#include "glm.h"

#include "projection.h"

std::ostream &
operator<<(std::ostream & os, const Projection & obj)
{
    // clang-format off
    os << "{"
       <<     "\"_type\":\"Projection\","
       <<     "\"width\":\"" << obj.width << "\","
       <<     "\"height\":\"" << obj.height << "\","
       <<     "\"aspect\":\"" << obj.aspect << "\","
       <<     "\"fov\":\"" << obj.fov << "\","
       <<     "\"nearPlane\":\"" << obj.nearPlane << "\","
       <<     "\"farPlane\":\"" << obj.farPlane << "\","
       <<     "\"perspective\":" << obj.perspective
       << "}";
    // clang-format on

    return os;
}

/*
 * Initialise all of our member variables, and ensure they are "sensible".
 * The perspective is calculated immediately (a dirty flag is not used) using
 * the glm::perspective function.
 */
Projection &
Projection::initialise(const GLint width, const GLint height, const GLfloat fov,
    const GLfloat nearPlane, const GLfloat farPlane)
{
    assert(width > 0);
    assert(height > 0);
    assert(fov > 0.0f);
    assert(fov < 180.0f);
    assert(nearPlane > 0.0f);
    assert(farPlane > nearPlane); 

    this->width = width;
    this->height = height;
    this->fov = fov;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;

    aspect = GLfloat(this->width) / GLfloat(this->height);
    perspective =
        glm::perspective(this->fov, aspect, this->nearPlane, this->farPlane);

    return *this;
}
