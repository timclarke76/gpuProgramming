/*
 * glo/program/uniform.h
 *
 * Wraps a uniform location into an object, allowing a more obect oriented
 * approach to uniform location management.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <cassert>
#include <format>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../log.h"
#include "program.h"

/*
 * A macro to allow easy wrapping of each glUniform function. Provide the
 * argument type, and the function name suffix.
 */
// clang-format off
#define UNIFORM(type, suffix) \
    const Uniform & set1##suffix(const type v0) const { \
        glUniform1##suffix(loc, v0); \
        return *this; \
    } \
    const Uniform & set2##suffix(const type v0, const type v1) const { \
        glUniform2##suffix(loc, v0, v1); \
        return *this; \
    } \
    const Uniform & set3##suffix(const type v0, const type v1, const type v2) const { \
        glUniform3##suffix(loc, v0, v1, v2); \
        return *this; \
    } \
    const Uniform & set4##suffix(const type v0, const type v1, const type v2, const type v3) const { \
        glUniform4##suffix(loc, v0, v1, v2, v3); \
        return *this; \
    } \
    const Uniform & set1##suffix(const type * const v) const { \
        assert(v != nullptr); \
        glUniform1##suffix##v(loc, 1, v); \
        return *this; \
    } \
    const Uniform & set2##suffix(const type * const v) const { \
        assert(v != nullptr); \
        glUniform2##suffix##v(loc, 1, v); \
        return *this; \
    } \
    const Uniform & set3##suffix(const type * const v) const { \
        assert(v != nullptr); \
        glUniform3##suffix##v(loc, 1, v); \
        return *this; \
    } \
    const Uniform & set4##suffix(const type * const v) const { \
        assert(v != nullptr); \
        glUniform4##suffix##v(loc, 1, v); \
        return *this; \
    }

/*
 * A macro to allow easy wrapping of the glUniformMatrix4 function. Provide the
 * function name suffix. TODO: extend for every variation.
 */
#define UNIFORM_MATRIX(suffix) \
     const Uniform & setMat4##suffix(const glm::mat4 & m) const { \
        glUniformMatrix4##suffix##v(loc, 1, GL_FALSE, glm::value_ptr(m)); \
        return *this; \
    }
// clang-format on

class Uniform
{
  public:
    /*
     * Gets and saves the uniform location for the program, identified by the
     * given name. If we fail to find the location, we simply log a warning, and
     * continue execution without throwing an exception.
     */
    Uniform(const Program & program, const std::string & name)
        : loc(glGetUniformLocation(program, name.c_str()))
    {
        if (loc == -1)
        {
            Log::w("Uniform", std::format("Location {} not found", name));
        }
    }

    // no destruction, copying allowed
    Uniform(const Uniform &) = default;
    Uniform & operator=(const Uniform &) = default;

    /*
     * Create the wrapper methods for GLfloat (f), GLint (i), GLuint (ui), and
     * GLdouble (d) glUniform functions.
     */
    UNIFORM(GLfloat, f)
    UNIFORM(GLint, i)
    UNIFORM(GLuint, ui)
    UNIFORM(GLdouble, d)

    /*
     * Create a wrapper methods for GLfloat (f) glUnfiromMatrix4 function.
     * TODO: extend for every type.
     */
    UNIFORM_MATRIX(f)

    /*
     * Convenience method for setting a single boolean. We hide from the client
     * that the boolean is converted to 1 or 0 appropriately, and set as an int
     * value.
     */
    const Uniform & set1b(const bool v0) const
    {
        glUniform1i(loc, v0 ? 1 : 0);
        return *this;
    }

  private:
    const GLint loc;
};

#undef UNIFORM
