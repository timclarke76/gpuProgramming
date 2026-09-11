/*
 * glo/scene/factories/cot_mobile.h
 *
 * Creates a cot mobile SceneNode graph. The mobile is made from three geometric
 * shapes, textured. A fourth ornament is made from another graph, representing
 * a simple train. The top of the mobile has two white crossbars, and the
 * ornaments "hang" from semi-transparent threads. The ornaments each rotate
 * randomly to simulate natural spin. The whole mobile rotates at a constant
 * speed, in one direction only.
 *
 * CITE: discussions with Google Gemini AI (https://gemini.google.com/) were had
 * to generate images for the cube, sphere, and star. These images were modified
 * as needed, and added to appropriate texture wraps. Software used was Inkscape
 * and Gimp.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <memory>

#include <GL/glew.h>

class GeometricFactory;
class SceneNode;

class CotMobileFactory
{
  private:
    static constexpr GLfloat RADIUS = 1.5f;
    static constexpr GLfloat ORNAMENT_SIZE = 0.5f;
    static constexpr GLfloat THREAD_LENGTH = 5.0f;

  public:
    // clang-format off
    CotMobileFactory(GeometricFactory & geometricFactory)
        : geometricFactory(geometricFactory) {}
    // clang-format on

    std::shared_ptr<SceneNode> create();

  private:
    GeometricFactory & geometricFactory;
};
