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

class FileDigitalInputPin : public DigitalInputPin, public Task {
public:
    // Constructor
    FileDigitalInputPin(const char* path) :
        fd(fopen(path, "r")),
        time(0),
        prev(false),
        level(FALLING),
        listener(0)
    {
    }

    // DigitalInputPin
    virtual bool read()
    {
        return pin_value;
    }

    virtual void attach_interrupt(Level level, Listener* listener)
    {
        this->level = level;
        this->listener = listener;
    }

    // Task
    virtual void execute()
    {
        bool newValue = read();
        if (listener) {
            if (((level == RISING) && !prev && newValue) |
                ((level == FALLING) && prev && !newValue))
            {
                listener->on_pin_change(time);
            }
        }
        prev = newValue;
    }

    bool step() {
        size_t s = fread(&pin_value, 1, 1, fd);
        time++;
        return (s == 1);
    }

protected:
    FILE *fd;
    uint8_t pin_value;

    uint16_t time;
    bool prev;
    Level level;
    Listener* listener;
};
