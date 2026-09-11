/*
 * scene/graphs/objects.cpp
 *
 * © Tim Clarke November 2025
 */

#include "objects.h"

#include "../../glo/camera.h"
#include "../../glo/program/program.h"
#include "../../glo/scene/animators/mobile.h"
#include "../../glo/scene/factories/cot_mobile.h"
#include "main_light.h"
#include "orbs.h"

ObjectsGraph::ObjectsGraph(Program & program,
    GeometricFactory & geometricFactory, const OrbsGraph & orbsGraph,
    const MainLightGraph & mainLightGraph)
    : locs(program, orbsGraph.getOrbs().size()
        + mainLightGraph.getLights().size() + 1)
{
    sceneNode->addChild(CotMobileFactory(geometricFactory).create());
}

/*
 * We reach into the scene graph to retrieve the animator for the mobile, and
 * toggle its pause state, allowing the ornaments to continue spinning.
 * Available for testing/verification purposes.
 */
ObjectsGraph &
ObjectsGraph::toggleMobileRotation()
{
    auto animator = sceneNode->getChildren()[0]->getAnimators()[0];
    auto mobileAnimator = std::static_pointer_cast<MobileAnimator>(animator);
    mobileAnimator->togglePause();
    return *this;
}

/*
 * Render the full scene, updating the light uniforms first.
 */
void
ObjectsGraph::render(const GLfloat deltaTime, const Camera & camera,
    const OrbsGraph & orbsGraph, const MainLightGraph & mainLightGraph)
{
    const auto & view = camera.getViewMatrix();
    const auto & position = camera.getPosition();

    locs.view.setMat4f(view);
    locs.position.set3f(&position[0]);

    locs.useBlinnPhongSpecular.set1i(useBlinnPhongSpecular ? 1 : 0);

    // === Update the light properties for the orbs, if they are enabled ===

    const auto orbs = orbsGraph.getOrbs();
    const auto numOrbs = showOrbs ? orbs.size() : 0;

    for (int lightIdx = 0; lightIdx < numOrbs; lightIdx++)
    {
        const auto & orb = orbs[lightIdx];

        const auto & loc = locs.lights[lightIdx];
        const auto & props = orbsGraph.getOrbProperties()[lightIdx];
        const auto & front = props.direction;

        const auto & position = orb->getWorldPosition();
        const auto & colour = orb->getRenderable()->getMaterial()
            .getBaseColour().value().getRgb();

        updateUniforms(loc, position, front, colour, props);
    }

    // === Update the light properties for the main lights, if enabled ===

    const auto mainLights = mainLightGraph.getLights();
    const auto numMainLights = showMainLights ? mainLights.size() : 0;

    for (int lightIdx = 0; lightIdx < mainLights.size(); lightIdx++)
    {
        const auto & light = mainLights[lightIdx];

        const auto & loc = locs.lights[numOrbs + lightIdx];
        const auto & props = mainLightGraph.getLightProperties()[lightIdx];
        const auto & front = props.direction;

        const auto & position = light->getWorldPosition();
        const auto & colour = light->getRenderable()->getMaterial()
            .getBaseColour().value().getRgb();

        updateUniforms(loc, position, front, colour, props);
    }

    // === Update the light properties for the torch, if powered ===

    if (torch.isPowered())
    {
        // the torch is always in our position, facing our direction
        const auto & loc = locs.lights[numOrbs + numMainLights];
        const auto & props = torch.getProperties();
        const auto & front = camera.getFront();
        const auto & colour = torch.getLensColour().getRgb();

        updateUniforms(loc, position, front, colour, props);
    }

    // tell the shaders how many lights are in the scene
    auto numLights = numOrbs + numMainLights + (torch.isPowered() ? 1 : 0);
    locs.numLights.set1i(numLights);

    if (!pauseAnimation) sceneNode->animate(deltaTime);
    sceneNode->updateTransformation();
    render(sceneNode);
}

void
ObjectsGraph::updateUniforms(const UniformLightLocations & loc,
    const glm::vec3 & position, const glm::vec3 & front,
    const glm::vec3 & colour, const LightProperties & props)
{
    loc.position.set3f(&position[0]);
    loc.direction.set3f(&front[0]);
    loc.colour.set3f(&(colour[0]));
    loc.ambientIntensity.set1f(props.ambientIntensity);
    loc.specularIntensity.set1f(props.specularIntensity);
    loc.attenuation.linear.set1f(props.attenuation.linear);
    loc.attenuation.quadratic.set1f(props.attenuation.quadratic);
    loc.cutOff.inner.set1f(props.cutOff.inner);
    loc.cutOff.outer.set1f(props.cutOff.outer);
}

/*
 * Render the provided node, providing its texture and colour information to the
 * shader, along with the world matrix. Then recursively render each child.
 */
void
ObjectsGraph::render(std::shared_ptr<SceneNode> node)
{
    if (node->getRenderable())
    {
        // this information needs to be provided to the shader for each
        // renderable.

        auto material = node->getRenderable()->getMaterial();
        locs.hasTexture.set1b(material.hasTexture());

        if (material.hasTexture())
        {
            material.getTexture().value()->bind(GL_TEXTURE0);
        }
    }

    locs.model.setMat4f(node->getWorldMatrix());
    node->render();

    for (auto c : node->getChildren()) render(c);
}
