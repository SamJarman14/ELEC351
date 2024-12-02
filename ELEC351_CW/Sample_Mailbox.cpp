#include "Sample_Mailbox.hpp"
#include "UpdateDisp.hpp"
#include "Monitor_Buttons.hpp"
#include "ReadTerminal.hpp"
#include "LED_Strip.hpp"

Mail<message_t, 16> mail_box;

void empty_Mailbox(){
    char OutputBuffer[2000] = "";
    while(!mail_box.empty()){
        message_t *msg = mail_box.try_get();

        if (msg != NULL){

            char OutputString[100];
            char Output_DateTime[30];

            struct tm* tt;                  // Create empty tm struct
            tt = localtime(&msg->time);      // Convert time_t to tm struct using localtime
            
            sprintf(OutputString, "\n----- Sample %d -----\nTemperature:\t%3.1fC\nPressure:\t%4.1fmbar\nLight Level:\t%1.2f\n", msg->sample_num, msg->temp,msg->pressure,msg->light_level);
            //printf("\n----- Sample %d -----\nTemperature:\t%3.1fC\nPressure:\t%4.1fmbar\nLight Level:\t%1.2f\n", msg->sample_num, msg->temp,msg->pressure,msg->light_level);
            sprintf(Output_DateTime, "%s\n", asctime(tt));

            strcat(OutputBuffer, OutputString);
            strcat(OutputBuffer, Output_DateTime);

            if (menu_engaged == 0){         // flag to check the user isn't in the menu to enter the date and time                                                       
                DateTime_UpdateDisp(tt);    // Call function to update the date and time on the LCD display
            }

            mail_box.free(msg);
        }  
    }    
    
    // Write text to the SD card
    if(sd.card_inserted()){ // Check to see if the card is present (polls PF_4)
    int err = sd.write_file("Samples.txt", OutputBuffer, 1, false);    // Attempt to write text to file
    if(err == 0){   // If is successful, read the content of the file back
        printf("Successfully written to SD card\n");
        printf("---------------------------\nFile contents:\n");
        sd.print_file("Samples.txt",false);
        printf("---------------------------\n");
    }
    else{
        printf("Error Writing to SD card\n");
    }
    }
    else{
        printf("No SD Card Detected\n");
    }
}

void Sample_Mailbox(){
    while(true){
        if (stop_sampling == 0){

            // Get sample values
            Temperature = env.getTemperature();
            Pressure = env.getPressure();
            Light_Level = ldr.read();

            mutex3.lock();                  // Lock (global variables)
            time_t time_now = time(NULL);   // Get a time_t timestamp from the RTC

            // Print date and time to LCD screen
            struct tm* tt;                  // Create empty tm struct
            tt = localtime(&time_now);      // Convert time_t to tm struct using localtime
            if (menu_engaged == 0){         // flag to check the user isn't in the menu to enter the date and time                                                       
                DateTime_UpdateDisp(tt);    // Call function to update the date and time on the LCD display
            }
            mutex3.unlock();                // Unlock

            // Check if LED strip needs to be triggered to flash for high value
            LED_Strip();

            // Empty mailbox if it's full
            if (mail_box.full()){
            empty_Mailbox();
            }

            message_t *message = mail_box.try_alloc();
            if (message == NULL){
                printf("Out of memory\n");
            };

            message->temp = Temperature;
            message->pressure = Pressure;
            message->light_level = Light_Level;
            message->time = time_now;
            message->sample_num = Sample_num;

            Sample_num++;

            osStatus stat = mail_box.put(message);

            if(stat == osErrorResource){
                printf("Queue->put() Error\n");
                mail_box.free(message);
            }
            ThisThread::sleep_for(std::chrono::seconds(1));       // Put this thread to sleep for 10s
        }
        else if (stop_sampling == 1){
            ThisThread::sleep_for(std::chrono::seconds(1));       // Put this thread to sleep for 10s
        }
    }
}