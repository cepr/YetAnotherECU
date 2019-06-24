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
 * @brief OC3B / PE4 pin
 */
class OC3B : public AvrTimerOutputCompare {
protected:
    virtual void enable_output_compare_interrupt() { TIMSK3 |= _BV(OCIE3B); }
    virtual void disable_output_compare_interrupt() { TIMSK3 &= ~_BV(OCIE3B); }
    virtual void set_pin_direction(bool value) { DDRE  = (DDRE  & ~_BV(4)) | (value << 4); }
    virtual void set_pin_value(bool value)     { PORTE = (PORTE & ~_BV(4)) | (value << 4); }
    virtual uint16_t get_timer_counter()       { return TCNT3; }
    virtual void set_compare_output_mode(uint8_t compare_output_mode)
    {
        TCCR3A = (TCCR3A & ~(_BV(COM3B1) | _BV(COM3B0))) | (compare_output_mode << COM3B0);
    }
    virtual void set_output_compare_register(uint16_t ocr) { OCR3B = ocr; }
    virtual bool get_output_compare_flag()
    {
        bool ret = TIFR3 & _BV(OCF3B);
        TIFR3 |= _BV(OCF3B);
        return ret;
    }
};

extern OC3B oc3b;
