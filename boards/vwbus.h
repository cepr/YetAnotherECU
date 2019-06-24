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
#include "board.h"

/**
 * 36-1 teeth trigger wheel.
 */
class TriggerWheel {

}

class VWBus_ECU_Board: public Board {

    virtual unsigned int get_injector_count()
    {
        return 2;
    }

    virtual Injector* get_injector(unsigned int index)
    {
        return &stub_injector;
    }

    virtual unsigned int get_coil_count()
    {
        return 2;
    }

    virtual Coil* get_coil(unsigned int index)
    {
        return &stub_coil;
    }

    virtual AnalogSensor* get_rpm_sensor()
    {
        return &stub_analog_sensor;
    }

    virtual AnalogSensor* get_throttle_position_sensor()
    {
        return &stub_analog_sensor;
    }

    virtual AnalogSensor* get_head_temperature_sensor()
    {
        return &stub_analog_sensor;
    }

    virtual AnalogSensor* get_intake_air_temperature_sensor()
    {
        return &stub_analog_sensor;
    }

    virtual AnalogSensor* get_battery_voltage_sensor()
    {
        return &stub_analog_sensor;
    }

    virtual AnalogSensor* get_oxygen_sensor()
    {
        return &stub_analog_sensor;
    }

    virtual AnalogSensor* get_manifold_air_pressure_sensor()
    {
        return &stub_analog_sensor;
    }

    virtual AnalogSensor* get_fuel_pressure_sensor()
    {
        return &stub_analog_sensor;
    }

    virtual AnalogSensor* get_oil_pressure_sensor()
    {
        return &stub_analog_sensor;
    }
};
