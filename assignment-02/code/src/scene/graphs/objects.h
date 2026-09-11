/*
 * scene/graphs/objects.h
 *
 * The graph for the cot mobile object. Some methods require the different light
 * graphs, so the shaders can calculate fragment colours.
 *
 * This graph also renders light from an "adventure torch", which can be toggled
 * on and off.
 *
 * TODO: a program class should be created to retain uniform locations for the
 * program, and update their values. It should also be responsible for
 * recursive rendering, thus removing this responsibility from the graphs.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include "../../glo/program/uniform.h"
#include "../../glo/scene/factories/geometric.h"
#include "../../glo/scene/node.h"
#include "../torch.h"

class Camera;
class MainLightGraph;
class OrbsGraph;
class Program;
class ModelFactory;
class TextureFactory;

class ObjectsGraph
{
  private:
    /*
     * These three Uniform structures mirror the structure of the Light
     * structres in the shaders. The individual properties are detailed in
     * light_properties.h
     */
    struct UniformAttenuationLocations
    {
        Uniform linear;
        Uniform quadratic;

        UniformAttenuationLocations(const Program & program,
            const std::string & idx)
            : linear(program, "u_Lights[" + idx + "].attenuation.linear"),
              quadratic(program, "u_Lights[" + idx + "].attenuation.quadratic")
        {
        }
    };

    struct UniformCutOffLocations
    {
        Uniform inner;
        Uniform outer;

        UniformCutOffLocations(const Program & program, const std::string & idx)
            : inner(program, "u_Lights[" + idx + "].cutOff.inner"),
              outer(program, "u_Lights[" + idx + "].cutOff.outer")
        {
        }
    };

    struct UniformLightLocations
    {
        Uniform position;
        Uniform direction;
        Uniform colour;
        Uniform ambientIntensity;
        Uniform specularIntensity;
        UniformAttenuationLocations attenuation;
        UniformCutOffLocations cutOff;

        UniformLightLocations(const Program & program, const std::string & idx)
            : position(program, "u_Lights[" + idx + "].position"),
              direction(program, "u_Lights[" + idx + "].direction"),
              colour(program, "u_Lights[" + idx + "].colour"),
              ambientIntensity(program,
                  "u_Lights[" + idx + "].ambientIntensity"),
              specularIntensity(program,
                  "u_Lights[" + idx + "].specularIntensity"),
              attenuation(program, idx),
              cutOff(program, idx)
        {
        }
    };

    // This structure holds all of the uniform locations that we'll need,
    // including property locations for each light in the orbs graph, the main
    // lights graph, and another for our torch.
    struct UniformLocations
    {
        Uniform projection;
        Uniform view;
        Uniform model;
        Uniform position;

        // Does the object have a texture? And if so, where is it?
        Uniform hasTexture;
        Uniform texture;

        // Does the object have a normals texture? And if so, where is it?
        Uniform hasNormals;
        Uniform normals;

        // Blinn Phong Specular can be switched on and off for testing
        Uniform useBlinnPhongSpecular;

        // The total number of lights, and their properties, for the scene. This
        // may change as features are toggled on and off.
        Uniform numLights;
        std::vector<UniformLightLocations> lights;

        UniformLocations(const Program & program, const int numLights)
            : projection(program, "u_Projection"),
              view(program, "u_View"),
              model(program, "u_Model"),
              position(program, "u_Position"),

              // not all of our objects use a texture
              hasTexture(program, "u_HasTexture"),
              texture(program, "u_Texture"),

              // not all of our objects use a normals texture
              hasNormals(program, "u_HasNormals"),
              normals(program, "u_Normals"),

              // Blinn Phong Specular may be toggled on and off
              useBlinnPhongSpecular(program, "u_UseBlinnPhongSpecular"),

              // the shader needs to be told how many lights we're using
              numLights(program, "u_NumLights")
        {
            // create a UniformLightsLocations object for each light that we'll
            // be rendering
            for (auto lightIdx = 0; lightIdx < numLights; lightIdx++)
            {
                lights.push_back(
                    UniformLightLocations(program, std::to_string(lightIdx)));
            }
        }
    };

  public:
    ObjectsGraph(Program & program, GeometricFactory & geometricFactory,
        ModelFactory & modelFactory, const OrbsGraph & orbsGraph,
        const MainLightGraph & mainLightGraph);

    // === allow the user to toggle on and off different features ===
    // clang-format off
    ObjectsGraph & toggleBlinnPhongSpecular()
        { useBlinnPhongSpecular = !useBlinnPhongSpecular; return *this; }
    bool getUseBlinnPhongSpecular() { return useBlinnPhongSpecular; }
    ObjectsGraph & toggleOrbs()
        { showOrbs = !showOrbs; return *this; }
    ObjectsGraph & toggleMainLights()
        { showMainLights = !showMainLights; return *this; }
    ObjectsGraph & toggleAnimation()
        { pauseAnimation = !pauseAnimation; return *this; }
    ObjectsGraph & toggleMobileRotation();
    ObjectsGraph & toggleNormalsMapping()
        { showNormalsMapping = !showNormalsMapping; return *this; }
    bool getShowNormalsMapping() { return showNormalsMapping; }
    // clang-format on

    Torch & getTorch() { return torch; }
    bool getShowOrbs() { return showOrbs; }
    bool getShowMainLights() { return showMainLights; }

    void render(const GLfloat deltaTime, const Camera & camera,
        const OrbsGraph & orbsGraph, const MainLightGraph & mainLightGraph);

  private:
    void updateUniforms(const UniformLightLocations & loc, const glm::vec3 &
        position, const glm::vec3 & front, const glm::vec3 & colour,
        const LightProperties & props);

    // these should be in a custom Program class
    void render(std::shared_ptr<SceneNode> node); // recursive
    UniformLocations locs;

    std::shared_ptr<SceneNode> sceneNode = std::make_shared<SceneNode>();
    Torch torch; // our adventure torch light

    // features toggled on and off by the user
    bool useBlinnPhongSpecular = true;
    bool showOrbs = true;
    bool showMainLights = true;
    bool pauseAnimation = false;
    bool showNormalsMapping = true;
};
