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
#include "../analog_sensor.h"
#include "../pin.h"

class PinAnalogSensor : public AnalogSensor, protected AnalogInputPin::Listener {
public:
    PinAnalogSensor(AnalogInputPin *pin) :
        pin(pin)
    {
    }

    virtual uint16_t get()
    {
        return pin->get();
    }

protected:
    // AnalogInputPin::Listener
    virtual void on_analog_pin_reading_complete(uint16_t value)
    {
        if (this->listener) {
            this->listener->on_analog_sensor_value_change(value);
        }
    }

    AnalogInputPin *pin;
};
