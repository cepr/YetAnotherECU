#pragma once
#include "output_stream.h"

class Serializable {
public:
    Serializable(uint8_t tag) :
        tag(tag)
    {}

    virtual uint8_t serialize(FrameOutputStream *out) = 0;

    virtual void unserialize_begin() = 0;
    virtual void unserialize(uint8_t byte) = 0;

    const uint8_t tag;
};
