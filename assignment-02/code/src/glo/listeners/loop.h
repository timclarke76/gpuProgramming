/*
 * glo/listeners/loop.h
 *
 * Interface called on each loop.
 *
 * © Tim Clarke November 2025
 */

#pragma once

class Window;

class LoopListener
{
  public:
    virtual void onLoopEvent(Window & window) = 0;
};
