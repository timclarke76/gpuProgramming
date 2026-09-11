/*
 * glo/program/shader.h
 *
 * Shader has a protected constructor. Clients must use one of the derived
 * classes --- VertexShader for vertices, and FragmentShader for fragments.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <string>
#include <string_view>
#include <utility>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Shader
{
    // file extension for vertex shaders
    static constexpr auto VERTEX_EXTENSION = std::string(".vert");

    // file extension for fragment shaders
    static constexpr auto FRAGMENT_EXTENSION = std::string(".frag");

    // the relative directory where shaders should be found
    static constexpr auto PATH = std::string("shaders/");

  protected:
    Shader(const GLuint type, const std::string_view & filename);

  public:
    // clang-format off
    ~Shader() { if (id != 0) glDeleteShader(id); }
    // clang-format on

    // prevent copies
    Shader(const Shader &) = delete;
    Shader & operator=(const Shader &) = delete;

    // safely move data
    Shader(Shader && other) : id(std::exchange(other.id, 0)) {}
    Shader & operator=(Shader && other);

    operator GLuint() const { return id; }

  private:
    GLuint id;
};

class VertexShader : public Shader
{
  public:
    // clang-format off
    VertexShader(const char * const filename)
        : Shader(GL_VERTEX_SHADER, filename) {}
    // clang-format on
};

class FragmentShader : public Shader
{
  public:
    // clang-format off
    FragmentShader(const char * const filename)
        : Shader(GL_FRAGMENT_SHADER, filename) {}
    // clang-format on
};
