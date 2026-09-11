/*
 * glo/program/shader.cpp
 *
 * © Tim Clarke November 2025
 */

#include "shader.h"

#include <fstream>
#include <stdexcept>

/*
 * Creates a shader of the given type. The type is used to create a shader id,
 * and to determine which filename extension to use. Otherwise, the logic
 * remains the same.
 *
 * If an id cannot be allocated, or we cannot open the file, an exception is
 * thrown. Similarly, an exception is thrown if we fail to compile the shader
 * source.
 */
Shader::Shader(const GLuint type, const std::string_view & filename)
    : id(glCreateShader(type))
{
    if (id == 0)
    {
        throw std::runtime_error(
            std::string("Failed to create shader object for \"") +
            filename.data() + "\"");
    }

    const auto & ext =
        ((type == GL_VERTEX_SHADER) ? VERTEX_EXTENSION : FRAGMENT_EXTENSION);
    const auto path = (PATH + filename.data() + ext);
    std::ifstream file(path);

    if (!file.is_open())
    {
        glDeleteShader(id);
        throw std::runtime_error("Failed to open shader file \"" + path + "\"");
    }

    const std::string source{std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()};
    const char * const cSource = source.c_str();

    glShaderSource(id, 1, &cSource, nullptr);
    glCompileShader(id);

    GLint compileStatus;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);

    if (compileStatus != GL_TRUE)
    {
        glDeleteShader(id);
        std::string what = (std::string("Error in shader file ") + path);

        GLint logLength = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

        if (logLength > 1)
        {
            std::string infoLog(logLength, '\0');
            glGetShaderInfoLog(id, logLength, nullptr, infoLog.data());

            what += (std::string(":\n    ") + infoLog.data());
        }

        throw std::logic_error(what);
    }
}

/*
 * Safely move data from the temporary object, other. Other's id is set to 0,
 * allowing safe destruction.
 */
Shader &
Shader::operator=(Shader && other)
{
    if (this != &other)
    {
        if (id != 0) glDeleteShader(id);
        id = std::exchange(other.id, 0);
    }

    return *this;
}
