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
#include "../rotation_sensor.h"
#include "../coil.h"
#include <avr/io.h>

// No injector clicking at 150
#define INJECTOR_PULSE_DURATION 200

#define TIMER_TOO_CLOSE 1000

class VwType4 : public RotationSensor::Listener {
public:
    VwType4(RotationSensor *crankshaft,
            Coil *coil1,
            Coil *coil2,
            DigitalOutputPin *injector1,
            DigitalOutputPin *injector2) :
        crankshaft(crankshaft),
        coil1(coil1),
        coil2(coil2),
        injector1(injector1),
        injector2(injector2)
    {
    }

    void begin()
    {
        crankshaft->set_listener(this);
    }

    // RotationSensor::Listener
    virtual void on_rotation_sensor_updated()
    {
        uint16_t spark_time1 = crankshaft->estimate_time_for_angle(0);
        if (spark_time1 - TCNT1 < TIMER_TOO_CLOSE) {
            coil1->set_spark_time(spark_time1);
        }

        uint16_t spark_time2 = crankshaft->estimate_time_for_angle(32768);
        if (spark_time2 - TCNT1 < TIMER_TOO_CLOSE) {
            coil2->set_spark_time(spark_time2);
        }

        uint16_t injector_time1 = crankshaft->estimate_time_for_angle(16000);
        if (injector_time1 - TCNT1 < TIMER_TOO_CLOSE) {
            injector1->set(true, injector_time1, INJECTOR_PULSE_DURATION);
        }

        uint16_t injector_time2 = crankshaft->estimate_time_for_angle(48000);
        if (injector_time2 - TCNT1 < TIMER_TOO_CLOSE) {
            injector2->set(true, injector_time2, INJECTOR_PULSE_DURATION);
        }
    }

protected:
    RotationSensor *crankshaft;
    Coil *coil1;
    Coil *coil2;
    DigitalOutputPin *injector1;
    DigitalOutputPin *injector2;
};
