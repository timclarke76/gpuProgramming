/*
 * glo/listeners/window.h
 *
 * Interface called when window events occur (resize or focus).
 *
 * © Tim Clarke November 2025
 */

#pragma once

class Window;

class WindowListener
{
  public:
    virtual void onWindowSizeEvent(Window & window, const GLint width,
        const GLint height) = 0;
    virtual void onWindowFocusEvent(Window & window, const bool isFocused) = 0;
};
