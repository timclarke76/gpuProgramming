/*
 * glo/listeners/mouse.h
 *
 * Interface called when mouse events occur.
 *
 * © Tim Clarke November 2025
 */

#pragma once

class Window;

class MouseListener
{
  public:
    virtual void onMouseMoveEvent(Window & window, const GLdouble xpos,
        const GLdouble ypos) = 0;
    virtual void onMouseScrollEvent(Window & window, const GLdouble xpos,
        const GLdouble ypos) = 0;
};
