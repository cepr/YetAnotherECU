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

#include <avr/interrupt.h>
#include "avr_uart_0.h"
#include "avr_scheduler.h"

AvrUart0 uart0;

ISR(USART0_RX_vect)
{
    // Read the incoming byte, this will clear the interrupt flag
    uart0.receive_handler.received_byte = UDR0;
    scheduler.add_interrupt_task(&uart0.receive_handler);
}

ISR(USART0_UDRE_vect)
{
    // Deactivate the interrupt until something is written again
    UCSR0B &= ~_BV(UDRIE0);
    //scheduler.add_interrupt_task(&uart0.transmit_handler);
}
