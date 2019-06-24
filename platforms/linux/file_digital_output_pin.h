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
#include <stdio.h>

class FileDigitalOutputPin : public DigitalOutputPin, public Task {
public:
    // Constructor
    FileDigitalOutputPin(const char* path, bool default_value = false) :
        time(0),
        pin_value(default_value),
        fd(fopen(path, "w")),
        pulse_value(false),
        pulse_start(0),
        pulse_duration(0)
    {
    }

    // DigitalOutputPin
    virtual void set(bool value)
    {
        pin_value = value;
    }

    virtual void set(bool value, uint16_t start, uint16_t duration)
    {
        this->pulse_value = value;
        this->pulse_start = start;
        this->pulse_duration = duration;
    }

    // Task
    virtual void execute()
    {
        if (pulse_duration) {
            set((time >= pulse_start) && (time < pulse_start + pulse_duration));
        }
    }

    void step() {
        fwrite(&pin_value, 1, 1, fd);
        time++;
    }

protected:
    uint16_t time;
    uint8_t pin_value;
    FILE* fd;
    bool pulse_value;
    uint16_t pulse_start;
    uint16_t pulse_duration;
};
