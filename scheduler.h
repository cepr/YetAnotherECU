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
#include "task.h"
#include "linked_queue.h"

/**
 * @brief
 *      Task scheduler
 *
 * This is a cooperative multi-tasking scheduler with two queue of execution.
 * It keeps running each registered task in a loop.
 *
 * Limitations from the LinkedQueue implementation:
 * - a Task can be registered in only one Scheduler
 * - a Task cannot be registered multiple times
 */
template <class Synchronized>
class Scheduler
{
public:
    /**
     * @brief
     *      Schedule a background task
     *
     * @param task
     *      Pointer to the task to execute
     *
     * @note
     *      Background tasks are executed repeatedly
     */
    inline void add_background_task(Task* task)
    {
        background_tasks.add(task);
    }

    /**
     * @brief
     *      Schedule a high-priority task
     *
     * @param task
     *      Pointer to the task to execute
     *
     * The task will be executed as soon as all other
     * tasks scheduled with this method have completed.
     *
     * Interrupt tasks are single shot.
     *
     * You should typically call this method from the
     * interrupt vector.
     */
    inline void add_interrupt_task(Task* task)
    {
        interrupt_tasks.add(task);
    }

    /**
     * @brief
     *      Execute all pending interrupt tasks and one background task.
     *
     * Make sure to call this method periodically. On Arduino, call
     * this method from the main loop() function.
     *
     * @return
     *      `true` if something was executed, `false` otherwise.
     */
    bool execute()
    {
        Task* task;
        bool busy = false;

        // Execute all interrupt tasks
        while ((task = interrupt_tasks.remove()) != 0) {
            task->execute();
            busy = true;
        }

        // Execute one background task
        task = background_tasks.remove();
        if (task) {
            task->execute();
            busy = true;
            // Put back the task
            background_tasks.add(task);
        }

        return busy;
    }

protected:
    LinkedQueue<Task, Synchronized> interrupt_tasks;
    LinkedQueue<Task, NotSynchronized> background_tasks;
};
