/*
 * scene/graphs/orbs.h
 *
 * The graph for the orbs. The graph contains five orbs of different colours,
 * each animated to float around the upper part of the scene. The orbs start at
 * the bottom of the scene so that when the graph animations begin, they all
 * float upwards.
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

class OrbsGraph
{
  private:
    static constexpr GLfloat SIZE = 0.05f;

    struct UniformLocations
    {
        Uniform orbsView;
        Uniform orbsModel;
        Uniform particlesView;

        // clang-format off
        UniformLocations(const Program & orbsProgram,
            const Program & particlesProgram)
            : orbsView(orbsProgram, "u_View"),
              orbsModel(orbsProgram, "u_Model"),
              particlesView(particlesProgram, "u_View") {}
        // clang-format on
    };

  public:
    OrbsGraph(const Program & program, const Program & particlesProgram,
        GeometricFactory & geometricFactory);

    // clang-format off
    const std::vector<std::shared_ptr<SceneNode>> & getOrbs() const
        { return sceneNode->getChildren(); }

    const std::vector<LightProperties> & getOrbProperties() const
        { return orbProperties; }

    // Freezes the orbs in place when animation is paused.
    OrbsGraph & toggleAnimation()
        { pauseAnimation = !pauseAnimation; return *this; }
    OrbsGraph & toggleEmission()
        { pauseEmission = !pauseEmission; return *this; }
    // clang-format on

    OrbsGraph & render(const GLfloat deltaTime, const Camera & camera);
    OrbsGraph & renderParticles(const GLfloat deltaTime, const Camera & camera);

  private:
    static GLfloat rand();

    // these should be in a custom Program class
    void render(std::shared_ptr<SceneNode> node); // recursive
    void renderParticles(std::shared_ptr<SceneNode> node); // recursive
    UniformLocations locs;

    std::shared_ptr<SceneNode> sceneNode = std::make_shared<SceneNode>();
    std::vector<LightProperties> orbProperties;

    bool pauseAnimation = false;
    bool pauseEmission = false;
};
