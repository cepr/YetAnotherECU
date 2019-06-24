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
#include <stdint.h>

/**
 * @brief Abstract class representing one injector channel.
 */
class Injector {
    /**
     * @brief Activate or deactivate the generation of the injector signal.
     *
     * @param enable
     *      `true` to enable, `false` to disable.
     */
    virtual void enable(bool enabled) = 0;

    /**
     * @brief Set the injector pulse timing.
     *
     * @param start
     *      Start time relative to the board timer and the begining of the cycle.
     *
     * @param duration
     *      Duration in units of the board timer.
     */
    virtual void set(uint16_t start, uint16_t duration) = 0;
};
