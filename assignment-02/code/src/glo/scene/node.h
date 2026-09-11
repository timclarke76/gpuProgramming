/*
 * glo/scene/node.h
 *
 * The SceneNode class can be used to create a graph of Renderable objects. Each
 * node may optionally have a Renderable, and zero or more child SceneNodes. An
 * empty node is valid, yet pointless. Using a graph provides easy management of
 * mesh construction, storage, and destruction. And provides efficient
 * transformation and animation.
 *
 * Primarily, the graph allows a transformation in a parent node, to be
 * reflected in all descen- dant nodes, without setting each separately —--
 * translating, scaling, and/or rotating a parent, will provide a transformation
 * matrix for each descendant with the same transformations. The transformations
 * are one way, from parent to child, allowing further transformations in the
 * children without affecting parents, cousins, etc.
 *
 * Methods are provided to allow relative and absolute transformations (e.g.
 * rotate and setRotation), and access to each transformation (e.g.
 * getRotation).
 *
 * Each SceneNode may have zero or more animations. These animations are updated
 * after every call to animate. An animation should update the SceneNode’s
 * transformation according to its purpose, taking advantage of the
 * transformation mechanics described above.
 *
 * By using shared pointers, the SceneNode graph also allows an easy way to
 * store all scene objects. The client may choose to keep its own shared pointer
 * of a node for later access, or access it from the graph using the getChildren
 * accessor.
 *
 * Rendering a SceneNode does not render the children. Graph traversal should be
 * done manually, to allow shader uniform values to be updated appropriately. A
 * call to updateTransformation should be made once, to the top parent node,
 * before the rendering loop begins. This allows any nodes marked as dirty to
 * update their transformation matrix just once.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <memory>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "animators/animator.h"
#include "emitters/emitter.h"
#include "renderable.h"

class SceneNode
{
    static constexpr auto MAX_ROTATION = 2.0f * glm::pi<float>();

  public:
    SceneNode() {}
    SceneNode(std::shared_ptr<Renderable> renderable)
        { renderables.push_back(renderable); }

    // === PARENTAGE ===
    SceneNode & addChild(std::shared_ptr<SceneNode> child);
    // clang-format off
    const std::vector<std::shared_ptr<SceneNode>> & getChildren() const
        { return children; }
    // clang-format on

    //=== TRANSLATION ===
    SceneNode & setTranslation(const glm::vec3 & translation);
    SceneNode & translate(const glm::vec3 & translation);
    const glm::vec3 & getTranslation() const { return translation; }

    //=== ROTATION ===
    SceneNode & setRotation(const glm::vec3 & rotation);
    SceneNode & rotate(const glm::vec3 & rotation);
    const glm::vec3 & getRotation() const { return rotation; }

    // === SCALING ===
    SceneNode & setScaling(const glm::vec3 & scaling);
    SceneNode & scale(const glm::vec3 & scaling);
    const glm::vec3 & getScaling() const { return scaling; }

    // clang-format off
    SceneNode & setScaling(const GLfloat scaling)
        { return setScaling(glm::vec3(scaling)); }
    SceneNode & scale(const GLfloat scaling)
        { return scale(glm::vec3(scaling)); }
    // clang-format on

    // === ANIMATORS ===
    // clang-format off
    SceneNode & addAnimator(std::shared_ptr<Animator> animator)
        { animators.push_back(animator); return *this; }
    const std::vector<std::shared_ptr<Animator>> & getAnimators() const
        { return animators; }
    // clang-format on

    // === EMMITTERS ===
    // clang-format off
    SceneNode & addEmitter(std::shared_ptr<Emitter> emmitter)
        { emmitters.push_back(emmitter); return *this; }
    const std::vector<std::shared_ptr<Emitter>> & getEmitters() const
        { return emmitters; }
    // clang-format on

    // === ACCESSORS ===
    const glm::mat4 & getWorldMatrix() const { return worldMatrix; }
    glm::vec3 getWorldPosition() const { return glm::vec3(worldMatrix[3]); }

    // clang-format off
    std::vector<std::shared_ptr<Renderable>> getRenderables() const
        { return renderables; }
    void addRenderable(std::shared_ptr<Renderable> renderable)
        { renderables.push_back(renderable); }
    void setRenderable(std::shared_ptr<Renderable> renderable)
        { renderables.clear(); renderables.push_back(renderable); }
    //clang-format on

    // === RENDERING ===
    SceneNode & animate(const GLfloat delta);
    SceneNode & emit(const GLfloat delta);
    SceneNode & updateTransformation();
    SceneNode & render() { for (auto r : renderables ) r->render();
        return *this; }
    SceneNode & renderParticles() { for (auto e : emmitters ) e->render();
        return *this; }

  private:
    /*
     * When a change is made to any transformation, we don't update the matrix
     * immediately, but instead mark the node as "dirty".  It is then the
     * client's responsibility to call updateTransformation, which will update
     * and save the matrix, and mark the node as clean again.
     */
    void markDirty();

    void setParent(SceneNode * newParent);

    SceneNode * parent = nullptr;
    std::vector<std::shared_ptr<SceneNode>> children;
    std::vector<std::shared_ptr<Renderable>> renderables;
    std::vector<std::shared_ptr<Animator>> animators;
    std::vector<std::shared_ptr<Emitter>> emmitters;

    glm::vec3 translation{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 scaling{1.0f};
    glm::mat4 worldMatrix{1.0f};
    bool isDirty = true;
};
