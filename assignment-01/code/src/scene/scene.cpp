/*
 * scene/scene.cpp
 *
 * © Tim Clarke November 2025
 */

#include "scene.h"

#include <sstream>

/*
 * Our structures are responsible for program and graph construction.
 */
Scene::Scene(Window & window)
    : orbs(geometricFactory),
      mainLight(orbs, geometricFactory),
      objects(window, orbs.graph, mainLight.graph, geometricFactory),
      cursor(window)
{
    Camera::instance().initialise({0.75f, 1.25f, 5.0f}, {0.0f, 1.0f, 0.0f});

    /*
     * Initialise both programs with the perspective, and for the objects
     * program, the texture location.
     */
    const auto perspective = window.getProjection().getPerspective();

    orbs.program.use();
    locs.orbsProjection.setMat4f(perspective);

    objects.program.use();
    locs.objectsProjection.setMat4f(perspective);
    locs.objectsTexture.set1i(0);

    /*
     * Switch off the main light initially, to best show off the orbs as they
     * float upwards.
     */
    objects.graph.toggleMainLights();
}

/*
 * Respond to a user's keyboard event.
 *
 * WASD to move the camera along the x and z axis, R and F to move up and down
 * the y axis.
 *
 * Keypad to move the main light. Keypad 5 to reset its position.
 *
 * F5: toggle Blinn Phong Specular.
 *
 * L: toggle the main light off and on.
 *
 * O: toggle the orbs' animation, SHIFT+O to toggle their visibility and light.
 *
 * M: toggle the mobile's rotation, while keeping the ornaments' spin.
 *
 * P: toggle all of the mobile's animation, including the ornaments.
 *
 * T: toggle the torch on and off.
 *
 * [ and ]: adjust the torch's beam width.
 *
 * keypad + and -: speed up and slow down all animation.
 *
 * ESC: close the window, ending the program.
 */
void
Scene::onKeyboardEvent(Window & window, const int key, const int scancode,
    const int action, const int mods)
{
    const auto speed = (2.5f * window.getDeltaTime());
    GLfloat x = 0.0f, y = 0.0f, z = 0.0f;

    if (window.isKeyPressed(GLFW_KEY_A) == GLFW_TRUE) x -= speed;
    if (window.isKeyPressed(GLFW_KEY_D) == GLFW_TRUE) x += speed;
    if (window.isKeyPressed(GLFW_KEY_R) == GLFW_TRUE) y += speed;
    if (window.isKeyPressed(GLFW_KEY_F) == GLFW_TRUE) y -= speed;
    if (window.isKeyPressed(GLFW_KEY_W) == GLFW_TRUE) z -= speed;
    if (window.isKeyPressed(GLFW_KEY_S) == GLFW_TRUE) z += speed;

    Camera::instance().translate(x, y, z);

    x = y = z = 0.0f;

    if (window.isKeyPressed(GLFW_KEY_KP_4) == GLFW_TRUE) x -= speed;
    if (window.isKeyPressed(GLFW_KEY_KP_6) == GLFW_TRUE) x += speed;
    if (window.isKeyPressed(GLFW_KEY_KP_9) == GLFW_TRUE) y += speed;
    if (window.isKeyPressed(GLFW_KEY_KP_3) == GLFW_TRUE) y -= speed;
    if (window.isKeyPressed(GLFW_KEY_KP_8) == GLFW_TRUE) z -= speed;
    if (window.isKeyPressed(GLFW_KEY_KP_2) == GLFW_TRUE) z += speed;

    mainLight.graph.translate({x, y, z});

    if (action != GLFW_PRESS) return;

    switch (key)
    {
        case GLFW_KEY_KP_5: mainLight.graph.resetTranslation(); break;
        case GLFW_KEY_F5: objects.graph.toggleBlinnPhongSpecular(); break;
        case GLFW_KEY_L: objects.graph.toggleMainLights(); break;

        case GLFW_KEY_O:
        {
            if (window.isKeyPressed(GLFW_KEY_LEFT_SHIFT) ||
                window.isKeyPressed(GLFW_KEY_RIGHT_SHIFT))
            {
                objects.graph.toggleOrbs();
            }
            else
            {
                orbs.graph.toggleAnimation();
            }
        }
        break;

        case GLFW_KEY_M: objects.graph.toggleMobileRotation(); break;
        case GLFW_KEY_P: objects.graph.toggleAnimation(); break;
        case GLFW_KEY_T: objects.graph.getTorch().togglePower(); break;

        case GLFW_KEY_LEFT_BRACKET:
        {
            objects.graph.getTorch().adjustBeam(-0.05f);
        }
        break;

        case GLFW_KEY_RIGHT_BRACKET:
        {
            objects.graph.getTorch().adjustBeam(0.05f);
        }
        break;

        case GLFW_KEY_KP_ADD: animationSpeed += 0.5f; break;

        case GLFW_KEY_KP_SUBTRACT:
        {
            animationSpeed = std::max(0.0f, animationSpeed - 0.5f);
        }
        break;

        case GLFW_KEY_ESCAPE: window.setShouldClose(true); break;
    }
}

/*
 * Adjust the camera's rotation, according to the mouse movement. We ignore the
 * first movement event after the window has gained focus, to avoid jumps.
 *
 * All mouse movement is ignored when the window does not have focus.
 */
void
Scene::onMouseMoveEvent(Window & window, const double xpos, const double ypos)
{
    if (!window.isFocused()) return;

    if (cursor.isFirstEvent)
    {
        cursor.position = {xpos, ypos};
        cursor.isFirstEvent = false;
    }
    else
    {
        const auto delta =
            glm::vec2((xpos - cursor.position.x), (cursor.position.y - ypos));
        Camera::instance().rotate(delta * MOUSE_SENSITIVITY);

        cursor.position = {xpos, ypos};
    }
}

/*
 * Cycle through the torch lens colours when the mouse wheel is scrolled, if the
 * window has focus.
 */
void
Scene::onMouseScrollEvent(Window & window, const double xoffset,
    const double yoffset)
{
    if (!window.isFocused()) return;
    objects.graph.getTorch().rotateLensColour(yoffset);
}

/*
 * Called from the main event loop. We display the orbs and main light according
 * to whether or not the user has enabled/disabled them. Objects are always
 * shown, and the objects graph needs the orbs graph and main light graph so
 * that it can pass the lights' properties the shaders.
 */
void
Scene::onLoopEvent(Window & window)
{
    // Adjust the delta time to speed up or slow down the animations,
    // according to animationSpeed (adjusted using the keypad + and - keys).
    const auto deltaTime = animationSpeed * window.getDeltaTime();

    // The user can opt not to show the orbs.
    if (objects.graph.getShowOrbs())
    {
        orbs.program.use();
        orbs.graph.render(deltaTime, Camera::instance());
    }

    // The user can opt not to show the main light.
    if (objects.graph.getShowMainLights())
    {
        mainLight.program.use();
        mainLight.graph.render(deltaTime, Camera::instance());
    }

    // The objects are always shown.
    objects.program.use();
    objects.graph.render(deltaTime, Camera::instance(), orbs.graph,
        mainLight.graph);
}

/*
 * Update the perspective for both programs, when the window changes size.
 */
void
Scene::onWindowSizeEvent(Window & window, const GLint width,
    const GLint height)
{
    const auto perspective = window.getProjection().getPerspective();

    orbs.program.use();
    locs.orbsProjection.setMat4f(perspective);

    objects.program.use();
    locs.objectsProjection.setMat4f(perspective);
}
