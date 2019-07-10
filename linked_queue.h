/*
 * YetAnotherECU
 * Copyright (C) 2019  Cedric Priscal
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

/**
 * Linked queue of elements.
 *
 * The queue element type must contain an attribute "next_element", pointing to
 * the next element of the list. Don't access this attribute directly.
 *
 * The list is a FIFO. The elements are queued using the `add()` method, and
 * remove the oldest element by calling `remove()`.
 *
 * Synchronized is a class that needs to disable the platform interrupts in its
 * constructor and re-enable them in its destructor. You can pass an empty class
 * if you don't plan on calling `add()` or `remove()` from an interrupt context.
 */
template<class Element, class Synchronized>
class LinkedQueue {
public:
    /**
     * @brief
     *      Default constructor.
     */
    LinkedQueue<Element, Synchronized>() :
        first(0),
        last(0)
    {
    }

    /**
     * @brief
     *      Add an element to the list.
     *
     * @param element
     *      Pointer to the element to append.
     */
    void add(Element* element) {
        if ((element->next_element != 0) || (element == last)) {
            // Element is already in a queue, ignore
            return;
        }
        Synchronized::lock();
        if (last) {
            // Non-empty list
            last->next_element = element;
        } else {
            // Empty list
            first = element;
        }
        last = element;
        Synchronized::unlock();
    }

    /**
     * @brief
     *      Remove the oldest element from the list.
     *
     * @return
     *      A pointer to the removed element, or 0 if the queue was empty.
     */
    Element* remove() {
        Synchronized::lock();
        Element* ret = first;
        if (ret) {
            // Non-empty list
            first = ret->next_element;
            if (!first) {
                // It was the last element
                last = 0;
            }
            // `first` is not part of the queue anymore, clear its pointer
            ret->next_element = 0;
        }
        Synchronized::unlock();
        return ret;
    }

protected:
    Element* first;
    Element* last;
};

class NotSynchronized {
public:
    static inline void lock() {
    }
    static inline void unlock() {
    }
};
