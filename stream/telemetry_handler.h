#pragma once

#include "../task.h"
#include "serializable.h"
#include "output_stream.h"

class TelemetryHandler: public Task {
public:
    TelemetryHandler(Serializable** objects) :
        objects(objects),
        state(0),
        index(0),
        sink(0),
        throttle(0)
    {
    }

    void pipe(FrameOutputStream* sink)
    {
        this->sink = sink;
    }

    // Task implementation
    virtual void execute()
    {
        // Slow down!
        if (throttle++)
            return;

        if (state == 0) {
            state += sink->write(objects[index]->tag);
        } else {
            uint8_t ret = objects[index]->serialize(sink);
            if (ret) {
                // Current object serialized, get next object
                state = 0;
                index++;
                if (!objects[index]) {
                    // End of list reached, go back to the first one
                    index = 0;
                }
            }
        }
    }

protected:
    Serializable** objects;
    uint8_t state;
    uint8_t index;
    FrameOutputStream* sink;
    uint8_t throttle;
};
