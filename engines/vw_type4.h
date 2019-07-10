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
#include "../task.h"
#include "../scheduler.h"

// No injector clicking at 150, idle should be between 2.5 and 3.5 ms, divided by two because of wasted spark setup.
//
#define INJECTOR_PULSE_DURATION 375

#define TIMER_TOO_CLOSE 1000
#define TIMER_TOO_FAR 32768

class VwType4 {
public:
    VwType4(RotationSensor *crankshaft,
            Coil *coil1,
            Coil *coil2,
            DigitalOutputPin *injector1,
            DigitalOutputPin *injector2) :
        crankshaft(crankshaft),
        coil_handler_1(crankshaft, coil1, 0),
        coil_handler_2(crankshaft, coil2, 32768),
        injector_handler_1(crankshaft, injector1, 500),
        injector_handler_2(crankshaft, injector2, 32768+500)
    {
    }

    class CoilHandler : public Task {
    public:
        CoilHandler(RotationSensor *crankshaft,
                     Coil *coil,
                     uint16_t angle) :
            crankshaft(crankshaft),
            coil(coil),
            angle(angle)
        {
        }

        virtual void execute()
        {
            uint16_t abs_time = crankshaft->estimate_time_for_angle(angle);
            uint16_t time_from_now = abs_time - tick;
            if (time_from_now > TIMER_TOO_CLOSE && time_from_now < TIMER_TOO_FAR) {
                // We only update if the spark will happen soon enough but not too soon
                coil->set_spark_time(abs_time);
            }
        }
    protected:
        RotationSensor *crankshaft;
        Coil *coil;
        uint16_t angle;
    };


    class InjectorHandler: public Task {
    public:
        InjectorHandler(RotationSensor *crankshaft,
                        DigitalOutputPin *injector,
                        uint16_t angle) :
            crankshaft(crankshaft),
            injector(injector),
            angle(angle)
        {
        }

        virtual void execute()
        {
            uint16_t abs_time = crankshaft->estimate_time_for_angle(angle);
            uint16_t time_from_now = abs_time - tick;
            if (time_from_now > TIMER_TOO_CLOSE && time_from_now < TIMER_TOO_FAR) {
                // We only update if the injector will happen soon enough but not too soon
                injector->set(true, abs_time, INJECTOR_PULSE_DURATION);
            }
        }

    protected:
        RotationSensor *crankshaft;
        DigitalOutputPin *injector;
        uint16_t angle;
    };

    RotationSensor *crankshaft;
    CoilHandler coil_handler_1;
    CoilHandler coil_handler_2;
    InjectorHandler injector_handler_1;
    InjectorHandler injector_handler_2;
};
