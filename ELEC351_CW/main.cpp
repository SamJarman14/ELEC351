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
#include "sample_data.hpp"
#include "Monitor_Buttons.hpp"
#include "ReadTerminal.hpp"

// These objects have already been created for you in uop_msb.h
extern EnvSensor env;
extern LatchedLED latchedLEDs;
extern SDCard sd;
extern LCD_16X2_DISPLAY disp;

// Global variables
time_t TimeDate;
int TimeDateSetting, year = 2000, month = 6, day = 15, hour = 12, minute = 30, second = 0;
bool Stop_t8, stop_t1 = 0, start = 0, menu_engaged = 0;
char string_input[31], Sampling_value[7], date_value[13], time_value[10], select_value[5]; 

// Mutexes
Mutex mutex1, mutex2, mutex3, mutex4;

// Threads
Thread t1, t2, t3, t4;

// Initialise struct
Data data;

// //Class type
// class data_d {
// public:    
//     float temp_value;    
//     float pressure_value;
//     float light_level_value;
    
//     //Constructor
//     data_d(float temp, float pressure, float light_level) {
//         temp_value = temp;
//         pressure_value = pressure;
//         light_level_value = light_level;    
//     }
// };

// Mail<data_d, 10> mail_box;

// void Mailbox_ (){
//     while (true){

//         // Get samples
//         sample_data(&data);

//         //Allocate a block from the memory pool (non blocking version)
//         data_d* message = mail_box.try_alloc();
//         if (message == NULL) {
//             //Out of memory
//             printf("Out of Memory");
//             return;   
//         }

//         //Fill in the data
//         message->temp_value   = data.temp;
//         message->pressure_value = data.pressure;
//         message->light_level_value = data.light_level;

//         //Write pointer to the queue
//         osStatus stat = mail_box.put(message);

//         //Check if succesful
//         if (stat != osOK) {
//             printf("mail_box.put() Error code: %4Xh, Resource not available\r\n", stat);   
//             mail_box.free(message);
//             return;
//         }
//     }   
// }





int main()
{
    t1.start(callback(sample_data, &data));
    t1.set_priority(osPriorityRealtime);       // Set thread 1 to the highest priority
    t2.start(Monitor_Buttons);
    t3.start(ReadTerminal);
    //t4.start(Mailbox_);

    // Set output enable on the latched LEDs.
    latchedLEDs.enable(true);
    
    // Write some text to the SD card
    if(sd.card_inserted()){ // Check to see if the card is present (polls PF_4)
        int err = sd.write_file("test.txt", "Plymouth University - ELEC351 Coursework 24-25\n");    // Attempt to write text to file
        if(err == 0){   // If is successful, read the content of the file back
            printf("Successfully written to SD card\n");
            printf("---------------------------\nFile contents:\n");
            sd.print_file("test.txt",false);
            printf("---------------------------\n");
        }
        else{
            printf("Error Writing to SD card\n");
        }
    }
    else{
        printf("No SD Card Detected\n");
    }

    while (true) {
        // Write the current light level as a float to the 7-segment display
        latchedLEDs.write_seven_seg(data.light_level);

        // If the current light level is above a threshold take action
        if(data.light_level>0.9f){
            for(int i=0;i<4;i++){
                buzz.playTone("C");                     // Play tone on buzzer
                latchedLEDs.write_strip(0xFF,RED);      // Turn on all LEDs on RGB bar
                latchedLEDs.write_strip(0xFF,GREEN);
                latchedLEDs.write_strip(0xFF,BLUE);
                ThisThread::sleep_for(200ms);

                buzz.rest();                            // Stop buzzer
                latchedLEDs.write_strip(0x0,RED);       // Turn off all LEDs on RGB bar
                latchedLEDs.write_strip(0x0,GREEN);
                latchedLEDs.write_strip(0x0,BLUE);
                ThisThread::sleep_for(200ms);
            }
        }
        else
        {
            latchedLEDs.write_strip(0x0,RED);           // Turn off all LEDs on RGB bar
            latchedLEDs.write_strip(0x0,GREEN);
            latchedLEDs.write_strip(0x0,BLUE);
        }

        // Wait total of 10 seconds before next sample
        ThisThread::sleep_for(std::chrono::seconds(10));
    }
}