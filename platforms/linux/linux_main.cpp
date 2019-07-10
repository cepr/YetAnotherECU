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

#include "../../scheduler.h"
#include "../../triggers/trigger_wheel_36_1.h"
#include "../../coil.h"
#include "../../engines/vw_type4.h"
#include "file_digital_input_pin.h"
#include "file_digital_output_pin.h"

#define SCHEDULER_CALLS_PER_TICK 20

uint16_t tick;

int main()
{
    Scheduler<NotSynchronized> scheduler;
    FileDigitalInputPin trigger_wheel_pin("trigger_wheel_signal.bin");
    TriggerWheelSensor crankshaft_position(&trigger_wheel_pin);
    FileDigitalOutputPin coil1_pin("coil1_signal.bin");
    FileDigitalOutputPin coil2_pin("coil2_signal.bin");
    Coil coil1(&coil1_pin);
    Coil coil2(&coil2_pin);
    FileDigitalOutputPin injector1_pin("injector1_signal.bin");
    FileDigitalOutputPin injector2_pin("injector2_signal.bin");
    VwType4 engine(&crankshaft_position, &coil1, &coil2, &injector1_pin, &injector2_pin);

    tick = 0;

    // Configure the scheduler
    scheduler.add_background_task(&trigger_wheel_pin);
    scheduler.add_background_task(&coil1_pin);
    scheduler.add_background_task(&coil2_pin);
    scheduler.add_background_task(&injector1_pin);
    scheduler.add_background_task(&injector2_pin);
    scheduler.add_background_task(&engine.coil_handler_1);
    scheduler.add_background_task(&engine.coil_handler_2);
    scheduler.add_background_task(&engine.injector_handler_1);
    scheduler.add_background_task(&engine.injector_handler_2);
    crankshaft_position.begin();

    while (trigger_wheel_pin.step()) {
        // Run the scheduler
        for (int i = 0; i < SCHEDULER_CALLS_PER_TICK; i++) {
            scheduler.execute();
        }

        tick++;

        // Write outputs
        coil1_pin.step();
        coil2_pin.step();
        injector1_pin.step();
        injector2_pin.step();
    }
    return 0;
}
