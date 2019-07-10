#pragma once
#include "serializable.h"

template <class T>
class SerializableVariable: public Serializable
{
public:
    SerializableVariable(uint8_t tag, T value) :
        Serializable(tag),
        value(value),
        state(0)
    {
    }

    virtual uint8_t serialize(FrameOutputStream *out)
    {
        if (state == 0) {
            cached_value = value;
        }

        state+=out->write(bytes[state]);

        if (state < sizeof(T)) {
            return 0;
        } else {
            state = 0;
            return 1;
        }
    }

    virtual void unserialize_begin()
    {
        state = 0;
    }

    virtual void unserialize(uint8_t byte)
    {
        if (state < sizeof(T)) {
            bytes[state++] = byte;
            if (state == sizeof(T)) {
                value = cached_value;
            }
        }
    }

    T get()
    {
        return value;
    }

    void set(T value)
    {
        this->value = value;
    }

protected:
    union {
        T cached_value;
        uint8_t bytes[sizeof(T)];
    };
    T value;
    uint8_t state;
};
