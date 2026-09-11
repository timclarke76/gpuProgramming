/*
 * glo/window.cpp
 *
 * © Tim Clarke November 2025
 */

#include <format>
#include <iostream>

#include "error.h"
#include "log.h"

#include "window.h"

std::map<GLFWwindow *, Window *> Window::map;

std::ostream &
operator<<(std::ostream & os, const Window & obj)
{
    GLint major, minor;

    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    // clang-format off
    os << "{"
       <<     "\"_type\":\"Window\","
       <<     "\"vendor\":\"" << glGetString(GL_VENDOR) << "\","
       <<     "\"version\":\"" << glGetString(GL_VERSION) << "\","
       <<     "\"renderer\":\"" << glGetString(GL_RENDERER) << "\","
       <<     "\"openGl\":" << major << "." << minor << ","
       <<     "\"projection\":" << obj.projection << ","
       <<     "\"backgroundColour\":" << obj.backgroundColour
       << "}";
    // clang-format on

    return os;
}

/*
 * Log OpenGL debug messages.
 */
void
onDebugMessageCallback(const GLenum source, const GLenum type, const GLuint id,
    const GLenum severity, const GLsizei length, const GLchar * const message,
    const void * const userParam)
{
    std::string safeMessage(message, length > 0 ? length : 0);

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_NOTIFICATION:
        {
            Log::d("OpenGL", safeMessage);
        }
        break;

        case GL_DEBUG_SEVERITY_LOW:
        {
            Log::i("OpenGL", safeMessage);
        }
        break;

        case GL_DEBUG_SEVERITY_MEDIUM:
        {
            Log::w("OpenGL", safeMessage);
        }
        break;

        case GL_DEBUG_SEVERITY_HIGH:
        {
            Log::e("OpenGL", safeMessage);
        }
        break;
    }
}

/*
 * The callback function registered to OpenGL to receive notifications about
 * keyboard events.
 */
void
onKeyboardEvent(GLFWwindow * const glfwWindow, const GLint key,
    const GLint scancode, const GLint action, const GLint mods)
{
    // find the correct Window
    if (Window::map.count(glfwWindow) == 0) return;
    Window & window = *Window::map[glfwWindow];

    // notify each listener
    for (auto l : window.keyboardListeners)
    {
        l->onKeyboardEvent(window, key, scancode, action, mods);
    }
}

/*
 * The callback function registered to OpenGL to receive notifications about
 * mouse movement events.
 */
void
onMouseMoveEvent(GLFWwindow * const glfwWindow, const GLdouble xpos,
    const GLdouble ypos)
{
    // find the correct Window
    if (Window::map.count(glfwWindow) == 0) return;
    Window & window = *Window::map[glfwWindow];

    // notify each listener
    for (auto l : window.mouseListeners)
    {
        l->onMouseMoveEvent(window, xpos, ypos);
    }
}


/*
 * The callback function registered to OpenGL to receive notifications about
 * mouse scroll events.
 */
void
onMouseScrollEvent(GLFWwindow * const glfwWindow, const GLdouble xoffset,
    const GLdouble yoffset)
{
    // find the correct Window
    if (Window::map.count(glfwWindow) == 0) return;
    Window & window = *Window::map[glfwWindow];

    // notify each listener
    for (auto l : window.mouseListeners)
    {
        l->onMouseScrollEvent(window, xoffset, yoffset);
    }
}

/*
 * The callback function registered to OpenGL to receive notifications about
 * window focus events.
 */
void
onWindowFocusEvent(GLFWwindow * const glfwWindow, const GLint focused)
{
    // find the correct Window
    if (Window::map.count(glfwWindow) == 0) return;
    Window & window = *Window::map[glfwWindow];

    // notify each listener
    for (auto l : window.windowListeners)
    {
        l->onWindowFocusEvent(window, (focused == GLFW_TRUE));
    }
}

/*
 * The callback function registered to OpenGL to receive notifications about
 * window size events.
 */
void
onWindowSizeEvent(GLFWwindow * const glfwWindow, const GLint width,
    const GLint height)
{
    // find the correct Window
    if (Window::map.count(glfwWindow) == 0) return;
    Window & window = *Window::map[glfwWindow];

    // notify each listener
    for (auto l : window.windowListeners)
    {
        l->onWindowSizeEvent(window, width, height);
    }
}

/*
 * Construct a new window according to the client's configuraiton.
 *
 * glfwInit must have been successfully called before Window construction.
 */
