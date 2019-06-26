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
#include "avr_oc1b.h"
#include "avr_scheduler.h"
#include "avr_init.h"

#define IGNITOR_PULSE_DURATION 200

class MyUartHandler: public Uart::Listener {
public:
    virtual void on_uart_receive(uint8_t data)
    {
        switch(data) {
        case 'e':
            oc1b.set(true, 0, IGNITOR_PULSE_DURATION);
            break;
        case 'd':
            oc1b.set(false);
            break;
        }
    }

    virtual void on_uart_transmit_ready()
    {
    }
};

int main(void)
{
    avr_init();

    MyUartHandler handler;
    uart0.set_listener(&handler);
    uart0.begin();

    while(true) {
        scheduler.execute();
    }
}
