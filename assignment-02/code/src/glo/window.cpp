/*
 * glo/window.cpp
 *
 * © Tim Clarke November 2025
 */

#include "window.h"

#include <format>
#include <iostream>

#include "mesh/shapes/quad.h"
#include "error.h"
#include "log.h"

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
    : config(config),
      projection(config.viewportWidth, config.viewportHeight, config.fov),
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

    initialiseSceneFramebuffer();
    initialiseGaussianFramebuffers();

    numBlurTaps.pushBack(9).pushBack(7).pushBack(5).pushBack(3);
    toneMappingTechnique.pushBack(3).pushBack(2).pushBack(1).pushBack(0);
}

/*
 * Remove the reference to this window in our map, and destroy the OpenGL
 * window and our framebuffer resources.
 */
Window::~Window()
{
    map.erase(window);

    if (FBO != 0)
    {
        glDeleteFramebuffers(1, &FBO);
        Log::d("Window", std::format("FBO {} deleted", FBO));
    }

    if (fboTexture != 0)
    {
        glDeleteFramebuffers(1, &fboTexture);
        Log::d("Window", std::format("fboTexture {} deleted", fboTexture));
    }

    if (RBO != 0)
    {
        glDeleteRenderbuffers(1, &RBO);
        Log::d("Window", std::format("RBO {} deleted", RBO));
    }

    if (gaussianFBO[0] != 0)
    {
        glDeleteFramebuffers(2, gaussianFBO);
        Log::d("Window", std::format("gaussianFBO [{},{}] deleted",
            gaussianFBO[0], gaussianFBO[1]));
    }

    if (gaussianTextures[0] != 0)
    {
        glDeleteFramebuffers(2, gaussianTextures);
        Log::d("Window", std::format("gaussianTextures [{},{}] deleted",
            gaussianTextures[0], gaussianTextures[1]));
    }

    glfwDestroyWindow(window);
}

/*
 * Initialise a texture using sensible parameters. The texture id must have
 * already been initialised by the client using a call to glGenTextures.
 *
 * Used for our main rendering FBO texture, and the Gaussian blur textures.
 */
void
Window::initialiseTexture(const GLuint id)
{
    glBindTexture(GL_TEXTURE_2D, id);

    // We use GL_RGBA16F for high precision colours
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, projection.getWidth(),
        projection.getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
        id, 0);

    GL_ERROR_CHECK;
}

/*
 * Initialises our framebuffers that we use to render the scene in the main
 * program. This should be called whenever the viewport size changes. Any
 * previously created framebuffers are deleted first.
 */
void
Window::initialiseSceneFramebuffer()
{
    if (FBO != 0)
    {
        glDeleteFramebuffers(1, &FBO);
        GL_ERROR_CHECK;
        Log::d("Window", std::format("FBO {} deleted", FBO));
        FBO = 0;
    }

    if (fboTexture != 0)
    {
        glDeleteFramebuffers(1, &fboTexture);
        GL_ERROR_CHECK;
        Log::d("Window", std::format("fboTexture {} deleted", fboTexture));
        fboTexture = 0;
    }

    if (RBO != 0)
    {
        glDeleteRenderbuffers(1, &RBO);
        GL_ERROR_CHECK;
        Log::d("Window", std::format("RBO {} deleted", RBO));
        RBO = 0;
    }

    // === FRAMEBUFFER: FBO ===
    glGenFramebuffers(1, &FBO);
    if (FBO == 0) throw std::runtime_error("Failed to create FBO");
    Log::d("Window", std::format("FBO {} created", FBO));
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    GL_ERROR_CHECK;

    // === SCENE TEXTURE ===
    glGenTextures(1, &fboTexture);
    if (fboTexture == 0)
        throw std::runtime_error("Failed to create fboTexture");
    Log::d("Window", std::format("fboTexture {} created", fboTexture));
    initialiseTexture(fboTexture);

    // === DEPTH TESTING BUFFER: RBO ===
    glGenRenderbuffers(1, &RBO);
    if (RBO == 0) throw std::runtime_error("Failed to create RBO");
    Log::d("Window", std::format("RBO {} created", RBO));
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
        projection.getWidth(), projection.getHeight());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER, RBO);
    GL_ERROR_CHECK;

    // === SANITY CHECK ===
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        const auto errorStr = "Scene framebuffer is incomplete.";
        Log::e("Window", errorStr);
        throw std::runtime_error(errorStr);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void
