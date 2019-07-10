#pragma once
#include <stdint.h>

class OutputStream {
public:
    virtual uint8_t write(uint8_t data) = 0;
};

class FrameOutputStream : public OutputStream {
public:
    virtual uint8_t end_of_frame() = 0;
};
