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

#include "avr_uart_0.h"
#include "avr_oc3a.h"
#include "avr_oc3c.h"
#include "avr_scheduler.h"
#include "avr_init.h"
#include "avr_digital_output.h"

// 200: misses at 13.8V
#define COIL_PULSE_DURATION 250

AvrDigitalOutput ignitor_en(&DDRE, &PORTE, 4, false);

class MyUartHandler: public OutputStream {
public:
    virtual uint8_t write(uint8_t data)
    {
        switch(data) {
        case 'e':
            ignitor_en.set(true);
            break;
        case 'd':
            ignitor_en.set(false);
            break;
        case 'a':
            oc3c.set(true, 0, COIL_PULSE_DURATION);
            oc3a.set(false);
            break;
        case 'b':
            oc3c.set(false);
            oc3a.set(true, 0, COIL_PULSE_DURATION);
            break;
        }
        return 1;
    }
};

int main(void)
{
    avr_init();
    ignitor_en.init();

    MyUartHandler handler;
    uart0.pipe(&handler);
    uart0.begin();

    while(true) {
        scheduler.execute();
    }
}
