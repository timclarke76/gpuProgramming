/*
 * glo/scene/factories/train.cpp
 *
 * © Tim Clarke November 2025
 */

#include "train.h"

#include <glm/glm.hpp>

#include "../animators/wheel.h"
#include "../node.h"
#include "geometric.h"

std::shared_ptr<SceneNode>
TrainFactory::create()
{
    // === TRANFORMATION VALUES ===

    // Sc = Scaling
    // Tr = Transformation

    GLfloat x = 0.0f, y = 0.0f, z = 0.0f;

    glm::vec3 platform1Sc(0.4f, 0.03f, 1.0f);
    glm::vec3 platform1Tr(0.0f, 0.2f, 0.0f);

    glm::vec3 platform2Sc(platform1Sc.x, platform1Sc.y * 2.0f, 1.0f);
    y = platform1Tr.y + ((platform1Sc.y + platform2Sc.y) / 2.0f);
    glm::vec3 platform2Tr(0.0f, y, 0.0f);

    glm::vec3 platform3Sc(platform1Sc.x, platform1Sc.y, 1.0f);
    y = platform2Tr.y + ((platform2Sc.y + platform3Sc.y) / 2.0f);
    glm::vec3 platform3Tr(0.0f, y, 0.0f);

    const glm::vec3 boilerSc(0.4f, 0.65f, 0.4f);
    y = platform3Tr.y + ((platform3Sc.y + boilerSc.z) / 2.0f);
    glm::vec3 boilerTr(0.0f, y, -(1.0f - boilerSc.y) / 2.0f);

    const glm::vec3 chimneySc(0.1f, 0.15f, 0.1f);
    y = boilerTr.y + ((boilerSc.z + chimneySc.y) / 2.0f);
    glm::vec3 chimneyTr(0.0f, y, (chimneySc.z - 0.75f) / 2.0f);

    const glm::vec3 cabSc(platform1Sc.x, 0.6f, platform1Sc.z - boilerSc.y);
    y = platform3Tr.y + ((platform3Sc.y + cabSc.y) / 2.0f);
    z = boilerTr.z + ((boilerSc.y + cabSc.z) / 2.0f);
    const glm::vec3 cabTr(0.0f, y, z);

    const glm::vec3 wheel1Scaling(0.3f, 0.025f, 0.3f);
    x = (platform1Sc.x + wheel1Scaling.y) / 2.0f;
    const glm::vec3 wheel1Translation(x, 0.25f, 0.3f);

    const glm::vec3 wheel2Scaling(0.2f, wheel1Scaling.y, 0.2f);
    glm::vec3 wheel2Translation = wheel1Translation;
    wheel2Translation.y -= 0.05f;
    wheel2Translation.z -= 0.35f;

    const glm::vec3 wheel3Scaling = wheel2Scaling;
    glm::vec3 wheel3Translation = wheel2Translation;
    wheel3Translation.z -= 0.25f;

    const glm::vec3 wheel4Scaling = wheel1Scaling;
    glm::vec3 wheel4Translation = wheel1Translation;
    wheel4Translation.x *= -1.0f;

    const glm::vec3 wheel5Scaling = wheel2Scaling;
    glm::vec3 wheel5Translation = wheel2Translation;
    wheel5Translation.x = wheel4Translation.x;

    const glm::vec3 wheel6Scaling = wheel3Scaling;
    glm::vec3 wheel6Translation = wheel3Translation;
    wheel6Translation.x = wheel5Translation.x;

    // === CONSTRUCTION ===

    auto sceneNode = std::make_shared<SceneNode>();

    auto platform = geometricFactory.getCube("#888");
    platform->setScaling(platform1Sc).setTranslation(platform1Tr);
    sceneNode->addChild(platform);

    platform = geometricFactory.getCube("#44F");
    platform->setScaling(platform2Sc).setTranslation(platform2Tr);
    sceneNode->addChild(platform);

    platform = geometricFactory.getCube("#888");
    platform->setScaling(platform3Sc).setTranslation(platform3Tr);
    sceneNode->addChild(platform);

    auto boiler = geometricFactory.getCylinder("#F00");
    boiler->setScaling(boilerSc).setTranslation(boilerTr);
    boiler->setRotation({0.0f, glm::radians(90.0f), glm::radians(90.0f)});
    sceneNode->addChild(boiler);

    auto chimey = geometricFactory.getCylinder("#F00");
    chimey->setScaling(chimneySc).setTranslation(chimneyTr);
    sceneNode->addChild(chimey);

    auto cab = geometricFactory.getCube("#44F");
    cab->setScaling(cabSc).setTranslation(cabTr);
    sceneNode->addChild(cab);

    auto wheelAnimator = std::make_shared<WheelAnimator>();

    auto wheel = geometricFactory.getCylinder("wheel.png");
    wheel->setScaling(wheel1Scaling).setTranslation(wheel1Translation);
    wheel->setRotation({glm::radians(90.0f), 0.0f, glm::radians(90.0f)});
    wheel->addAnimator(wheelAnimator);
    sceneNode->addChild(wheel);

    wheel = geometricFactory.getCylinder("wheel.png");
    wheel->setScaling(wheel2Scaling).setTranslation(wheel2Translation);
    wheel->setRotation({glm::radians(90.0f), 0.0f, glm::radians(90.0f)});
    wheel->addAnimator(wheelAnimator);
    sceneNode->addChild(wheel);

    wheel = geometricFactory.getCylinder("wheel.png");
    wheel->setScaling(wheel3Scaling).setTranslation(wheel3Translation);
    wheel->setRotation({glm::radians(90.0f), 0.0f, glm::radians(90.0f)});
    wheel->addAnimator(wheelAnimator);
    sceneNode->addChild(wheel);

    wheel = geometricFactory.getCylinder("wheel.png");
    wheel->setScaling(wheel4Scaling).setTranslation(wheel4Translation);
    wheel->setRotation({glm::radians(90.0f), 0.0f, glm::radians(90.0f)});
    wheel->addAnimator(wheelAnimator);
    sceneNode->addChild(wheel);

    wheel = geometricFactory.getCylinder("wheel.png");
    wheel->setScaling(wheel5Scaling).setTranslation(wheel5Translation);
    wheel->setRotation({glm::radians(90.0f), 0.0f, glm::radians(90.0f)});
    wheel->addAnimator(wheelAnimator);
    sceneNode->addChild(wheel);

    wheel = geometricFactory.getCylinder("wheel.png");
    wheel->setScaling(wheel6Scaling).setTranslation(wheel6Translation);
    wheel->setRotation({glm::radians(90.0f), 0.0f, glm::radians(90.0f)});
    wheel->addAnimator(wheelAnimator);
    sceneNode->addChild(wheel);

    return sceneNode;
}
