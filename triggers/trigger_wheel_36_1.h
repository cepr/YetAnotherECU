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
#include "../platform.h"
#include "../pin.h"
#include "../shift_register.h"
#include "../rotation_sensor.h"

// http://thedubshop.com/crank-trigger-package-type-4-vw/

/**
 * @brief
 *      Implementation of a 36-1 trigger wheel using a single digital input pin.
 */
class TriggerWheelSensor : public RotationSensor, private DigitalInputPin::Listener {
public:
    /**
     * @brief
     *      Constructor
     *
     * @param pin
     *      Digital pin connected to the hall sensor.
     */
    TriggerWheelSensor(DigitalInputPin* pin) :
        pin(pin),
        tooth_time(0),
        tooth_angle(0),
        tooth_number(0),
        synchronized(false),
        last_pin_change_time(0),
        rpm(0)
    {
    }

    /**
     * @brief Start tracking the wheel angle and speed
     *
     */
    void begin()
    {
        pin->attach_interrupt(DigitalInputPin::FALLING, this);
    }

    /**
     * @brief Estimate the time at which the wheel will be at the specified angle.
     *
     * @param angle
     *      Angle between 0 and 65535
     *
     * @return
     *      A time expressed in the platform tick.
     *
     * If the position is unknown, or if the returned value is too far in the future,
     * the maximum value is returned, which is tick-1.
     */
    virtual uint16_t estimate_time_for_angle(uint16_t angle)
    {
        if (tick - last_pin_change_time > sr.average() * 3) {
            synchronized = false;
        }
        if (!synchronized) {
            return tick-1;
        }
        uint16_t ticks_per_tooth = sr.average();
        uint16_t angle_diff = angle - tooth_angle;
        // time_diff = angle_diff * tick_per_tooth * 36 / 65536
        // This calculation could potentially overflow a 32 bits register
        // I decided to split this calculation in bit shifting:
        // x*36/65536 = x*32/65536 + x*4/65536
        //            = x/2048 + x/16384
        //            = (x/2 + x/16) / 1024
        // dividing by 2 is equivalent to shifting to the right by 1 bit
        // dividing by 16 is equivalent to shifting to the right by 4 bits
        // dividing by 1024 is equivalent to shifting to the right by 10 bits
        uint32_t x = (uint32_t)angle_diff * (uint32_t)ticks_per_tooth;
        uint32_t time_diff = ((x >> 1) + (x >> 4)) >> 10;
        if (!(time_diff >> 16)) {
            return tooth_time + time_diff;
        } else {
            return tick-1;
        }
    }

    virtual uint16_t get_rpm()
    {
        return rpm;
    }

protected:
    // DigitalInputPin::Listener::on_change_pin
    virtual void on_pin_change(uint16_t time)
    {
        last_pin_change_time = time;
        // how much time since the last tooth
        uint16_t duration = time - tooth_time;
        tooth_time = time;
        // fill in the shift register at boot
        // check if we skipped the missing tooth
        uint16_t threshold = sr.average() * 1.5;
        if ((tooth_number < 8) || (duration < threshold)) {
            // Regular tooth
            tooth_number++;
            sr.push(duration);
        } else {
            // We skipped the missing tooth, we are now back on tooth 0
            if (tooth_number == 34) {
                synchronized = true;
                rpm = 416666.7 / sr.average();
            } else {
                synchronized = false;
                rpm = 0;
            }
            tooth_number = 0;
        }
        tooth_angle = (((uint32_t)tooth_number)<<16)/36;
    }

    DigitalInputPin* pin;
    uint16_t tooth_time;
    uint16_t tooth_angle;
    uint8_t tooth_number;
    ShiftRegister sr;
    bool synchronized;
    uint16_t last_pin_change_time;
    uint16_t rpm;
};
