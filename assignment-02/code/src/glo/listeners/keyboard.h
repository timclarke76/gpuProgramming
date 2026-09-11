/*
 * glo/listeners/keyboard.h
 *
 * Interface called when keyboard events occur.
 *
 * © Tim Clarke November 2025
 */

#pragma once

class Window;

class KeyboardListener
{
  public:
    virtual void onKeyboardEvent(Window & window, const GLint key,
        const GLint scancode, const GLint action, const GLint mods) = 0;
};
