#pragma once
#include <stdint.h>
#include "stream/serializable_variable.h"

#define TAG_MANIFOLD_AIR_PRESSURE       0x00
#define TAG_INTAKE_AIR_TEMPERATURE      0x01
#define TAG_HEAD_TEMPERATURE            0x02
#define TAG_THROTTLE_POSITION           0x03
#define TAG_OIL_PRESSURE                0x04
#define TAG_FUEL_PRESSURE               0x05
#define TAG_OXYGEN                      0x06
#define TAG_BATTERY_VOLTAGE             0x07
#define TAG_CRANKSHAFT_RPM              0x08
#define TAG_IGNITION_ADVANCE            0x09
#define TAG_FUEL_PULSE_WIDTH            0x0a

#define TAG_CRANKSHAFT_OFFSET           0x0b
#define TAG_IGNITION_PULSE_WIDTH        0x0c
#define TAG_IGNITION_PULSE_OFFSET       0x0d
#define TAG_FUEL_MINIMUM_PULSE_WIDTH    0x0e
#define TAG_FUEL_PULSE_OFFSET           0x0f

struct Parameters {
    Parameters() :
        crankshaft_offset(TAG_CRANKSHAFT_OFFSET, 0),
        ignition_pulse_width(TAG_IGNITION_PULSE_WIDTH, 250)
    {
    }

    SerializableVariable<uint16_t> crankshaft_offset;
    SerializableVariable<uint16_t> ignition_pulse_width;
};

extern Parameters parameters;
