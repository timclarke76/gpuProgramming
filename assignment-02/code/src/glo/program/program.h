/*
 * glo/program/program.h
 *
 * The program links a VertexShader and a FragmentShader.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <string_view>
#include <utility>

#include "../error.h"
#include "shader.h"

class Program
{
  public:
    Program(const VertexShader & vertexShader,
        const FragmentShader & fragmentShader)
    {
        Link(vertexShader, fragmentShader);
    }

    Program(const std::string_view & filename)
    {
        Link(filename.data(), filename.data());
    }

    // clang-format off
    ~Program() { if (id != 0) glDeleteProgram(id); }
    // clang-format on

    // prevent copies
    Program(const Program &) = delete;
    Program & operator=(const Program &) = delete;

    // safely move data
    Program(Program && other) : id(std::exchange(other.id, 0)) {}
    Program & operator=(Program && other);

    // clang-format off
    // Use the program. Both const and non-const versions provided for client
    // convenience.
    Program & use() { glUseProgram(id); GL_ERROR_CHECK; return *this; }
    const Program & use() const
        { glUseProgram(id); GL_ERROR_CHECK; return *this; }
    // clang-format on

    operator GLuint() const { return id; }

  private:
    void Link(const VertexShader & vertexShader,
        const FragmentShader & fragmentShader);

    GLuint id = glCreateProgram();
};
