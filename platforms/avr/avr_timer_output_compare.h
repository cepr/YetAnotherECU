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
#include "../../pin.h"
#include "../../task.h"
#include <avr/io.h>

/**
 * @brief AVR hardware pulse generator.
 *
 * Pulse generator using the AVR hardware timer in "Compare Output Mode, non-PWM".
 */
class AvrTimerOutputCompare: public DigitalOutputPin, public Task {
protected:
public:

    virtual void set(bool value)
    {
        set_pin_value(value);
        if (mode != MODE_GPIO) {
            // Switch port to normal operation
            disable_output_compare_interrupt();
            set_compare_output_mode(0);
            set_pin_direction(true);
            mode = MODE_GPIO;
        }
    }

    virtual void set(bool value, uint16_t start, uint16_t duration)
    {
        this->pulse_value = value;
        this->pulse_start = start;
        this->pulse_duration = duration;
        execute();
        if (mode != MODE_TIMER) {
            // Initialize the output compare unit
            enable_output_compare_interrupt();
            set_pin_direction(true);
            mode = MODE_TIMER;
        }
    }

protected:
    // Constructor
    AvrTimerOutputCompare() :
        mode(MODE_DEFAULT),
        pulse_value(false),
        pulse_start(0),
        pulse_duration(0)
    {
    }

    // Task implementation
    // This method is called when:
    // - the pulse parameters have changed (start, value, duration)
    // - the timer compare match interrupt happened
    void execute()
    {
        // Output compare mode
        uint16_t now = get_timer_counter();
        uint16_t x = now - pulse_start;
        if (x < pulse_duration) {
            // We are currently inside the pulse, program the end of the pulse
            set_compare_output_mode(pulse_value ? 2 : 3);
            set_output_compare_register(pulse_start + pulse_duration);
        } else {
            // Outside of the pulse, we must program the next pulse
            set_compare_output_mode(pulse_value ? 3 : 2);
            set_output_compare_register(pulse_start);
        }
    }

    //
    virtual void enable_output_compare_interrupt() = 0;
    virtual void disable_output_compare_interrupt() = 0;

    // Set the bit in the DDR register (1: out, 0: in)
    virtual void set_pin_direction(bool value) = 0;

    // Set the bit in the PORT register (1: high, 0: low)
    virtual void set_pin_value(bool value) = 0;

    // Get register TCNTn
    virtual uint16_t get_timer_counter() = 0;

    // Set register COMn[A,B,C]
    // 0: Normal port operation
    // 1: Toggle on compare match
    // 2: Clear on compare match
    // 3: Set on compare match
    virtual void set_compare_output_mode(uint8_t compare_output_mode) = 0;

    // Set register OCR{1,3,4}{A,B,C}
    virtual void set_output_compare_register(uint16_t value) = 0;

    // Attributes
    enum {
        MODE_DEFAULT,
        MODE_GPIO,
        MODE_TIMER
    } mode;
    bool pulse_value;
    uint16_t pulse_start;
    uint16_t pulse_duration;
};
