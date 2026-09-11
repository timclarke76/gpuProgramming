/*
 * scene/support/circular_deque.h
 *
 * A simple circular deque, allowing the client to "rotate" the queue, moving
 * items from the front to the back, or from the back to the front.
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <deque>

template <typename T> class CircularDeque
{
  public:
    CircularDeque<T> & pushBack(const T & value)
    {
        deque.push_back(value);
        return *this;
    }

    /*
     * Move the front item to the back.
     */
    CircularDeque<T> & rotateLeft()
    {
        if (size() > 1)
        {
            T front = deque.front();
            deque.pop_front();
            deque.push_back(front);
        }

        return *this;
    }

    /*
     * Move the back item to the front.
     */
    CircularDeque<T> & rotateRight()
    {
        if (size() > 1)
        {
            T back = deque.back();
            deque.pop_back();
            deque.push_front(back);
        }

        return *this;
    }

    /*
     * If the direction is positive, rotate left. If negative, rotate right. If
     * zero, do not rotate.
     */
    CircularDeque<T> & rotate(const signed short direction)
    {
        if (direction < 0) rotateLeft();
        else if (direction > 0) rotateRight();
        return *this;
    }

    T & front() const { return deque.front(); }
    T & back() const { return deque.back(); }
    size_t size() const { return deque.size(); }

    // override the dereference operator to return the front item
    T & operator*() { return deque.front(); }

  private:
    std::deque<T> deque;
};
