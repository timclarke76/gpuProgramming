/*
 * glo/scene/factories/train.h
 *
 * Creates a simple train SceneNode graph, using geometric shapes from the
 * client supplied GeometricFactory. Animation is used to turn the wheels in a
 * constant forward direction.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <memory>

#include <GL/glew.h>

class GeometricFactory;
class SceneNode;

class TrainFactory
{
  public:
    // clang-format off
    TrainFactory(GeometricFactory & geometricFactory)
        : geometricFactory(geometricFactory) {}
    // clang-format on

    std::shared_ptr<SceneNode> create();

  private:
    GeometricFactory & geometricFactory;
};
