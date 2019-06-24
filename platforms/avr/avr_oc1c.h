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
#include "avr_timer_output_compare.h"

/**
 * @brief OC1C / PB7
 */
class OC1C : public AvrTimerOutputCompare {
protected:
    virtual void enable_output_compare_interrupt() { TIMSK1 |= _BV(OCIE1C); }
    virtual void disable_output_compare_interrupt() { TIMSK1 &= ~_BV(OCIE1C); }
    virtual void set_pin_direction(bool value) { DDRB  = (DDRB  & ~_BV(7)) | (value << 7); }
    virtual void set_pin_value(bool value)     { PORTB = (PORTB & ~_BV(7)) | (value << 7); }
    virtual uint16_t get_timer_counter()       { return TCNT1; }
    virtual void set_compare_output_mode(uint8_t compare_output_mode)
    {
        TCCR1A = (TCCR1A & ~(_BV(COM1C1) | _BV(COM1C0))) | (compare_output_mode << COM1C0);
    }
    virtual void set_output_compare_register(uint16_t ocr) { OCR1C = ocr; }
    virtual bool get_output_compare_flag()
    {
        bool ret = TIFR1 & _BV(OCF1A);
        TIFR1 |= _BV(OCF1A);
        return ret;
    }
};

extern OC1C oc1c;
