#ifndef _SAMPLE_MAILBOX_HPP_
#define _SAMPLE_MAILBOX_HPP_
#include "uop_msb.h"

// Functions
void empty_Mailbox();
void Sample_Mailbox();

// Global variables
extern float Temperature, Pressure, Light_Level;
extern int Sample_num;

// Class initialisation 
class message_t{
    public:
    float temp;
    float pressure;
    float light_level;
    time_t time;
    int sample_num;

    // Constructor
    message_t(float T, float P, float L_L, time_t Ti, int sn){
        temp = T;
        pressure = P;
        light_level = L_L;
        time = Ti;
        sample_num = sn;
    }
};

#endif