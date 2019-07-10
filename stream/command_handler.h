#pragma once
#include "serializable.h"

class CommandHandler: public FrameOutputStream
{
public:
    CommandHandler(Serializable **objects) :
        objects(objects),
        index(0),
        state(0)
    {
    }

    virtual uint8_t write(uint8_t data)
    {
        if (state == 0) {
            // We just received a tag, search for the object to deserialize
            uint8_t i = 0;
            while(objects[i]) {
                if (objects[i]->tag == data) {
                    index = i;
                    state = 1;
                    objects[index]->unserialize_begin();
                    break;
                }
                i++;
            }
        } else {
            objects[index]->unserialize(data);
        }
        return 1;
    }

    virtual uint8_t end_of_frame()
    {
        state = 0;
        return 1;
    }

protected:
    Serializable **objects;
    uint8_t index;
    uint8_t state;
};
