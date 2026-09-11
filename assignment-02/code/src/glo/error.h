/*
 * glo/error.h
 *
 * A simple macro and function to check for OpenGL errors. If an error has
 * occurred, an error is logged and an exception thrown. Clients should use the
 * GL_ERROR_CHECK macro, and avoid glErrorCheck, for easy recording of
 * __FILE__ and __LINE__.
 *
 * TODO: confirm usage for every interaction with OpenGL across whole system.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#define GL_ERROR_CHECK glErrorCheck(__FILE__, __LINE__)

#include <format>
#include <stdexcept>

#include <GL/glew.h>

#include "log.h"

inline void
glErrorCheck(const char * const file, const int line)
{
    GLenum error;

    if ((error = glGetError()) != GL_NO_ERROR)
    {
        // clang-format off
        const auto s = std::format("OpenGL error [{}] at line {} in file {}",
            error, line, file);
        // clang-format on

        Log::e("OpenGL", s);
        throw std::runtime_error(s);
    }
}
