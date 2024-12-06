////////////// Sam Jarman //////////////
//////////////  10704486  //////////////

#include "Sample_Mailbox.hpp"
#include "UpdateDisp.hpp"
#include "Monitor_Buttons.hpp"
#include "ReadTerminal.hpp"
#include "LED_Strip.hpp"

// Initialise mailbox (size = 16)
Mail<message_t, 16> mail_box;

// Function to empty the mailbox
void empty_Mailbox(){
    char OutputBuffer[2000] = "";
    // Repeat until the mailbox is empty
    while(!mail_box.empty()){

        // Try to get data from the mailbox
        message_t *msg = mail_box.try_get();

        // If data was successfully got 
        if (msg != NULL){

            char OutputString[100];
            char Output_DateTime[30];

            struct tm* tt;                  // Create empty tm struct
            tt = localtime(&msg->time);      // Convert time_t to tm struct using localtime
            
            // Put all data into buffers
            sprintf(OutputString, "\n----- Sample %d -----\nTemperature:\t%3.1fC\nPressure:\t%4.1fmbar\nLight Level:\t%1.2f\n", msg->sample_num, msg->temp,msg->pressure,msg->light_level);
            //printf("\n----- Sample %d -----\nTemperature:\t%3.1fC\nPressure:\t%4.1fmbar\nLight Level:\t%1.2f\n", msg->sample_num, msg->temp,msg->pressure,msg->light_level);
            sprintf(Output_DateTime, "%s\n", asctime(tt));


            // Concatenate buffers
            strcat(OutputBuffer, OutputString);
            strcat(OutputBuffer, Output_DateTime);

            // As long as the user isn't currently changing the date and time, update the LCD display
            if (menu_engaged == 0){         // flag to check the user isn't in the menu to enter the date and time                                                       
                DateTime_UpdateDisp(tt);    // Call function to update the date and time on the LCD display
            }

            // Free the mailbox
            mail_box.free(msg);
        }  
    }    
    
    // Write text to the SD card
    if(sd.card_inserted()){ // Check to see if the card is present (polls PF_4)
    int err = sd.write_file("Samples.txt", OutputBuffer, 1, false);    // Attempt to write text to file
    if(err == 0){   // If is successful, read the content of the file back
        printf("Successfully written to SD card\n");
    }
    else{
        printf("Error Writing to SD card\n");
    }
    }
    else{
        printf("No SD Card Detected\n");
    }
}

// Thread to sample data and put it into the mailbox
void Sample_Mailbox(){
    while(true){
        // If the user hasn't stopped sampling via a terminal input
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

            // Print each sample number, sample values and date and time to the terminal
            printf("\n----- Sample %d -----\nTemperature:\t%3.1fC\nPressure:\t%4.1fmbar\nLight Level:\t%1.2f\n", Sample_num, Temperature, Pressure, Light_Level);
            printf("%s\n", asctime(tt));

            // Update LED Strip 
            LED_Strip(selection);

            // Empty mailbox if it's full
            if (mail_box.full()){
            empty_Mailbox();
            }

            // Allocate a space in the mailbox for the latest sample reading if there's space
            message_t *message = mail_box.try_alloc();
            if (message == NULL){
                printf("Out of memory\n");
            };

            // Update the class variables with the latest sample readings 
            message->temp = Temperature;
            message->pressure = Pressure;
            message->light_level = Light_Level;
            message->time = time_now;
            message->sample_num = Sample_num;

            // Increment the sample number
            Sample_num++;

            // Put the latest sample readings, date, time and current sample number into the mailbox
            osStatus stat = mail_box.put(message);

            // If there's an error, free the mailbox
            if(stat == osErrorResource){
                printf("Queue->put() Error\n");
                mail_box.free(message);
            }
            ThisThread::sleep_for(std::chrono::seconds(10));       // Put this thread to sleep for 10s (10s sample rate)
        }
        else if (stop_sampling == 1){
            ThisThread::sleep_for(std::chrono::seconds(10));       // Put this thread to sleep for 10s (10s sample rate)
        }
    }
}