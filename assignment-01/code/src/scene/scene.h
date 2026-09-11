/*
 * scene/scene.h
 *
 * The main application class, containing two Programs and three graphs:
 *   1  the orbs program uses lights.vert and lights.frag, and is used to render
 *      two graphs --- the orbs graph (the floating orb lights) and the
 *      mainlight graph (the "room" light).
 *   2  the objects program uses objects.vert and objects.frag, and is used to
 *      render the objects graph, which contains the cot mobile.
 *
 * For ease of maintenance, these member variables are kept in three structures:
 *   1  Orbs orbs
 *   2  MainLight mainLight
 *   3  Objects objects
 *
 * Each structure contains
 *   1  Program program
 *   2  Graph graph (where Graph is OrbsGraph, MainLightGraph, or ObjectsGraph)
 *
 * For efficiency, mainLight.program is a reference to orbs.program.
 *
 * The UniformLocs structure holds locations for the projection value in both
 * programs, and the texture value in orbs.program.
 *
 * The Cursor structure holds values necessary to move the camera rotation with
 * the mouse cursor:
 *   1  isFirstEvent records if this is the first event _since the window gained
 *      focus_. The allows us to avoid camera jumps by ignoring the first mouse
 *      movement event.
 *   2  position records the last known cursor position, so we can determine how
 *      much it was moved, and thus how much the camera should rotate and in
 *      which direction.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <utility>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../glo/camera.h"
#include "../glo/listeners/keyboard.h"
#include "../glo/listeners/loop.h"
#include "../glo/listeners/mouse.h"
#include "../glo/listeners/window.h"
#include "../glo/program/program.h"
#include "../glo/program/uniform.h"
#include "../glo/scene/factories/geometric.h"
#include "../glo/window.h"
#include "graphs/main_light.h"
#include "graphs/objects.h"
#include "graphs/orbs.h"

class Scene : public KeyboardListener, public LoopListener,
    public MouseListener, public WindowListener
{
  private:
    static constexpr GLfloat MOUSE_SENSITIVITY = 0.1f;

    /*
     * The program and graph for our floating orbs.
     */
    struct Orbs
    {
        Program program;
        OrbsGraph graph;

        Orbs(GeometricFactory & geometricFactory)
            : program("lights"),
              graph(program, geometricFactory) {}
    };

    /*
     * The program and graph for our main room light. Note that program is
     * simply a reference to Orbs.program.
     */
    struct MainLight
    {
        Program & program;
        MainLightGraph graph;

        MainLight(Orbs & orbs, GeometricFactory & geometricFactory)
            : program(orbs.program),
              graph(program, geometricFactory) {}
    };

    /*
     * The program and graph for the cot mobile.
     */
    struct Objects
    {
        Program program;
        ObjectsGraph graph;

        Objects(Window & window, OrbsGraph & orbsGraph,
            MainLightGraph & mainLightGraph,
            GeometricFactory & geometricFactory)
            : program("objects"),
              graph(program, geometricFactory, orbsGraph, mainLightGraph) {}
    };

    /*
     * The uniform locations we need to update the shaders.
     */
    struct UniformLocs
    {
        Uniform orbsProjection;
        Uniform objectsProjection;
        Uniform objectsTexture;

        // clang-format off
        UniformLocs(Program & orbsProgram, Program & objectsProgram) :
            orbsProjection(orbsProgram.use(), "u_Projection"),
            objectsProjection(objectsProgram.use(), "u_Projection"),
            objectsTexture(objectsProgram, "u_Texture") {}
        // clang-format on
    };

    /*
     * Mouse cursor variables for smooth camera rotation with the mouse.
     */
    struct Cursor
    {
        bool isFirstEvent = true;
        glm::vec2 position;

        // clang-format off
        Cursor(const Window & window) :
            position((window.getProjection().getWidth() / 2),
                (window.getProjection().getHeight() / 2)) {}
        // clang-format on
    };

  public:
    Scene(Window & window);

    // === LISTENER METHODS ===

    void onKeyboardEvent(Window & window, const int key, const int scancode,
        const int action, const int mods);
    void onMouseMoveEvent(Window & window, const double xpos,
        const double ypos);
    void onMouseScrollEvent(Window & window, const double xpos,
        const double ypos);
    virtual void onLoopEvent(Window & window) override;
    virtual void onWindowSizeEvent(Window & window, const GLint width,
        const GLint height) override;

    virtual void onWindowFocusEvent(Window & window,
        const bool isFocused) override
    {
        // ignore the next mouse movement event, to avoid camera jumping
        cursor.isFirstEvent = true;
    }

  private:
    // we don't use this directly, but constructing it here allows maximum
    // reuse, and gives us the option to use alternative texture folders
    GeometricFactory geometricFactory = GeometricFactory("textures/");

    Orbs orbs;
    MainLight mainLight;
    Objects objects;
    // TODO: uniform locations should be handled by specialised program classes
    UniformLocs locs = UniformLocs(orbs.program, objects.program);
    Cursor cursor;

    // The user may choose to speed up, or slow down, the animations.
    GLfloat animationSpeed = 1.0f;
};
