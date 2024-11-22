#include "sample_data.hpp"

uint32_t sample_num = 0;


// Function to sample and display the data
void sample_data(Data *data)
{
    while(true)
    {
        if (stop_t1 == 0)                   // Flag to ensure Sampling [OFF] hasn't been entered by the user
        {
            data->update();                 // Update the data samples

            // Print the samples to the terminal
            printf("\n----- Sample %d -----\nTemperature:\t%3.1fC\nPressure:\t%4.1fmbar\nLight Level:\t%1.2f\n", sample_num,data->temp,data->pressure,data->light_level);
            // Print the date and time to terminal
            mutex3.lock();                  // Lock (global variables)
            time_t time_now = time(NULL);   // Get a time_t timestamp from the RTC
            struct tm* tt;                  // Create empty tm struct
            tt = localtime(&time_now);      // Convert time_t to tm struct using localtime
            mutex3.unlock();                // Unlock
            printf("%s\n",asctime(tt));     // Print in human readable format

            if (menu_engaged == 0){         // flag to check the user isn't in the menu to enter the date and time                                                       
                DateTime_UpdateDisp(tt);    // Call function to update the date and time on the LCD display
            }
            sample_num++;                   // Increment the sample number
        }
        ThisThread::sleep_for(std::chrono::seconds(10));       // Put this thread to sleep for 10s
    }
}