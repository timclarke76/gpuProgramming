/*
 * glo/window.h
 *
 * Creates a window according to the client's specified configuration.
 *
 * Listeners may be added/removed for keyboard, mouse, and window events. A
 * listener model is also used for the loop event, allowing several objects to
 * be called independently if necessary.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <iostream>
#include <map>
#include <set>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "colour.h"
#include "projection.h"

#include "listeners/keyboard.h"
#include "listeners/loop.h"
#include "listeners/mouse.h"
#include "listeners/window.h"
#include "program/program.h"
#include "program/uniform.h"
#include "support/circular_deque.h"

class QuadMesh;

class Window : public WindowListener
{
  static constexpr auto BRIGHTNESS_THRESHOLD = 1.0f;
  static constexpr auto BLOOM_AMOUNT = 1.0f;
  static constexpr auto BLUR_AMOUNT = 16;
  static constexpr auto FPS = 1.0f/60.0f;

  public:
    // The configuration object for use by the client to specify window
    // requirements. Pre-populated with sensible defaults.
    typedef struct Config
    {
        // OpenGL version
        int majorVersion = 3;
        int minorVersion = 3;
        int profile = GLFW_OPENGL_CORE_PROFILE;

        // Initial viewport width and height
        GLuint viewportWidth = 800;
        GLuint viewportHeight = 600;

        bool resizable = true; // whether the window is resizeable
        std::string title; // the window's title string
        std::string backgroundColour = "000"; // the colour to use when clearing

        GLfloat fov = 45.0f; // the camera's FOV
        int samples = 0; // multisampling
        bool enableDebug = false; // enable/disable OpenGL debug output
        GLfloat pointSize = 0.0f; // set the program point size
        bool enableCursor = true; // should the mouse cursor be shown?
        bool enableStickyKeys = false; // should key presses be sticky?
        bool enableDepthTest = true;
        bool enableCullFace = true;
    } Config;

  private:
    // debug output in JSON format
    friend std::ostream & operator<<(std::ostream & os, const Window & obj);

    // These functions are called by OpenGL, and in turn use the windows
    // member variables to call any registered listeners according to the event
    // that occurred.
    friend void onKeyboardEvent(GLFWwindow * const window, const GLint key,
        const GLint scancode, const GLint action, const GLint mods);
    friend void onMouseMoveEvent(GLFWwindow * const window, const GLdouble xpos,
        const GLdouble ypos);
    friend void onMouseScrollEvent(GLFWwindow * const window,
        const GLdouble xpos, const GLdouble ypos);
    friend void onWindowFocusEvent(GLFWwindow * const window,
        const GLint focused);
    friend void onWindowSizeEvent(GLFWwindow * const window, const GLint width,
        const GLint height);

  public:
    Window(const Config & config);
    ~Window();

    // copying now allowed
    Window(const Window &) = delete;
    Window & operator=(const Window &) = delete;
    Window(Window &&) = delete;
    Window & operator=(Window &&) = delete;

    // The Window class registers itself as a WindowListener, and changes the
    // viewport dimensions when it is resized.
    virtual void onWindowSizeEvent(Window & window, const GLint width,
        const GLint height)
    {
        glViewport(0, 0, width, height);
        projection.setViewport(width, height);
        initialiseSceneFramebuffer();
        initialiseGaussianFramebuffers();
    }

    // The Window class doesn't care about whether or not is focused. We just
    // use glfwWindowAttrib if necessary.
    virtual void onWindowFocusEvent(Window &, const bool) {} // ignore

    // Accessors to the projection. Const and non-const methods.
    const Projection & getProjection() const { return projection; }
    Projection & getProjection() { return projection; }

    // Access the delta time since the last loop.
    GLfloat getDeltaTime() const { return deltaTime; }

    // True if the window has focus, false otherwise.
    bool isFocused() const { return glfwGetWindowAttrib(window, GLFW_FOCUSED); }

    // Mutates the cursor position.
    Window & setCursorPosition(const GLdouble x, const GLdouble y)
        { glfwSetCursorPos(window, x, y); return *this; }

    // Accessor for the mouse cursor x and y coordinates
    const Window & getCursorPosition(GLdouble & x, GLdouble & y) const
        { glfwGetCursorPos(window, &x, &y); return *this; }
    Window & getCursorPosition(GLdouble & x, GLdouble & y)
        { glfwGetCursorPos(window, &x, &y); return *this; }

    // Mutate and access whether or not the event loop should end.
    Window & setShouldClose(const bool shouldClose)
        { glfwSetWindowShouldClose(window, shouldClose); return *this; }
    bool getShouldClose() const { return glfwWindowShouldClose(window); }

    // Convenience method to discover if the specified key is pressed.
    bool isKeyPressed(const GLint key) const
        { return (glfwGetKey(window, key) == GLFW_PRESS) ? true : false; }

    // === REGISTER/REMOVE LISTENERS ===

    // keyboard
    Window & addKeyboardListener(KeyboardListener * const listener);
    Window & removeKeyboardListener(KeyboardListener * const listener);

    // loops
    Window & addLoopListener(LoopListener * const listener)
    {
        assert(listener != nullptr);
        loopListeners.insert(listener);
        return *this;
    }

    Window & removeLoopListener(LoopListener * const listener)
    {
        assert(listener != nullptr);
        loopListeners.erase(listener);
        return *this;
    }

    // mouse
    Window & addMouseListener(MouseListener * const listener);
    Window & removeMouseListener(MouseListener * const listener);

    // window
    Window & addWindowListener(WindowListener * const listener);
    Window & removeWindowListener(WindowListener * const listener);

    // The main loop. Returns when shouldClose is set to true.
    Window & Loop();

    Window & rotateToneMappingTechniqueLeft()
        { toneMappingTechnique.rotateLeft(); return *this; }
    Window & rotateToneMappingTechniqueRight()
        { toneMappingTechnique.rotateRight(); return *this; }
    GLuint getToneMappingTechnique() { return *toneMappingTechnique; }

    Window & rotateNumBlurTapsLeft()
        { numBlurTaps.rotateLeft(); return *this; }
    Window & rotateNumBlurTapsRight()
        { numBlurTaps.rotateRight(); return *this; }
    GLuint getNumBlurTaps() { return *numBlurTaps; }

  private:
    // === SHADERS ===
    // Each shader struct contains a program, and the uniform locations

    struct BrightPassShader
    {
        Program program;
        Uniform sceneTextureLoc;
        Uniform brightnessThresholdLoc;

        BrightPassShader() : program("composite", "bright_pass"),
            sceneTextureLoc(program, "sceneTexture"),
            brightnessThresholdLoc(program, "brightnessThreshold") {}
    };

    struct BlurShader
    {
        Program program;
        Uniform isHorizontalLoc;
        Uniform srcTextureLoc;
        Uniform tapCountLoc;

        BlurShader() : program("composite", "blur"),
            isHorizontalLoc(program, "isHorizontal"),
            srcTextureLoc(program, "srcTexture"),
            tapCountLoc(program, "tapCount") {}
    };

    struct CompositeShader
    {
        Program program;
        Uniform sceneTextureLoc;
        Uniform bloomTextureLoc;
        Uniform bloomStrengthLoc;
        Uniform toneMappingTechniqueLoc;

        CompositeShader() : program("composite"),
            sceneTextureLoc(program, "sceneTexture"),
            bloomTextureLoc(program, "bloomTexture"),
            bloomStrengthLoc(program, "bloomStrength"),
            toneMappingTechniqueLoc(program, "toneMappingTechnique") {}
    };

    void initialiseTexture(const GLuint id);
    void initialiseSceneFramebuffer();
    void initialiseGaussianFramebuffers();
    void renderScene();
    void brightPass(const BrightPassShader & shader, const QuadMesh & quad);
    GLuint blur(const BlurShader & shader, const QuadMesh & quad);
    void renderComposite(const CompositeShader & shader,
        const GLuint bloomTexture, const QuadMesh & quad);

    // Keep a map of GLFWwindow pointers to our Window class, allowing the
    // callback functions to access the correct Window's member variables.
    static std::map<GLFWwindow *, Window *> map;

    const Config config;
    GLFWwindow * window;
    Projection projection;
    Colour backgroundColour;

    // deltaTime is how long since the last loop occurred, mainly for use in
    // transformations
    GLfloat lastTime;
    GLfloat deltaTime;

    // our registered listeners, used by the callback functions
    std::set<KeyboardListener *> keyboardListeners;
    std::set<LoopListener *> loopListeners;
    std::set<MouseListener *> mouseListeners;
    std::set<WindowListener *> windowListeners;

    // === SCENE BUFFERS ===
    GLuint FBO = 0; // The framebuffer the scene is rendered into.
    GLuint fboTexture = 0; // The texture rendered into by the FBO.
    GLuint RBO = 0; // Used by OpenGL to perform depth testing.

    // === GAUSSIAN BLUR BUFFERS ===
    GLuint gaussianFBO[2] = {0};
    GLuint gaussianTextures[2] = {0};

    CircularDeque<GLuint> numBlurTaps;
    CircularDeque<GLuint> toneMappingTechnique;
};
