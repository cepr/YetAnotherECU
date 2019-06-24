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
#include "avr_uart.h"
#include <avr/io.h>

class AvrUart0: public AvrUart {
protected:
    // UDRn – USART I/O Data Register n
    virtual uint8_t UDRn() { return UDR0; }
    virtual void UDRn(uint8_t value) { UDR0 = value; }

    // UCSRnA – USART Control and Status Register A
    virtual uint8_t UCSRnA() { return UCSR0A; }
    virtual void UCSRnA(uint8_t value) { UCSR0A = value; }

    // UCSRnB – USART Control and Status Register n B
    virtual uint8_t UCSRnB() { return UCSR0B; }
    virtual void UCSRnB(uint8_t value) { UCSR0B = value; }

    // UCSRnC – USART Control and Status Register n C
    virtual uint8_t UCSRnC() { return UCSR0C; }
    virtual void UCSRnC(uint8_t value) { UCSR0C = value; }

    // UBRRnL and UBRRnH – USART Baud Rate Registers
    virtual uint16_t UBRRn() { return UBRR0; }
    virtual void UBRRn(uint16_t value) { UBRR0 = value; }
};

extern AvrUart0 uart0;
