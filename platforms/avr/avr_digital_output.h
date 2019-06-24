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
#include <stdint.h>
#include <avr/io.h>

class AvrDigitalOutput : public DigitalOutputPin, public Task {
public:
    AvrDigitalOutput(volatile uint8_t* ddr, volatile uint8_t* port, uint8_t bit, bool value) :
        ddr(ddr),
        port(port),
        bit(bit),
        value(value),
        pulse_value(false),
        pulse_start(0),
        pulse_duration(0)
    {
    }

    void init()
    {
        set(value);
        *ddr |= 1<<bit;
    }

    virtual void set(bool value)
    {
        if (value) {
            *(this->port) |= 1<<bit;
        } else {
            *(this->port) &= ~(1<<bit);
        }
    }

    virtual void set(bool value, uint16_t start, uint16_t duration)
    {
        this->pulse_value = value;
        this->pulse_start = start;
        this->pulse_duration = duration;
    }

    virtual void execute()
    {
        if (this->pulse_duration) {
            uint16_t now = TCNT1;
            uint16_t x = now - pulse_start;
            if (x < pulse_duration) {
                // We are currently inside the pulse
                set(pulse_value);
            } else {
                set(!pulse_value);
            }
        }
    }

protected:
    volatile uint8_t* ddr;
    volatile uint8_t* port;
    uint8_t bit;
    bool value;
    bool pulse_value;
    uint16_t pulse_start;
    uint16_t pulse_duration;
};
