#pragma once
#include "../rotation_sensor.h"
#include "serializable.h"

class RotationSensorSerializable: public Serializable
{
public:
    RotationSensorSerializable(uint8_t tag, RotationSensor* sensor) :
        Serializable(tag),
        sensor(sensor),
        state(0),
        value(0)
    {
    }

    // Serializable implementation
    virtual uint8_t serialize(FrameOutputStream *out)
    {
        switch(state) {
        case 0:
            // Cache the sensor value
            this->value = sensor->get_rpm();
            // Send the first byte
            state += out->write(bytes[0]);
            return 0;

        case 1:
            // Send the second byte
            state += out->write(bytes[1]);
            return 0;

        default:
            // Send the end of frame
            if (out->end_of_frame()) {
                state = 0;
                return 1;
            } else {
                return 0;
            }
        }
    }

    virtual void unserialize_begin()
    {
        // No-op
    }

    virtual void unserialize(uint8_t byte)
    {
        // No-op
    }

protected:
    RotationSensor* sensor;
    uint8_t state;
    union {
        uint16_t value;
        uint8_t bytes[2];
    };
};
