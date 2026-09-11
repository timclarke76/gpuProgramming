/*
 * main.cpp
 *
 * The entry point for our program. Simply responsible for providing program
 * usage instructions to the user, initialising OpenGL, creating our window and
 * main scene, and starting the loop.
 *
 * © Tim Clarke November 2025
 */

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glo/window.h"

#include "scene/scene.h"

#define BOLD "\033[1m"
#define ITALIC "\033[3m"
#define UNDERLINE "\033[4m"
#define MAGENTA "\033[35m"
#define RED "\033[31m"
#define CYAN "\033[36m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define RESET "\033[0m"

/*
 * Echo clear program usage instructions to stdout.
 */
// clang-format off
void echoControls()
{
    std::cout << BOLD << UNDERLINE << "\n=== COT MOBILE ===\n\n" << RESET;
    std::cout << ITALIC << "Instructions for use.\n\n" << RESET;
    std::cout << ITALIC << "The train was clearly made by the child,"
        << " and he likes it!" << RESET << " 🚂 🤷‍♂️\n\n";

    auto COL = MAGENTA;
    std::cout << BOLD << UNDERLINE << "Movement" << RESET << ": "
        << COL << "WASD" << RESET " keys for movement. "
        << COL << "R" << RESET << " and " << COL << "F" << RESET
        << " to move up and down. Move the " << COL << "mouse" << RESET
        << "\n          to change camera rotation.\n\n";

    COL = RED;
    std::cout << BOLD << UNDERLINE << "Torch" << RESET << ": "
        << COL << "T" << RESET << " to toggle the torch's power, "
        << COL << "[" << RESET << " and " << COL << "]" << RESET
        << " to tighten and widen the beam.\n"
        << "       Use the mouse " << COL << "scroll wheel" << RESET
        << " to rotate through the different coloured\n"
        << "       lens filters.\n\n";

    COL = CYAN;
    std::cout << BOLD << UNDERLINE << "Main Light" << RESET << ": "
        << COL << "L" << RESET " switches the main light off and on."
        << " The " << COL << "keypad" << RESET << " numbers\n            "
        << COL << "8" << RESET ", "
        << COL << "4" << RESET ", "
        << COL << "6" << RESET ", and "
        << COL << "2" << RESET
        << " can be used to move the main light along the\n"
        << "            x and z axis. "
        << COL << "9" << RESET " and "
        << COL << "3" << RESET
        << " will move it up and down the y axis.\n"
        << COL << "            Keypad 5" << RESET
        << " will reset the light to its initial location of\n"
        << "            {-2.0f, 2.0f, 2.0f}.\n\n";

    COL = YELLOW;
    std::cout << BOLD << UNDERLINE << "Orbs" << RESET << ": "
        << COL << "O" << RESET " pauses the animation of the orbs."
        << COL << " Shift+O" << RESET " toggles their\n"
        << "      visibility (and their light) entirely.\n\n";

    COL = GREEN;
    std::cout << BOLD << UNDERLINE << "Mobile" << RESET << ": "
        << COL << "M" << RESET << " pauses the rotation of the"
        << " cot mobile. " << COL << "P" << RESET << " pauses the\n"
        << "        rotation of the mobile, the ornaments, and the cradle.\n\n";

    COL = BLUE;
    std::cout << BOLD << UNDERLINE << "Animation" << RESET << ": "
        << COL << "Keypad +" << RESET << " and " << COL << "-" << RESET
        << " adjusts all animation speeds.\n\n";

    COL = MAGENTA;
    std::cout << BOLD << UNDERLINE << "Particles" << RESET << ": "
        << COL << "E" << RESET << " pauses the particle emission.\n\n";

    COL = RED;
    std::cout << BOLD << UNDERLINE << "Phong Specular" << RESET << ": "
        << COL << "F5" << RESET << " switches between Blinn Phong Specular"
        << " and Phong\n                Specular.\n\n";

    COL = CYAN;
    std::cout << BOLD << UNDERLINE << "Bloom" << RESET << ": "
        << COL << "F6" << RESET << " and " << COL << "Shift+F6" << RESET
        << " adjusts bloom level (number of Gaussian blur taps).\n\n";

    COL = YELLOW;
    std::cout << BOLD << UNDERLINE << "Tone Mapping" << RESET << ": "
        << COL << "F7" << RESET << " and " << COL << "Shift+F7" << RESET
        << " cycles between tone mapping techniques.\n\n";

    COL = GREEN;
    std::cout << BOLD << UNDERLINE << "Normal Mapping" << RESET << ": "
        << COL << "F8" << RESET << " toggles off and on normal mapping.\n\n";

    COL = BLUE;
    std::cout << BOLD << UNDERLINE << "Exit" << RESET << ": "
        << COL << "Escape" << RESET << " exits the program.\n\n";
}
// clang-format on

int main(void)
{
    if (!glfwInit()) throw std::runtime_error("Failed to initialise GLFW");

    {
        echoControls();

        Window::Config config;
        config.title = "Cot Mobile";
        config.backgroundColour = "111"; // dark grey background
        config.enableCursor = false;     // don't display the cursor
        config.samples = 8;              // anti-aliasing

        Window window(config);
        Scene scene(window);

        // Add the scene as a listener here, so it can be safely removed again
        // when the loop ends (keep add/remove together).
        window.addKeyboardListener(&scene)
            .addLoopListener(&scene)
            .addMouseListener(&scene)
            .addWindowListener(&scene);

        window.Loop();

        window.removeKeyboardListener(&scene)
            .removeLoopListener(&scene)
            .removeMouseListener(&scene)
            .removeWindowListener(&scene);
    }

    // The window is out of scope, and destroyed. It's safe to terminate glfw.
    glfwTerminate();

    return 0;
}

/*
 * We don't really need to do this here as this file is never included. But keep
 * it clean anyway.
 */
#undef BOLD
#undef ITALIC
#undef UNDERLINE
#undef MAGENTA
#undef RED
#undef CYAN
#undef YELLOW
#undef GREEN
#undef BLUE
#undef RESET
