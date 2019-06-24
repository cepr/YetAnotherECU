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

#include "avr_init.h"
#include <avr/io.h>

void avr_init()
{
    // Enable Prescaler Timer/Counter Synchronization Mode
    // all timers will be frozen during the configuration
    GTCCR = _BV(TSM);
    // Configure timer1
    TCCR1B = _BV(CS11) | _BV(CS10); // CLK/64, 4us per tick at FCLK=16MHz
    // Configure timer3
    TCCR3B = _BV(CS31) | _BV(CS30); // CLK/64, 4us per tick at FCLK=16MHz
    // Release the prescaler synchronization mode (start timers)
    GTCCR = 0;
}
