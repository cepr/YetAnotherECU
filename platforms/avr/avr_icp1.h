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
#include "avr_timer_input_capture.h"
#include <avr/io.h>

class ICP1: public AvrTimerInputCapture {
protected:
    virtual bool read()
    {
        return (PIND >> 4) & 1;
    }
    virtual void set_input_capture_edge(bool rising)
    {
        if (rising) {
            TCCR1B |= _BV(ICES1);
        } else {
            TCCR1B &= ~_BV(ICES1);
        }
    }
    virtual void enable_input_capture_noise_canceler()
    {
        TCCR1B |= _BV(ICNC1);
    }
    virtual void enable_input_capture_interrupt()
    {
        TIMSK1 |= _BV(ICIE1);
    }
    virtual uint16_t read_input_capture_register()
    {
        return ICR1;
    }
};

extern ICP1 icp1;
