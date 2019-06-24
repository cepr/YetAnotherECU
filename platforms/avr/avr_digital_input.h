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
#include <stdint.h>

class AvrDigitalInput : public DigitalInputPin {
public:
    AvrDigitalInput(volatile uint8_t* ddr, volatile uint8_t* pin, volatile uint8_t* port, uint8_t bit, bool pull_up) :
        ddr(ddr),
        pin(pin),
        port(port),
        bit(bit),
        pull_up(pull_up)
    {
    }

    void init()
    {
        if (pull_up) {
            *(this->port) |= 1<<bit;
        } else {
            *(this->port) &= ~(1<<bit);
        }
        *ddr &= ~(1<<bit);
    }

    virtual bool read()
    {
        return (*pin) & (1<<bit);
    }

    virtual void attach_interrupt(Level level, Listener* listener)
    {
        // TODO
    }

protected:
    volatile uint8_t* ddr;
    volatile uint8_t* pin;
    volatile uint8_t* port;
    uint8_t bit;
    bool pull_up;
};