Window::Window(const Config & config)
    : projection(config.viewportWidth, config.viewportHeight, config.fov),
      backgroundColour(config.backgroundColour),
      lastTime(glfwGetTime()),
      deltaTime(lastTime)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.minorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, config.profile);
    glfwWindowHint(GLFW_SAMPLES, config.samples);
    glfwWindowHint(GLFW_RESIZABLE, config.resizable);

    window = glfwCreateWindow(projection.getWidth(), projection.getHeight(),
        config.title.c_str(), nullptr, nullptr);

    if (window == nullptr)
    {
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        glfwDestroyWindow(window);
        throw std::runtime_error("Failed to initialise Glew");
    }

    glfwSetErrorCallback([](const GLint error, const char * const description)
        { Log::e("GLFW", std::format("{}: {}", error, description)); });

    if (config.enableDebug)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(onDebugMessageCallback, nullptr);
    }

    if (config.pointSize <= 0.0f)
    {
        glDisable(GL_PROGRAM_POINT_SIZE);
    }
    else
    {
        glEnable(GL_PROGRAM_POINT_SIZE);
        glPointSize(config.pointSize);
    }

    glfwSetInputMode(window, GLFW_CURSOR,
        config.enableCursor ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, config.enableStickyKeys);

    if (config.samples > 0) glEnable(GL_MULTISAMPLE);
    else glDisable(GL_MULTISAMPLE);

    if (config.enableDepthTest) glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);

    if (config.enableCullFace) glEnable(GL_CULL_FACE);
    else glDisable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // register for window events, so we know when we resize
    addWindowListener(this);

    // register a pointer to this window, so the callback functions can find us
    map[window] = this;
}

/*
 * Remove the reference to this window in our map, and destroy the OpenGL
 * window.
 */
Window::~Window()
{
    map.erase(window);
    glfwDestroyWindow(window);
}

/*
 * Register a keyboard listener, and our callback function if this is the only
 * keyboard listener.
 */
Window &
Window::addKeyboardListener(KeyboardListener * const listener)
{
    assert(listener != nullptr);

    if (keyboardListeners.empty()) glfwSetKeyCallback(window, onKeyboardEvent);
    keyboardListeners.insert(listener);

    return *this;
}

/*
 * Remove a keyboard listener. If we have no more keyboard listeners, unregister
 * our callback function to save processing events.
 */
Window &
Window::removeKeyboardListener(KeyboardListener * const listener)
{
    assert(listener != nullptr);

    if (keyboardListeners.erase(listener) && keyboardListeners.empty())
        glfwSetKeyCallback(window, nullptr);

    return *this;
}

/*
 * Register a mouse listener, and our callback functions if this is the only
 * mouse listener.
 */
Window &
Window::addMouseListener(MouseListener * const listener)
{
    assert(listener != nullptr);

    if (mouseListeners.empty())
    {
        glfwSetCursorPosCallback(window, onMouseMoveEvent);
        glfwSetScrollCallback(window, onMouseScrollEvent);
    }

    mouseListeners.insert(listener);

    return *this;
}

/*
 * Remove a mouse listener. If we have no more mouse listeners, unregister
 * our callback functions to save processing events.
 */
Window &
Window::removeMouseListener(MouseListener * const listener)
{
    assert(listener != nullptr);

    if (mouseListeners.erase(listener) && mouseListeners.empty())
    {
        glfwSetCursorPosCallback(window, nullptr);
        glfwSetScrollCallback(window, nullptr);
    }

    return *this;
}

/*
 * Register a window listener, and our callback functions if this is the only
 * window listener.
 */
Window &
Window::addWindowListener(WindowListener * const listener)
{
    assert(listener != nullptr);

    if (windowListeners.empty())
    {
        glfwSetFramebufferSizeCallback(window, &::onWindowSizeEvent);
        glfwSetWindowFocusCallback(window, &::onWindowFocusEvent);
    }

    windowListeners.insert(listener);

    return *this;
}

/*
 * Remove a window listener. If we have no more window listeners, unregister
 * our callback functions to save processing events.
 */
Window &
Window::removeWindowListener(WindowListener * const listener)
{
    assert(listener != nullptr);

    if (windowListeners.erase(listener) && windowListeners.empty())
    {
        glfwSetFramebufferSizeCallback(window, nullptr);
        glfwSetWindowFocusCallback(window, nullptr);
    }

    return *this;
}

/*
 * Our main loop. Returns when the window is marked for closure.
 *
 * TODO: limit the fps
 */
Window &
Window::Loop()
{
    GLfloat currentTime;

    while (!getShouldClose())
    {
        // calculate the delta time since the last loop, and record the current
        // time for the next loop
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        const auto & rgba = backgroundColour.getRgba();
        glClearColor(rgba[0], rgba[1], rgba[2], rgba[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto & l : loopListeners) l->onLoopEvent(*this);

        glfwSwapBuffers(window);
        glfwPollEvents();
        GL_ERROR_CHECK;
    }

    return *this;
}
