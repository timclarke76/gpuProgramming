/*
 * glo/scene/factories/cot_mobile.cpp
 *
 * © Tim Clarke November 2025
 */

#include "cot_mobile.h"

#include <glm/glm.hpp>

#include "../animators/mobile.h"
#include "../animators/ornament.h"
#include "../node.h"
#include "geometric.h"
#include "train.h"

std::shared_ptr<SceneNode>
CotMobileFactory::create()
{
    /*
     * Shared pointers are automatically deleted when the sceneNode goes out of
     * scope. A seperate animator is provided for each node, as the animators
     * are stateful with randomised data (time, direction, etc).
     */

    glm::vec3 scaling{ORNAMENT_SIZE}, translation;
    glm::vec3 threadScaling{0.005f, THREAD_LENGTH, 0.005f};
    glm::vec3 threadTranslation{0.0f, 2.5f, 0.0f};

    auto sceneNode = std::make_shared<SceneNode>();
    sceneNode->addAnimator(std::make_shared<MobileAnimator>());

    scaling.z = scaling.x / 5.0f;
    translation.x = -RADIUS;
    translation.y = 0.0f;
    translation.z = RADIUS;
    auto star = geometricFactory.getStar("#FFDF00");
    star->addAnimator(std::make_shared<OrnamentAnimator>())
        .setScaling(scaling)
        .setTranslation(translation);
    sceneNode->addChild(star);

    auto decal = geometricFactory.getDecal("sun.png");
    decal->scale({0.4f, 0.4f, 1.05f}).translate({0.0f, 0.0f, 0.02f});
    star->addChild(decal);

    auto thread = geometricFactory.getCylinder("#FFF3");
    thread->setScaling(threadScaling).setTranslation(threadTranslation);
    star->addChild(thread);

    scaling = glm::vec3(ORNAMENT_SIZE);
    scaling.z = ORNAMENT_SIZE / 3.0f;
    translation.x = RADIUS;
    auto animals = geometricFactory.getSphere("animals.png");
    animals->addAnimator(std::make_shared<OrnamentAnimator>())
        .setScaling(scaling)
        .setTranslation(translation);
    sceneNode->addChild(animals);

    thread = geometricFactory.getCylinder("#FFF3");
    thread->setScaling(threadScaling).setTranslation(threadTranslation);
    animals->addChild(thread);

    scaling = glm::vec3(ORNAMENT_SIZE);
    translation.x = -RADIUS;
    translation.z = -RADIUS;
    auto cube = geometricFactory.getCube("letters-cube.png");
    cube->addAnimator(std::make_shared<OrnamentAnimator>())
        .setScaling(scaling)
        .setTranslation(translation);
    sceneNode->addChild(cube);

    thread = geometricFactory.getCylinder("#FFF3");
    threadScaling = {0.005f, THREAD_LENGTH - 0.25f, 0.005f};
    threadTranslation = {0.0f, 2.75f, 0.0f};
    thread->setScaling(threadScaling).setTranslation(threadTranslation);
    cube->addChild(thread);

    scaling = glm::vec3(ORNAMENT_SIZE);
    translation.x = RADIUS - scaling.x;
    translation.z = -RADIUS + scaling.z;
    auto train = TrainFactory(geometricFactory).create();
    train->addAnimator(std::make_shared<OrnamentAnimator>())
        .setScaling(scaling)
        .setTranslation(translation);
    sceneNode->addChild(train);

    thread = geometricFactory.getCylinder("#FFF3");
    thread->setScaling(threadScaling).setTranslation(threadTranslation);
    train->addChild(thread);

    auto crossbar = geometricFactory.getCube("#FFF");
    crossbar->setScaling({5.0f, 0.1f, 0.1f})
        .setTranslation({0.0f, 2.5f, 0.0f})
        .rotate({0.0f, glm::radians(45.0f), 0.0f});
    sceneNode->addChild(crossbar);

    // The second crossbar is slightly smaller to avoid depth fighting with the
    // first.
    crossbar = geometricFactory.getCube("#FFF");
    crossbar->setScaling({5.0f, 0.09f, 0.09f})
        .setTranslation({0.0f, 2.5f, 0.0f})
        .rotate({0.0f, -glm::radians(45.0f), 0.0f});
    sceneNode->addChild(crossbar);

    return sceneNode;
}
