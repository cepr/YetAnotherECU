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
 * @brief
 *      Abstract class for rotation sensors.
 *
 * A rotation sensor measures the angle of a shaft and estimates
 * the time of when the shaft will be at a particular angle.
 *
 * The angle unit used by this class is 65536 units per revolution.
 */
class RotationSensor {
public:
    /**
     * @brief
     *      Calculate the estimated instant at which the shaft will be at the specfied angle.
     *
     * @param angle
     *      Angle between 0 and 65535
     *
     * @return
     *      Time
     */
    virtual uint16_t estimate_time_for_angle(uint16_t angle) = 0;

    struct Listener {
        virtual void on_rotation_sensor_updated() = 0;
    };

    /**
     * @brief Set the listener
     *
     * @param listener
     *      User callback
     *
     * The listener will be called everytime the angle has been determined accurately.
     * In the case of a trigger wheel, this happens when the hall sensor detects a tooth.
     */
    void set_listener(Listener* listener)
    {
        this->listener = listener;
    }

protected:
    Listener* listener;
};
