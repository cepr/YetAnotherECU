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
#include "../../task.h"
#include "../../pin.h"

class AvrTimerInputCapture: public DigitalInputPin, public Task {

public:
    virtual void attach_interrupt(Level level, Listener* listener)
    {
        this->listener = listener;
        set_input_capture_edge(level == RISING);
        enable_input_capture_noise_canceler();
        enable_input_capture_interrupt();
    }

    // Task implementation
    // Called on input capture interrupt
    virtual void execute()
    {
        if (listener) {
            listener->on_pin_change(read_input_capture_register());
        }
    }

protected:
    virtual void set_input_capture_edge(bool rising) = 0;
    virtual void enable_input_capture_noise_canceler() = 0;
    virtual void enable_input_capture_interrupt() = 0;
    virtual uint16_t read_input_capture_register() = 0;
    Listener* listener;
};
