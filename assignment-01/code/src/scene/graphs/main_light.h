/*
 * scene/graphs/main_light.h
 *
 * The graph for the main lights. Currently only one light is defined, but this
 * may be extended in future to cater for table lamps etc.
 *
 * TODO: a program class should be created to retain uniform locations for the
 * program, and update their values. It should also be responsible for
 * recursive rendering, thus removing this responsibility from the graphs.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "../../glo/program/uniform.h"
#include "../../glo/scene/node.h"
#include "light_properties.h"

class Camera;
class GeometricFactory;
class Program;

class MainLightGraph
{
  private:
    static constexpr GLfloat SIZE = 0.05f;

    struct UniformLocations
    {
        Uniform view;
        Uniform model;

        UniformLocations(const Program & program)
            : view(program, "u_View"), model(program, "u_Model") {}
    };

  public:
    MainLightGraph(const Program & program,
        GeometricFactory & geometricFactory);

    // clang-format off
    const std::vector<std::shared_ptr<SceneNode>> & getLights() const
        { return sceneNode->getChildren(); }

    const std::vector<LightProperties> & getLightProperties() const
        { return lightProperties; }

    // The main light may be moved, usually for testing/debugging purposes.
    MainLightGraph & translate(const glm::vec3 & translation)
        { sceneNode->translate(translation); return *this; }

    // Reset the main light to its initial position.
    MainLightGraph & resetTranslation()
        { sceneNode->setTranslation({-2.0f, 2.0f, 2.0f}); return *this; }
    // clang-format on

    MainLightGraph & render(const GLfloat deltaTime, const Camera & camera);

  private:
    // these should be in a custom Program class
    void render(std::shared_ptr<SceneNode> node); // recursive
    UniformLocations locs;

    std::shared_ptr<SceneNode> sceneNode = std::make_shared<SceneNode>();
    std::vector<LightProperties> lightProperties;
};
