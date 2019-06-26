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
#include <avr/io.h>
#include "avr_scheduler.h"
#include "../../triggers/trigger_wheel_36_1.h"
#include "../../coil.h"
#include "../../engines/vw_type4.h"
#include "avr_digital_input.h"
#include "avr_digital_output.h"
#include "avr_icp1.h"
#include "avr_uart_0.h"
#include "avr_oc3c.h"
#include "avr_oc3a.h"
#include "avr_oc1b.h"
#include "avr_oc1a.h"
#include "avr_init.h"

// 200: misses at 13.8V
#define COIL_PULSE_DURATION 250

AvrDigitalOutput ignitor_en(&DDRE, &PORTE, 4, false);
TriggerWheelSensor crankshaft_position(&icp1);
AvrDigitalInput trigger_wheel_pin(&DDRD, &PIND, &PORTD, 4, false);

Coil coil1(&oc3c, COIL_PULSE_DURATION);
Coil coil2(&oc3a, COIL_PULSE_DURATION);

VwType4 engine(&crankshaft_position, &coil1, &coil2, &oc1b, &oc1a);

class MyUartHandler: public Uart::Listener {
public:
    virtual void on_uart_receive(uint8_t data)
    {
        switch(data) {
        case 'e':
            ignitor_en.set(true);
            break;
        case 'd':
            ignitor_en.set(false);
            break;
        }
    }

    virtual void on_uart_transmit_ready()
    {
    }
};

extern "C" void __cxa_pure_virtual()
{
    while (true)
    {
    }
}

int main(void)
{
    avr_init();

    ignitor_en.init();

    MyUartHandler handler;
    uart0.set_listener(&handler);
    uart0.begin();

    engine.begin();
    crankshaft_position.begin();

    while(true) {
        scheduler.execute();
    }
}
