/* 
 * Filename: main.cpp
 * Author: Andrew Norris
 * Institution: Plymouth University
 * Date: 21/10/24
 * Description: Starter Code for ELEC351 Coursework 24/25
 * 
 * Notes:
 *
 * For documentation regarding the module support board library, please see the Readme.md on github
 * https://github.com/PlymouthUniversityEEER/PlymouthUniversityModuleSupportBoard/blob/main/README.md
 *
 * You will need to set the module support board version that you have in MSB_Config.h
 * The default if V4. If you are using a V2 comment out "#define MSB_VER 4" and uncomment "#define MSB_VER 2"
 * 
 * 
 */
#include "uop_msb.h"
#include "mbed.h"
#include <chrono>
#include "Datetime.hpp"
#include "UpdateDisp.hpp"
#include "Monitor_Buttons.hpp"
#include "ReadTerminal.hpp"
#include "Sample_Mailbox.hpp"
#include "LED_Strip.hpp"

// These objects have already been created for you in uop_msb.h
extern EnvSensor env;
extern LatchedLED latchedLEDs;
extern SDCard sd;
extern LCD_16X2_DISPLAY disp;

// Global variables
time_t TimeDate;
int TimeDateSetting, year = 2000, month = 6, day = 15, hour = 12, minute = 30, second = 0;
bool Stop_t8, stop_sampling = 0, start = 0, menu_engaged = 0;
char string_input[31], Sampling_value[7], date_value[13], time_value[10], select_value[5]; 

// Mutexes
Mutex mutex1, mutex2, mutex3, mutex4;

// Threads
Thread t1, t2, t3;

int Mailbox_size;

float Temperature, Pressure, Light_Level;
int Sample_num = 1;

bool Clear_File = 0;


int main()
{
    // Clear text file contents
    if (Clear_File == 0){
        int err = sd.write_file("Samples.txt", "------ Sample Data ------", 0, false);   
        Clear_File = 1;
    }
    
    t1.start(Monitor_Buttons);
    t2.start(ReadTerminal);
    t3.start(Sample_Mailbox);
    t3.set_priority(osPriorityRealtime);       // Set thread 3 to the highest priority
}