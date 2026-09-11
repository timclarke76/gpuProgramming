/*
 * glo/program/program.cpp
 *
 * © Tim Clarke November 2025
 */

#include "program.h"

#include <stdexcept>
#include <string>

/*
 * Should only be called during construction. As the id is created during
 * initialisation, we check if the call to glCreateProgram was successful here.
 *
 * We attach both shaders to the program, link them, then detach them again. If
 * linking fails, we construct a suitable error message, record it to the log,
 * and throw an exception. The program is deleted in such a case, to prevent
 * partial construction.
 */
void
Program::Link(const VertexShader & vertexShader,
    const FragmentShader & fragmentShader)
{
    if (id == 0) throw std::runtime_error("Failed to create program");

    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);
    glDetachShader(id, fragmentShader);
    glDetachShader(id, vertexShader);

    GLint linkStatus;
    glGetProgramiv(id, GL_LINK_STATUS, &linkStatus);

    if (linkStatus != GL_TRUE)
    {
        glDeleteProgram(id);
        id = 0;

        std::string what("Program linking failed");

        GLint logLength = 0;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);

        if (logLength > 1)
        {
            std::string infoLog(logLength, '\0');
            glGetProgramInfoLog(id, logLength, nullptr, infoLog.data());

            what += (std::string(":\n    ") + infoLog.data());
        }

        throw std::logic_error(what);
    }
}

/*
 * Safely move data from the temporary object, other. Other's id is set to 0,
 * allowing safe destruction.
 */
Program &
Program::operator=(Program && other)
{
    if (this != &other)
    {
        if (id != 0) glDeleteProgram(id);
        id = std::exchange(other.id, 0);
    }

    return *this;
}
