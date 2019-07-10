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
        if (state != STATE_CONTINUOUS) {
            // Switch port to normal operation
            disable_output_compare_interrupt();
            set_compare_output_mode(0);
            set_pin_direction(true);
            state = STATE_CONTINUOUS;
        }
    }

    virtual void set(bool value, uint16_t start, uint16_t duration)
    {
        this->pulse_value = value;
        this->pulse_end = start + duration;
        this->state = STATE_WAIT_FOR_PULSE_START;
        set_compare_output_mode(pulse_value ? 3 : 2);
        set_output_compare_register(start);
        enable_output_compare_interrupt();
        set_pin_direction(true); // set as output
    }

protected:
    // Constructor
    AvrTimerOutputCompare() :
        state(STATE_IDLE),
        pulse_value(false),
        pulse_end(0)
    {
    }

    // Task implementation
    // This method is called when:
    // - it's time to start the pulse
    // - it's time to end the pulse
    void execute()
    {
        if (state == STATE_WAIT_FOR_PULSE_START) {
            set_compare_output_mode(pulse_value ? 2 : 3);
            set_output_compare_register(pulse_end);
        } else if (state == STATE_WAIT_FOR_PULSE_END) {
            // Switch port to normal operation
            disable_output_compare_interrupt();
            set_compare_output_mode(0);
            state = STATE_IDLE;
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
        STATE_IDLE,
        STATE_CONTINUOUS,
        STATE_WAIT_FOR_PULSE_START,
        STATE_WAIT_FOR_PULSE_END
    } state;
    bool pulse_value;
    uint16_t pulse_end;
};