Window::initialiseGaussianFramebuffers()
{
    if (gaussianFBO[0] != 0)
    {
        glDeleteFramebuffers(2, gaussianFBO);
        GL_ERROR_CHECK;
        Log::d("Window", std::format("gaussianFBO [{},{}] deleted",
            gaussianFBO[0], gaussianFBO[1]));
        gaussianFBO[0] = 0;
    }

    if (gaussianTextures[0] != 0)
    {
        glDeleteFramebuffers(2, gaussianTextures);
        GL_ERROR_CHECK;
        Log::d("Window", std::format("gaussianTextures [{},{}] deleted",
            gaussianTextures[0], gaussianTextures[1]));
        gaussianTextures[0] = 0;
    }

    glGenFramebuffers(2, gaussianFBO);
    if (gaussianFBO[0] == 0)
        throw std::runtime_error("Failed to create gaussianFBO");
    Log::d("Window", std::format("gaussianFBO [{},{}] created",
        gaussianFBO[0], gaussianFBO[1]));

    glGenTextures(2, gaussianTextures);
    if (gaussianTextures[0] == 0)
        throw std::runtime_error("Failed to create gaussianTextures");
    Log::d("Window", std::format("gaussianTextures [{},{}] created",
        gaussianTextures[0], gaussianTextures[1]));

    for (auto gaussianIdx = 0; gaussianIdx < 2; gaussianIdx++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, gaussianFBO[gaussianIdx]);
        glViewport(0, 0, projection.getWidth(), projection.getHeight());
        initialiseTexture(gaussianTextures[gaussianIdx]);

        // === SANITY CHECK ===
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            const auto errorStr = std::format(
                "Gaussian framebuffer {} is incomplete.", gaussianIdx);
            Log::e("Window", errorStr);
            throw std::runtime_error(errorStr);
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
 */
Window &
Window::Loop()
{
    GLfloat currentTime;

    // === SHADER PROGRAMS ===
    const BrightPassShader brightPassShader;
    brightPassShader.program.use();
    brightPassShader.sceneTextureLoc.set1i(0);
    brightPassShader.brightnessThresholdLoc.set1f(BRIGHTNESS_THRESHOLD);

    const BlurShader blurShader;
    blurShader.program.use();
    blurShader.srcTextureLoc.set1i(0);

    const CompositeShader compositeShader;
    compositeShader.program.use();
    compositeShader.sceneTextureLoc.set1i(0);
    compositeShader.bloomTextureLoc.set1i(1);
    compositeShader.bloomStrengthLoc.set1f(BLOOM_AMOUNT);

    // used to render to a framebuffer
    const auto quad = QuadMesh();

    while (!getShouldClose())
    {
        // calculate the delta time since the last loop, and record the current
        // time for the next loop
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // === PIPELINE ===
        // 1. Render the original scene.
        // 2. Determine and render the bright parts of the scene.
        // 3. Blur the bright parts of the scene for a bloom effect.
        // 4. Combine the original scene, and the bloom, into one framebuffer.
        renderScene();
        if (config.enableDepthTest) glDisable(GL_DEPTH_TEST);
        brightPass(brightPassShader, quad);
        const auto bloomTexture = blur(blurShader, quad);
        renderComposite(compositeShader, bloomTexture, quad);
        if (config.enableDepthTest) glEnable(GL_DEPTH_TEST);

        glfwSwapBuffers(window);
        glfwPollEvents();
        glfwWaitEventsTimeout(FPS);
        GL_ERROR_CHECK;
    }

    return *this;
}

/*
 * Renders our scene to the fboTexture.
 */
void
Window::renderScene()
{
    const auto & rgba = backgroundColour.getRgba();

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glClearColor(rgba[0], rgba[1], rgba[2], rgba[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto & l : loopListeners) l->onLoopEvent(*this);
}

/*
 * Passes the scene generated into fboTexture through the bright pass shader,
 * identifying any parts that are over the brightness threshold previously set.
 *
 * The identified bright parts of the scene are rendered to the first gaussian
 * FBO (gaussianFBO[0]), using the supplied QuadMesh.
 */
void
Window::brightPass(const BrightPassShader & shader, const QuadMesh & quad)
{
    shader.program.use();

    // source
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fboTexture);

    // destination
    glBindFramebuffer(GL_FRAMEBUFFER, gaussianFBO[0]);
    glClear(GL_COLOR_BUFFER_BIT);

    quad.draw();
}

/*
 * Blur the texture, starting with gaussianTextures[0]. We "ping pong" between
 * two textures for BLUR_AMOUNT times, swapping direction (horizontal/vertical)
 * as we swap textures.
 *
 * The final texture we render into depends on if BLUR_AMOUNT is odd or even, so
 * the client should use the texture ID returned by this method.
 */
GLuint
Window::blur(const BlurShader & shader, const QuadMesh & quad)
{
    shader.program.use();
    shader.tapCountLoc.set1i(*numBlurTaps);
    auto isHorizontal = true;

    for (auto blurIdx = 0; blurIdx < BLUR_AMOUNT; blurIdx++)
    {
        // source --- isHorizontal must be used in such a way that we start with
        // guassianTextures[0], where our texture should have previously been
        // rendered into.
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gaussianTextures[!isHorizontal]);

        // destination --- swaps according to the value of isHorizontal
        glBindFramebuffer(GL_FRAMEBUFFER, gaussianFBO[isHorizontal]);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.isHorizontalLoc.set1b(isHorizontal);

        quad.draw();
        isHorizontal = !isHorizontal;
    }

    return gaussianTextures[!isHorizontal];
}

/*
 * Render our composite image, combining the original scene, and the bloom
 * effect texture. As we can't be sure which was the final texture that the
 * guassian blur process rendered into, this must be communicated to us by the
 * client.
 *
 * The resulting composite is rendered to the default framebuffer, using the
 * supplied QuadMesh.
 */
void
Window::renderComposite(const CompositeShader & shader,
    const GLuint bloomTexture, const QuadMesh & quad)
{
    shader.program.use();
    shader.toneMappingTechniqueLoc.set1i(*toneMappingTechnique);

    // source 1 --- the original scene
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fboTexture);

    // source 2 --- the blurred parts of the scene, used for the bloom effect
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, bloomTexture);

    // destination --- the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    quad.draw();
}
