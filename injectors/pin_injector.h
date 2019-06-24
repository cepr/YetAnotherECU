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
#include "../injector.h"
#include "../pin.h"

class PinInjector : public Injector {
public:
    void init(DigitalOutputPin *pin) {
        this->enable = false;
        this->pin = pin;
    }

    virtual void enable(bool enabled)
    {
        this->enabled = enabled;
    }

    virtual void set(uint16_t start, uint16_t duration)
    {
        // TODO replace time by angle
        this->pin->set(true, start, duration);
    }

protected:
    bool enabled;
    DigitalOutputPin *pin;
};
