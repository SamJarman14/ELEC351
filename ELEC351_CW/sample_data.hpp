#ifndef _SAMPLE_DATA_H_
#define _SAMPLE_DATA_HPP_
#include "uop_msb.h"
#include "UpdateDisp.hpp"


extern bool stop_t1, menu_engaged;
extern Mutex mutex3;

// Structure for data 
struct Data
{
    // Data variables
    float temp;
    float pressure;
    float light_level;

    // Function to update the data
    void update()
    {
        temp = env.getTemperature();
        pressure = env.getPressure();
        light_level = ldr.read();
    }
};

extern Data data;

void sample_data(Data *data);

#endif