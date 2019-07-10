#pragma once
#include "../pin.h"
#include "output_stream.h"
#include "serializable.h"

class AnalogInputPinSerializable: public Serializable
{
public:
    AnalogInputPinSerializable(uint8_t tag, AnalogInputPin* pin) :
        Serializable(tag),
        pin(pin),
        value(0),
        state(0)
    {
    }

    // Serializable implementation
    virtual uint8_t serialize(FrameOutputStream *out)
    {
        switch(state) {
        case 0:
            this->value = pin->get();
            state += out->write(this->bytes[0]);
            return 0;
        case 1:
            state += out->write(this->bytes[1]);
            return 0;
        default:
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
    AnalogInputPin* pin;
    union {
        uint16_t value;
        uint8_t bytes[2];
    };
    uint8_t state;
};
