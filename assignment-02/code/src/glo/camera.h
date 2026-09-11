/*
 * glo/camera.h
 *
 * Responsible for the viewer's position --- the "camera". Provides methods for
 * both translation and rotation.
 *
 * TODO: make interface consistent with SceneNode.
 * TODO: allow multiple cameras, to provide for PiP, screen images, etc.
 * TODO: like SceneNode, the class should have an isDirty boolean, and only
 *       update the matrix when necessary
 *
 * CITE: I asked DeepSeek (DeepSeek 2025) to generate code for the lookAt
 * method, which changes the camera’s yaw and pitch according to the direction
 * of the target location. Apart from modifying the code to fit into the Camera
 * class, and adding a clamp, no other modification was required.
 *
 * DeepSeek (2025). DeepSeek-V3.2-Exp. url: https://chat.deepseek.com
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
    static constexpr auto DEFAULT_POSITION = glm::vec3(0.0f);
    static constexpr auto DEFAULT_TARGET = glm::vec3(0.0f, 0.0f, -1.0f);

    friend std::ostream & operator<<(std::ostream & os, const Camera & obj);

  public:
    static Camera & instance()
    {
        static Camera instance;
        return instance;
    }

    Camera & initialise(const glm::vec3 & position = DEFAULT_POSITION,
        const glm::vec3 & target = DEFAULT_TARGET)
    {
        return setPosition(position).lookAt(target);
    }

    Camera & setPosition(const GLfloat x, const GLfloat y, const GLfloat z)
    {
        return setPosition({x, y, z});
    }

    Camera & setPosition(const glm::vec3 & position)
    {
        this->position = position;
        return *this;
    }

    Camera & translate(const GLfloat x, const GLfloat y, const GLfloat z);

    Camera & translate(const glm::vec3 & amount)
    {
        return translate(amount.x, amount.y, amount.z);
    }

    glm::vec3 getPosition() const { return position; }
    glm::vec3 getFront() const { return front; }

    Camera & rotate(const glm::vec2 & offset)
    {
        return rotate(offset.x, offset.y);
    }

    Camera & rotate(const GLfloat xOffset, GLfloat yOffset);

    Camera & lookAt(const GLfloat x, const GLfloat y, const GLfloat z)
    {
        return lookAt({x, y, z});
    }

    // Turns the camera to look at the supplied target position.
    Camera & lookAt(const glm::vec3 & target);

    glm::mat4 getViewMatrix() const
        { return glm::lookAt(position, (position + front), up); }

  private:
    Camera() { initialise(); }

    // copies not allowed
    Camera(const Camera &) = delete;
    Camera & operator=(const Camera &) = delete;
    Camera(Camera &&) = delete;
    Camera & operator=(Camera &&) = delete;

    void updateVectors();

    glm::vec3 position;
    const glm::vec3 up{0.0f, 1.0f, 0.0f};
    glm::vec3 front;
    glm::vec3 right;

    GLfloat yaw;
    GLfloat pitch;
};
