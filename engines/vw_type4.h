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

class VwType4 : public RotationSensor::Listener {
public:
    VwType4(RotationSensor *crankshaft, Coil *coil1, Coil *coil2) :
        crankshaft(crankshaft),
        coil1(coil1),
        coil2(coil2)
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
        coil1->set_spark_time(spark_time1);
        uint16_t spark_time2 = crankshaft->estimate_time_for_angle(32768);
        coil2->set_spark_time(spark_time2);
    }

protected:
    RotationSensor *crankshaft;
    Coil *coil1;
    Coil *coil2;
};
