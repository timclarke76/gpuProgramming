/*
 * scene/graphs/orbs.cpp
 *
 * © Tim Clarke November 2025
 */

#include "orbs.h"

#include <random>

#include "../../glo/camera.h"
#include "../../glo/program/program.h"
#include "../../glo/scene/animators/orb.h"
#include "../../glo/scene/factories/geometric.h"

OrbsGraph::OrbsGraph(const Program & program,
    GeometricFactory & geometricFactory)
    : locs(program)
{
    // Five different colours, five different orbs
    // clang-format off
    const std::vector<const char *> colours{
        "#F00", "#FF0", "#0F0", "#0FF", "#00F"};
    // clang-format on

    for (auto i = 0; i < colours.size(); i++)
    {
        auto light = geometricFactory.getSphere(colours[i]);

        // start from y -0.5f so the orbs rise when the program starts
        light->setTranslation({rand(), -0.5f, rand()}).setScaling(SIZE);

        // the orb animator translates the orbs location, using Bezier curves
        light->addAnimator(std::make_shared<OrbAnimator>());
        sceneNode->addChild(light);

        // all the orbs have identical properties, but having a list allows
        // future pulsing animations etc, without changing client implementation
        const LightProperties lp{glm::vec3(0.0f), 0.1f, 0.5f,
            {0.18f, 0.32f}, {0.0f, 0.0f}};
        orbProperties.push_back(lp);
    }
}

/*
 * Sets the programs view matrix, and executes the animation updates (unless
 * animation is paused). The SceneNode, and its children, are then rendered
 * in the recursive render method.
 *
 * The client should have enabled the program for use before calling this
 * method.
 */
OrbsGraph &
OrbsGraph::render(const GLfloat deltaTime, const Camera & camera)
{
    locs.view.setMat4f(camera.getViewMatrix());
    if (!pauseAnimation) sceneNode->animate(deltaTime);
    sceneNode->updateTransformation();
    render(sceneNode);
    return *this;
}

/*
 * Render the given node, and recursively render its children.
 */
void
OrbsGraph::render(std::shared_ptr<SceneNode> node)
{
    locs.model.setMat4f(node->getWorldMatrix());
    node->render();

    for (auto child : node->getChildren()) render(child);
}

/*
 * Convenience method to return a random float between -2.0f and 2.0f, for
 * initial positioning of the orbs.
 */
GLfloat
OrbsGraph::rand()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<GLfloat> dis(-2.0f, 2.0f);
    GLfloat pos = 0.0f;

    while ((pos < 1.0f) && (pos > -1.0f)) pos = dis(gen);

    return pos;
}
