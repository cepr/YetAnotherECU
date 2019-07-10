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
 * @brief Task interface
 *
 * The system is a cooperative multi-tasking system.
 * Each task must implement this interface, and its
 * `loop` method will be called periodically.
 *
 * Tasks must not wait inside a `loop` function, otherwise
 * all other tasks will be blocked as well.
 */
class Task {
public:
    /**
     * @brief
     *      Default constructor.
     */
    Task() :
        next_element(0)
    {
    }

    /**
     * @brief Execute the task
     */
    virtual void execute() = 0;

    // LinkedQueue
    class Task* next_element;
};
