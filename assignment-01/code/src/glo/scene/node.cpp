/*
 * glo/scene/node.cpp
 *
 * © Tim Clarke November 2025
 */

#include "node.h"

#include <algorithm>
#include <cassert>

#include <glm/gtc/matrix_transform.hpp>

// === PARENTAGE ===

/*
 * The child must be non-null, not be this node, and not already have a parent.
 * TODO: allow children to be moved from one parent to another.
 */
SceneNode &
SceneNode::addChild(std::shared_ptr<SceneNode> child)
{
    assert(child);
    assert(child.get() != this);
    assert(child->parent == nullptr);

    children.push_back(child);
    child->setParent(this);
    child->markDirty();

    return *this;
}

// === TRANSLATION ===

SceneNode &
SceneNode::setTranslation(const glm::vec3 & translation)
{
    this->translation = translation;
    markDirty();
    return *this;
}

SceneNode &
SceneNode::translate(const glm::vec3 & translation)
{
    this->translation += translation;
    markDirty();
    return *this;
}

// === ROTATION ===

SceneNode &
SceneNode::setRotation(const glm::vec3 & rotation)
{
    this->rotation = rotation;
    markDirty();
    return *this;
}

SceneNode &
SceneNode::rotate(const glm::vec3 & rotation)
{
    this->rotation += rotation;

    // don't let our rotation values get too high and wrap
    this->rotation.x = glm::mod(this->rotation.x, MAX_ROTATION);
    if (this->rotation.x < 0) this->rotation.x += MAX_ROTATION;

    this->rotation.y = glm::mod(this->rotation.y, MAX_ROTATION);
    if (this->rotation.y < 0) this->rotation.y += MAX_ROTATION;

    this->rotation.z = glm::mod(this->rotation.z, MAX_ROTATION);
    if (this->rotation.z < 0) this->rotation.z += MAX_ROTATION;

    markDirty();

    return *this;
}

// === SCALING ===

SceneNode &
SceneNode::setScaling(const glm::vec3 & scaling)
{
    assert(scaling.x != 0.0f);
    assert(scaling.y != 0.0f);
    assert(scaling.z != 0.0f);

    this->scaling = scaling;
    markDirty();
    return *this;
}

SceneNode &
SceneNode::scale(const glm::vec3 & scaling)
{
    assert(scaling.x != 0.0f);
    assert(scaling.y != 0.0f);
    assert(scaling.z != 0.0f);

    this->scaling *= scaling;
    markDirty();
    return *this;
}

// === RENDERING ===

SceneNode &
SceneNode::animate(const GLfloat deltaTime)
{
    for (auto & a : animators) a->animate(deltaTime, *this);
    for (auto & c : children) c->animate(deltaTime);
    return *this;
}

SceneNode &
SceneNode::updateTransformation()
{
    if (isDirty)
    {
        glm::mat4 localMatrix{1.0f};

        localMatrix = glm::translate(localMatrix, translation);
        localMatrix = glm::rotate(localMatrix, rotation.x, {1.0, 0.0, 0.0});
        localMatrix = glm::rotate(localMatrix, rotation.y, {0.0, 1.0, 0.0});
        localMatrix = glm::rotate(localMatrix, rotation.z, {0.0, 0.0, 1.0});
        localMatrix = glm::scale(localMatrix, scaling);

        if (parent != nullptr)
        {
            worldMatrix = parent->worldMatrix * localMatrix;
        }
        else
        {
            worldMatrix = localMatrix;
        }

        isDirty = false;
    }

    for (auto & child : children) child->updateTransformation();
    return *this;
}

void
SceneNode::setParent(SceneNode * newParent)
{
    parent = newParent;
    markDirty();
}

void
SceneNode::markDirty()
{
    isDirty = true;

    // When this node requires to update its matrix, then so do all the children
    // to ensure their worldMatrix is correct.
    for (auto & child : children) child->markDirty();
}
