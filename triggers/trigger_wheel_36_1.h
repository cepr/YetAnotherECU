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
        init_count(0),
        tooth_time(0),
        tooth_angle(0),
        tooth_number(0)
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
     * @brief Estimate the time as which the wheel will be at the specified angle.
     *
     * @param angle
     *      Angle between 0 and 65535
     *
     * @return
     *      A time expressed in the platform tick.
     */
    virtual uint16_t estimate_time_for_angle(uint16_t angle)
    {
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
        uint16_t time_diff = ((x >> 1) + (x >> 4)) >> 10;
        return tooth_time + time_diff;
    }

protected:
    // DigitalInputPin::Listener::on_change_pin
    virtual void on_pin_change(DigitalInputPin* pin, uint16_t time)
    {
        // Unused argument
        (void)pin;
        // how much time since the last tooth
        uint16_t duration = time - tooth_time;
        tooth_time = time;
        // fill in the shift register at boot
        if (init_count < 8) {
            sr.push(duration);
            init_count++;
        } else {
            // check if we skipped the missing tooth
            uint16_t threshold = sr.average() * 1.5;
            if (duration < threshold) {
                // Regular tooth
                tooth_number++;
                sr.push(duration);
            } else {
                // We skipped the missing tooth, we are now back on tooth 0
                tooth_number = 0;
            }
            tooth_angle = (((uint32_t)tooth_number)<<16)/36;
            // Notify the listener
            if (this->listener) {
                this->listener->on_rotation_sensor_updated();
            }
        }
    }

    DigitalInputPin* pin;
    uint8_t init_count;
    uint16_t tooth_time;
    uint16_t tooth_angle;
    uint8_t tooth_number;
    ShiftRegister sr;
};
