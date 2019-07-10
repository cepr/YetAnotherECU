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
#include "../../pin.h"

class AvrAnalogInput : public AnalogInputPin {
public:
    AvrAnalogInput() :
        enabled(false),
        value(0),
        listener(0)
    {
    }

    virtual uint16_t get() {
        return this->value;
    }

    virtual void listen(Listener* listener)
    {
        this->listener = listener;
    }

    // Accessed by ADC driver
    bool enabled;
    uint16_t value;
    Listener* listener;
};
