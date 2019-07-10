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
#include <avr/io.h>
#include "avr_scheduler.h"
#include "../../triggers/trigger_wheel_36_1.h"
#include "../../coil.h"
#include "../../engines/vw_type4.h"
#include "avr_digital_input.h"
#include "avr_digital_output.h"
#include "avr_icp1.h"
#include "avr_uart_0.h"
#include "avr_oc3c.h"
#include "avr_oc3a.h"
#include "avr_oc1b.h"
#include "avr_oc1a.h"
#include "avr_init.h"
#include "avr_adc.h"
#include "avr_analog_input.h"
#include "../../stream/analog_input_pin_serializable.h"
#include "../../stream/rotation_sensor_serializable.h"
#include "../../stream/kiss.h"
#include "../../stream/telemetry_handler.h"
#include "../../stream/command_handler.h"
#include "../../stream/serializable_variable.h"
#include "../../parameters.h"

AvrDigitalOutput ignitor_en(&DDRE, &PORTE, 4, false);
TriggerWheelSensor crankshaft_position(&icp1);
AvrDigitalInput trigger_wheel_pin(&DDRD, &PIND, &PORTD, 4, false);

Coil coil1(&oc3c);
Coil coil2(&oc3a);

VwType4 engine(&crankshaft_position, &coil1, &coil2, &oc1b, &oc1a);

AvrADC adc;

extern "C" void __cxa_pure_virtual()
{
    while (true)
    {
    }
}



int main(void)
{
    avr_init();

    ignitor_en.init();
    ignitor_en.set(true);

    scheduler.add_background_task(&adc);
    scheduler.add_background_task(&engine.coil_handler_1);
    scheduler.add_background_task(&engine.coil_handler_2);
    scheduler.add_background_task(&engine.injector_handler_1);
    scheduler.add_background_task(&engine.injector_handler_2);
    crankshaft_position.begin();

    AnalogInputPin* sensor_map = adc.get(0);
    AnalogInputPin* sensor_iat = adc.get(1);
    AnalogInputPin* sensor_head_temp = adc.get(2);
    AnalogInputPin* sensor_tps = adc.get(3);
    AnalogInputPin* sensor_oil_pressure = adc.get(4);
    AnalogInputPin* sensor_fuel_pressure = adc.get(5);
    AnalogInputPin* sensor_oxygen = adc.get(6);
    AnalogInputPin* sensor_battery = adc.get(7);

    AnalogInputPinSerializable sensor_map_serializer(TAG_MANIFOLD_AIR_PRESSURE, sensor_map);
    AnalogInputPinSerializable sensor_iat_serializer(TAG_INTAKE_AIR_TEMPERATURE, sensor_iat);
    AnalogInputPinSerializable sensor_head_temp_serializer(TAG_HEAD_TEMPERATURE, sensor_head_temp);
    AnalogInputPinSerializable sensor_tps_serializer(TAG_THROTTLE_POSITION, sensor_tps);
    AnalogInputPinSerializable sensor_oil_pressue_serializer(TAG_OIL_PRESSURE, sensor_oil_pressure);
    AnalogInputPinSerializable sensor_fuel_pressure_serializer(TAG_FUEL_PRESSURE, sensor_fuel_pressure);
    AnalogInputPinSerializable sensor_oxygen_serializer(TAG_OXYGEN, sensor_oxygen);
    AnalogInputPinSerializable sensor_battery_serializer(TAG_BATTERY_VOLTAGE, sensor_battery);
    RotationSensorSerializable sensor_rpm_serializable(TAG_CRANKSHAFT_RPM, &crankshaft_position);

    // Sensors

    // Telemetry
    Serializable* telemetry_fields[] = {
        &sensor_map_serializer,
        &sensor_iat_serializer,
        &sensor_head_temp_serializer,
        &sensor_tps_serializer,
        &sensor_oil_pressue_serializer,
        &sensor_fuel_pressure_serializer,
        &sensor_oxygen_serializer,
        &sensor_battery_serializer,
        &sensor_rpm_serializable,
        0
    };
    TelemetryHandler telemetry_handler(telemetry_fields);
    KissEncoder kiss_encoder;
    telemetry_handler.pipe(&kiss_encoder);
    kiss_encoder.pipe(&uart0);
    scheduler.add_background_task(&telemetry_handler);

    // Commanding
    Serializable* command_fields[] = {
        &parameters.crankshaft_offset,
        &parameters.ignition_pulse_width,
        0
    };
    KissDecoder kiss_decoder;
    CommandHandler command_handler(command_fields);
    uart0.pipe(&kiss_decoder);
    kiss_decoder.pipe(&command_handler);

    // Start the platform drivers
    uart0.begin();

    // Main loop
    while(true) {
        tick = TCNT1;
        scheduler.execute();
    }
}
