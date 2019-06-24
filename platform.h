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
 * @brief Platform interface.
 *
 * Each platform must implement this interface.
 */
class Platform {
public:
    /**
     * @brief Return the current time.
     *
     * @return Current time in timer ticks
     */
    virtual uint16_t now() = 0;

    /**
     * @brief Return a digital output pin
     *
     * @param id
     *      Pin number, platform dependent
     *
     * @param value
     *      Default value of the pin
     *
     * @return
     *      A pointer to a DigitalOutputPin instance, or `null` if the pin is not available.
     */
    virtual DigitalOutputPin* get_digital_output_pin(int id, bool value) = 0;

    /**
     * @brief Return a digital input pin
     *
     * @param id
     *      Pin number, platform dependent
     *
     * @param pull_up
     *      `true` to enable an internal pull-up, `false` otherwise.
     *      This feature may not be available on all platforms.
     *
     * @return
     *      A pointer to a DigitalInputPin instance, or `null` if the pin is not available.
     */
    virtual DigitalInputPin* get_digital_input_pin(int id, bool pull_up) = 0;

    /**
     * @brief Return an analog input pin
     *
     * @param id
     *      Pin number, platform dependent
     *
     * @param value
     *      Default value of the pin
     *
     * @return
     *      A pointer to a DigitalOutputPin instance, or `null` if the pin doesn't exist.
     */
    virtual AnalogInputPin* get_analog_input_pin(int id, uint32_t refresh_interval) = 0;

    struct EventHandler {
        virtual void on_event(int id, uint16_t time) = 0;
    };

    /**
     * @brief Schedule an event at a given time.
     *
     * @param id
     *      Event id
     *
     * @param time
     *      When to generate the event
     *
     * @param event_handler
     *      Event handler
     *
     * A platform can support only a limited number of scheduled events.
     * Each scheduled event must be given a different id.
     * Using the same id will replace the previous scheduling.
     * The events will be handled in a non-interrupt context.
     */
    virtual void schedule_event(int id, uint16_t time, EventHandler* event_handler) = 0;

};
