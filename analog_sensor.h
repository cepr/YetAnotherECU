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
 * @brief Abstract class representing a single-channel analog sensor.
 */
class AnalogSensor {
    AnalogSensor() :
        listener(0)
    {
    }

    /**
     * @brief Get the latest sensor reading.
     *
     * @return The latest reading of the sensor.
     *
     * This method is non-blocking. It simply returns the latest reading, or
     * 0 if the sensor has never been enabled before.
     *
     * For performance reasons, the return type is an integer. The physical
     * unit and range depends on the sensor type.
     */
    virtual uint16_t get() = 0;

    struct Listener {
        virtual void on_analog_sensor_value_change(uint16_t value) = 0;
    };

    /**
     * @brief
     *      Register a listener to monitor the sensor.
     *
     * @param listener
     *      Pointer to an instance of `AnalogSensor::Listener`
     */
    void register_listener(Listener* listener)
    {
        this->listener = listener;
    }

protected:
    Listener* listener;
};
