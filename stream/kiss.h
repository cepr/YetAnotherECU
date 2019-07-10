#pragma once
#include "output_stream.h"
#include "../task.h"

#define FEND 0xC0 // Frame End
#define FESC 0xDB // Frame Escape
#define TFEND 0xDC // Transposed Frame End
#define TFESC 0xDD // Transposed Frame Escape

class KissEncoder: public FrameOutputStream
{
public:
    KissEncoder() :
        out(0),
        buffer(-1)
    {
    }

    void pipe(OutputStream *out)
    {
        this->out = out;
    }

    // OutputStream implementation
    virtual uint8_t write(uint8_t data)
    {
        // Check if we have to send more data from previous call
        if (buffer >= 0) {
            if (out->write((uint8_t)buffer)) {
                // Successfuly wrote the content of the buffer
                buffer = -1;
            }
            return 0;
        }

        // Buffer is empty at this point
        if (data == FEND) {
            if (out->write(FESC)) {
                buffer = TFEND;
                return 1;
            } else {
                return 0;
            }
        }

        if (data == FESC) {
            if (out->write(FESC)) {
                buffer = TFESC;
                return 1;
            } else {
                return 0;
            }
        }

        return out->write(data);
    }

    virtual uint8_t end_of_frame()
    {
        // Check if we have to send more data from previous call
        if (buffer >= 0) {
            if (out->write((uint8_t)buffer)) {
                // Successfuly wrote the content of the buffer
                buffer = -1;
            }
            return 0;
        }

        // Buffer is empty at this point
        return out->write(FEND);
    }

protected:
    OutputStream *out;
    int16_t buffer;
};

class KissDecoder: public OutputStream
{
public:
    KissDecoder() :
        sink(0),
        escape(false)
    {
    }

    FrameOutputStream* pipe(FrameOutputStream *sink)
    {
        this->sink = sink;
        return sink;
    }

    virtual uint8_t write(uint8_t data)
    {
        if (data == FEND) {
            return sink->end_of_frame();
        }

        if (data == FESC) {
            escape = true;
            return 1;
        }

        if (escape) {
            if (data == TFEND)
                data = FEND;
            else if (data == TFESC)
                data = FESC;
        }

        uint8_t ret = sink->write(data);
        if (ret) {
            escape = false;
        }
        return ret;
    }

protected:
    FrameOutputStream *sink;
    bool escape;
};
