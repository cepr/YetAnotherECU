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
#include "pin.h"

/**
 * @brief
 *      Class representing one ignition coil channel.
 */
class Coil {
public:
    /**
     * @brief
     *      Constructor.
     *
     * @param pin
     *      Digital output pin the coil is connected to.
     */
    Coil(DigitalOutputPin *pin, uint16_t pulse_duration) :
        pin(pin),
        pulse_duration(pulse_duration)
    {
    }

    /**
     * @brief Set the ignition spark angle.
     *
     * @param time
     *      Time relative to the board timer and the begining of the cycle.
     */
    void set_spark_time(uint16_t time)
    {
        pin->set(true, time - pulse_duration, pulse_duration);
    }

protected:
    DigitalOutputPin *pin;
    uint16_t pulse_duration;
};
