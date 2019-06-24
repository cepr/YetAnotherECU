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
#include "../../linked_queue.h"
#include "../../task.h"
#include "avr_analog_input.h"
#include <avr/io.h>

class AvrADC : public Task {
public:
    /**
     * @brief
     *      Default constructor
     */
    AvrADC() :
        iterator(&list),
        analog_input(0)
    {
    }

    /**
     * @brief
     *      Register an analog input to read
     *
     * @param analog_input
     *      Pointer to a AvrAnalogInput
     */
    void register_analog_input(AvrAnalogInput* analog_input)
    {
        list.add(analog_input);
    }

protected:
    // Task implementation
    void execute()
    {
        if (!analog_input) {
            // Get the next channel to read
            analog_input = iterator.next();
            if (!analog_input) {
                // Nothing to read
                return;
            }
            // Select the ADC channel
            ADMUX = analog_input->channel;
            // Start the conversion
            ADCSRA = _BV(ADEN) | // ADC Enable
                     _BV(ADSC) | // ADC Start Conversion
                     _BV(ADIF);  // ADC Interrupt Flag
        } else if (ADCSRA & _BV(ADIF)) {
            // This bit is set when an ADC conversion completes
            // and the Data Registers are updated.
            analog_input->on_adc_conversion_complete(ADC);
            analog_input = 0;
        }
    }

    LinkedQueue<AvrAnalogInput> list;
    LinkedQueue<AvrAnalogInput>::Iterator iterator;
    AvrAnalogInput* analog_input;
};
