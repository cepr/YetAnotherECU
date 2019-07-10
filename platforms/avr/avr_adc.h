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
#include "avr_analog_input.h"
#include <avr/io.h>

// TODO: implement this task as an interrupt task instead of a background task
class AvrADC : public Task {
public:
    /**
     * @brief
     *      Constructor
     */
    AvrADC() :
        index(0),
        state(0)
    {
    }

    AnalogInputPin* get(uint8_t channel) {
        channels[channel].enabled = true;
        return &channels[channel];
    }

protected:

    // Task implementation
    void execute()
    {
        switch(state) {
        case 0:
            // Get the next channel
            index = (index + 1) & 7;
            if (!channels[index].enabled) {
                // This channel is not enabled, abort.
                break;
            }

            // Select the ADC channel
            ADMUX = _BV(REFS0) | // AVCC with external capacitor at AREF pin
                    index;

            state++;
            break;

        case 1:
            // Start the conversion
            ADCSRA = _BV(ADEN) | // ADC Enable
                     _BV(ADSC) | // ADC Start Conversion
                     _BV(ADIF);  // ADC Interrupt Flag
            state++;
            break;

        default:
            if (ADCSRA & _BV(ADSC)) {
                // Conversion is not complete, abort
                break;
            }

            channels[index].value = ADC;
            if (channels[index].listener) {
                channels[index].listener->on_analog_pin_reading_complete(channels[index].value);
            }
            state = 0;
        }
    }

    AvrAnalogInput channels[8];
    uint8_t index;
    uint8_t state;
};
