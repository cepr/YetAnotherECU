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
#include <stdint.h>

/**
 * @brief
 *      Shift register with an history of 8 elements
 *
 * Every time you push a new element to the register, the oldest element is discarded.
 *
 * The register keeps track of the average of all elements.
 */
class ShiftRegister {
public:

    /**
     * @brief
     *      Default constructor.
     *
     * The shift register is initialized with zeros.
     */
    ShiftRegister() :
        reg{0,},
        sum(0),
        avg(0)
    {
    }

    /**
     * @brief
     *      Push a new value to the register and discard the oldest one.
     *
     * @param value
     *      Value to push
     */
    void push(uint16_t value) {
        sum -= reg[7];
        reg[7] = reg[6];
        reg[6] = reg[5];
        reg[5] = reg[4];
        reg[4] = reg[3];
        reg[3] = reg[2];
        reg[2] = reg[1];
        reg[1] = reg[0];
        reg[0] = value;
        sum += value;
        avg = sum >> 3;
    }

    /**
     * @brief
     *      Get the average value of all 8 elements in the register.
     *
     * @return
     *      The average value
     */
    inline uint16_t average() {
        return avg;
    }

protected:
    uint16_t reg[8];
    uint32_t sum;
    uint16_t avg;
};
