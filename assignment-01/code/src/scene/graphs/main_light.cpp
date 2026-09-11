/*
 * scene/graphs/main_light.cpp
 *
 * © Tim Clarke November 2025
 */

#include "main_light.h"

#include <random>

#include "../../glo/camera.h"
#include "../../glo/program/program.h"
#include "../../glo/scene/factories/geometric.h"

MainLightGraph::MainLightGraph(const Program & program,
    GeometricFactory & geometricFactory)
    : locs(program)
{
    // a pure white light makes testing easier

    auto light = geometricFactory.getSphere("#FFF");
    light->setScaling(SIZE);
    sceneNode->addChild(light);

    // a fixed initial position
    sceneNode->setTranslation({-2.0f, 2.0f, 2.0f});

    // only one light is provided, but this may be extended in the future for
    // table lamps etc
    const LightProperties lp{{0.0f, -1.0f, 0.0f}, 0.3f, 1.0f,
        {0.0f, 0.0f}, {0.0f, 0.0f}};
    lightProperties.push_back(lp);
}

/*
 * Sets the programs view matrix. The SceneNode, and its children, are then
 * rendered in the recursive render method.
 *
 * The client should have enabled the program for use before calling this
 * method.
 */
MainLightGraph &
MainLightGraph::render(const GLfloat deltaTime, const Camera & camera)
{
    locs.view.setMat4f(camera.getViewMatrix());
    sceneNode->updateTransformation();
    render(sceneNode);
    return *this;
}

/*
 * Render the given node, and recursively render its children.
 */
void
MainLightGraph::render(std::shared_ptr<SceneNode> node)
{
    locs.model.setMat4f(node->getWorldMatrix());
    node->render();

    for (auto child : node->getChildren()) render(child);
}
