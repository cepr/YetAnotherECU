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
#include "injector.h"
#include "coil.h"
#include "analog_sensor.h"

class Board {
    virtual unsigned int  get_injector_count()                = 0;
    virtual Injector*     get_injector(unsigned int index)    = 0;

    virtual unsigned int  get_coil_count()                    = 0;
    virtual Coil*         get_coil(unsigned int index)        = 0;

    virtual AnalogSensor* get_rpm_sensor()                    = 0;
    virtual AnalogSensor* get_throttle_position_sensor()      = 0;
    virtual AnalogSensor* get_head_temperature_sensor()       = 0;
    virtual AnalogSensor* get_intake_air_temperature_sensor() = 0;
    virtual AnalogSensor* get_battery_voltage_sensor()        = 0;
    virtual AnalogSensor* get_oxygen_sensor()                 = 0;
    virtual AnalogSensor* get_manifold_air_pressure_sensor()  = 0;
    virtual AnalogSensor* get_fuel_pressure_sensor()          = 0;
    virtual AnalogSensor* get_oil_pressure_sensor()           = 0;
};
