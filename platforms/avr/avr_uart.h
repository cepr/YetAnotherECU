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
#include "../../uart.h"
#include "../../task.h"
#include <avr/io.h>
#include <avr/interrupt.h>

class AvrUart: public Uart {
public:
    AvrUart() :
        Uart(),
//        transmit_handler(this),
        receive_handler(this)
    {
    }

    // defaults to 9600 baud if F_CPU = 16000000
    void begin(uint16_t ubrr = 103)
    {
        cli();
        UCSRnA(0);
        UCSRnB(_BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0));
        UCSRnC(_BV(UCSZ01) | _BV(UCSZ00));
        UBRRn(ubrr);
        sei();
    }

    virtual uint8_t write(uint8_t data)
    {
        // Check the UART write buffer is ready
        if ((UCSRnA() & _BV(UDRE0)) == 0) {
            // The USART data register is not empty
            return 0;
        }
        // Write the byte into the UART write buffer
        UDRn(data);
        // Enable the write buffer empty interrupt
        UCSRnB(UCSRnB() | _BV(UDRIE0));
        // Done
        return 1;
    }

//    class AvrUartTransmitHandler: public Task {
//    public:
//        AvrUartTransmitHandler(AvrUart* uart) :
//            uart(uart)
//        {
//        }

//        virtual void execute()
//        {
//            // Transmit buffer empty
//            if (uart->listener) {
//                uart->listener->on_uart_transmit_ready();
//            }
//        }

//    protected:
//        AvrUart* uart;
//    };

    class AvrUartReceiveHandler: public Task {
    public:
        AvrUartReceiveHandler(AvrUart* uart) :
            uart(uart)
        {
        }

        virtual void execute()
        {
            // Receive buffer has some data
            uart->sink->write(received_byte);
        }
        uint8_t received_byte;

    protected:
        AvrUart* uart;
    };

//    AvrUartTransmitHandler transmit_handler;
    AvrUartReceiveHandler receive_handler;

protected:
    // UDRn – USART I/O Data Register n
    virtual uint8_t UDRn() = 0;
    virtual void UDRn(uint8_t value) = 0;

    // UCSRnA – USART Control and Status Register A
    virtual uint8_t UCSRnA() = 0;
    virtual void UCSRnA(uint8_t value) = 0;

    // UCSRnB – USART Control and Status Register n B
    virtual uint8_t UCSRnB() = 0;
    virtual void UCSRnB(uint8_t value) = 0;

    // UCSRnC – USART Control and Status Register n C
    virtual uint8_t UCSRnC() = 0;
    virtual void UCSRnC(uint8_t value) = 0;

    // UBRRnL and UBRRnH – USART Baud Rate Registers
    virtual uint16_t UBRRn() = 0;
    virtual void UBRRn(uint16_t value) = 0;
};
