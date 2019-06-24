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
 *      Interface representing a digital output pin.
 */
class DigitalOutputPin {
public:
    /**
     * @brief
     *      Set the pin output value.
     *
     * @param value
     *      `true` for high, `false` for low.
     */
    virtual void set(bool value) = 0;

    /**
     * @brief
     *      Schedule a pulse in the future.
     *
     * @param value
     *      `true` for high, `false` for low.
     *
     * @param start
     *      Start time in timer ticks
     *
     * @param duration
     *      Duration of the pulse in timer ticks
     */
    virtual void set(bool value, uint16_t start, uint16_t duration) = 0;
};

/**
 * @brief
 *      Interface representing a digital input pin.
 */
class DigitalInputPin {
public:
    enum Level {
        FALLING, // for when the pin goes from high to low
        RISING // to trigger when the pin goes from low to high
    };

    struct Listener {
        virtual void on_pin_change(DigitalInputPin* pin, uint16_t time) = 0;
    };

    /**
     * @brief
     *      Read and return the value of the pin.
     *
     * @return
     *      `true` if high, `false` if low.
     */
    virtual bool read() = 0;

    /**
     * @brief
     *      Listen for a change on this pin.
     *
     * @param level
     *      Type of change to listen for.
     *
     * @param listener
     *      Listener implementation to call when the change is detected. The call will
     *      be done from a non-interrupt context.
     */
    virtual void attach_interrupt(Level level, Listener* listener) = 0;
};

/**
 * @brief
 *      Interface representing an analog input pin.
 */
class AnalogInputPin {
public:
    struct Listener {
        virtual void on_analog_pin_reading_complete(uint16_t value) = 0;
    };

    /**
     * @brief
     *      Return the latest analog reading of a pin.
     *
     * @return
     *      The analog value from 0 to 65535.
     *
     * This method is non-blocking. Platforms implementing this class are
     * in charge of reading the analog pin in the background.
     */
    virtual uint16_t get() = 0;

    /**
     * @brief Register a listener to receive asynchronous analog readings.
     *
     * @param listener
     *      User callback
     */
    virtual void listen(Listener* listener) = 0;
};
