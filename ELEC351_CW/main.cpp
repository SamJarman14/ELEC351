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

// Inputs 
DigitalIn ButtonA(BTN1_PIN), ButtonB(BTN2_PIN);
DigitalInOut ButtonC(BTN3_PIN,PIN_INPUT,PullDown,0), ButtonD(BTN4_PIN,PIN_INPUT,PullDown,0);

// These objects have already been created for you in uop_msb.h
extern EnvSensor env;
extern LatchedLED latchedLEDs;
extern SDCard sd;
extern LCD_16X2_DISPLAY disp;

// Global variables
uint32_t sample_num = 0;
time_t TimeDate;
int TimeDateSetting, year = 2000, month = 6, day = 15, hour = 12, minute = 30, second = 0;
bool Stop_t8, stop_t1 = 0, start = 0, menu_engaged = 0;
char string_input[31], Sampling_value[7], date_value[13], time_value[10], select_value[5]; 

// Mutexes
Mutex mutex1, mutex2, mutex3, mutex4;

// Threads
Thread t1, t2, t3, t4, t5, t6, t7;

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

// Initialise struct
Data data;

// Function to update the date and time on the LCD display
void DateTime_UpdateDisp(struct tm* tt)
{
    disp.cls();                                                             // Clear the LCD                 
    char lcd_line_buffer[17];           
    strftime(lcd_line_buffer, sizeof(lcd_line_buffer), "%a %d-%b-%Y", tt);  // Create a string DDD dd-MM-YYYY
    disp.locate(0,0);                                                       // Set LCD cursor to (0,0)
    disp.printf("%s",lcd_line_buffer);                                      // Write text to LCD
    strftime(lcd_line_buffer, sizeof(lcd_line_buffer), "     %H:%M", tt);   // Create a string HH:MM
    disp.locate(1,0);                                                       // Set LCD cursor to (1,0)
    disp.printf("%s",lcd_line_buffer);                                      // Write text to LCD
}

// Function to sample and display the data
void sample_data(Data *data)
{
    while(1)
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

// Function to update the menu on the LCD display
void Menu_UpdateDisp()
{
    disp.cls();                                       // Clear the LCD                 
    disp.locate(0,0);                                 // Set LCD cursor to (0,0)
    switch (TimeDateSetting){                         // Determine which part of the date or time to update
        case 0:                                                   
            disp.printf("Enter Year:");               // Write text to LCD
            disp.locate(1,0);                         // Set LCD cursor to (1,0)
            disp.printf("%d", year);                  // Write text to LCD
            ThisThread::sleep_for(200ms);             // Put thread in a sleep mode for 200ms
            break;
        case 1:
            disp.printf("Enter Month:");                                     
            disp.locate(1,0);                                                      
            disp.printf("%d", month);                                       
            ThisThread::sleep_for(200ms);
            break;
        case 2:
            disp.printf("Enter Day:");                                     
            disp.locate(1,0);                                              
            disp.printf("%d", day);                                   
            ThisThread::sleep_for(200ms);
            break;
        case 3:
            disp.printf("Enter Hours:");                                   
            disp.locate(1,0);                                                      
            disp.printf("%d", hour);                                       
            ThisThread::sleep_for(200ms);
            break;
        case 4:
            disp.printf("Enter Minutes:");                                      
            disp.locate(1,0);                                                       
            disp.printf("%d", minute);                                     
            ThisThread::sleep_for(200ms);
            break;
        case 5:
            disp.printf("Enter Seconds:");                                  
            disp.locate(1,0);                                                        
            disp.printf("%d", second);  
            break;                                    
    }
    ThisThread::sleep_for(200ms);                        // Put thread to sleep for 200ms
}

// Thread to monitor button A
void ButtonA_Monitor()
{
    while(true){
        while(ButtonA==0){};
        ThisThread::sleep_for(100ms);     // Put thread to sleep for 100ms
        while(ButtonA==1){};              // Wait for button A to be pressed and released 
        mutex4.lock();                    // Lock (global variable)
        menu_engaged = 1;                 // Set flag 
        mutex4.unlock();                  // Unlock

        mutex1.lock();                    // Lock (global variables inside switch-statement cases)
        switch (TimeDateSetting){
            case 0:
                if (year < 2125){         // Only years less than 2124 can be displayed                                   
                    year++;               // Increment year
                }
                if (year == 2125){        // If not less than 2124, loop to 1970
                    year = 1970;
                }
                break;
            case 1:
                if (month < 13){           // Only months less than 12 can be displayed                                           
                    month++;               // Increment month
                }
                if (month == 13){          // If not less than 12, loop to 1
                    month = 1;
                }
                break;
            case 2:
                if (day < 32){              // Only days less than 31 can be displayed                                          
                    day++;                  // Increment day
                }
                if (day == 32){             // If not less than 31, loop to 1
                    day = 1;
                }
                break;
            case 3:
                if (hour < 24){              // Only hours less than 23 can be displayed                                        
                    hour++;                  // Increment hour
                }
                if (hour == 24){             // If not less than 24, loop to 0
                    hour = 0;
                }
                break;
            case 4:
                if (minute < 60){             // Only minutes less thn 60 can be displayed                                            
                    minute++;                 // Increment minute
                }
                if (minute == 60){            // If not less than 60, loop to 0
                    minute = 0;
                }
                break;
            case 5:
                if (second < 60){             // Only seconds less than 60 can be displayed                                                    
                    second++;                 // Increment second
                }                
                if (second == 60){            // If not less than 60, loop to 0
                    second = 0;
                }
                break;
        }   
        mutex1.unlock();                      // Unlock
        Menu_UpdateDisp();                    // Call function to update the menu on the LCD display
        ThisThread::sleep_for(100ms);         // Put thread to sleep for 100ms
    }
}

// Thread to monitor button C
//void ButtonC_Monitor()
//{
//    while(1){
//        while(ButtonC==0){};
//        ThisThread::sleep_for(100ms);   // Put thread to sleep for 100ms
//        while(ButtonC==1){};            // Wait for button C to be pressed and released 
//        mutex4.lock();                  // Lock (global variable)
//        menu_engaged = 1;               // Set flag
//        mutex4.unlock();                // Unlock
//
//        mutex1.lock();                  // Lock (global variables inside switch-statement cases)
//        switch (TimeDateSetting){
//            case 0:                                                   
//                if (year > 1969){       // Only years greater than 1970 can be displayed                                                    
//                    year--;             // Decrement year 
//                }
//                if (year == 1969){
//                    year = 2124;        // If not greater than 1969, loop to 2124
//                }
//                break;
//            case 1:
//                if (month > 0){         // Only months greater than 0 can be diaplyed                                            
//                    month--;            // Decrement month
//                }
//                if (month == 0){        // If not greater than 0, loop to 12
//                    month = 12;
//                }
//                break;
//            case 2:
//                if (day > 0){           // Only days greater than 0 can be displayed                                               
//                    day--;              // Decrement day
//                }
//                if (day == 0){          // If not greater than 0, loop to 31
//                    day = 31;
//                }
//                break;
//            case 3:
//               if (hour > -1){         // Only hours greater than -1 can be displayed                                              
//                    hour--;             // Decrement hour
//                }
//                if (hour == -1){        // If not greater than -1, loop to 23
//                    hour = 23;
//                }
//                break;
//            case 4:
//                if (minute > -1){       // Only minutes greater than -1 can be displayed                                              
//                    minute--;           // Decrement minute
//                }
//                if (minute == -1){      // If not greater than -1, loop to 59
//                    minute = 59;
//                }
//                break;
//            case 5:
//                if (second > -1){       // Only seconds greater than -1 can be displayed                                            
//                   second--;           // Decrement second
//               }
//                if (second == -1){      // If not greater than -1, loop to 59
//                    second = 59;
//                }
//                break;
//        }
//        mutex1.unlock();                // Unlock
//        UpdateDisp();                   // Call function to update the LCD display
//            
//        ThisThread::sleep_for(100ms);   // Put thread to sleep for 100ms
//    }
//}

// Thread to monitor button B
void ButtonB_Monitor()
{
    while(1){
        while(ButtonB==0){};
        ThisThread::sleep_for(100ms);      // Put thread to sleep for 100ms
        while(ButtonB==1){};               // Wait for button B to be pressed and released 
        mutex4.lock();                     // Lock (global variable)
        menu_engaged = 1;                  // Set flag   
        mutex4.unlock();                   // Unlock

        mutex2.lock();                     // Lock (global variables inside switch-statement cases)
        if (TimeDateSetting > 0){          
            TimeDateSetting--;             // If greater than 0, decrement value 
        }
        else if (TimeDateSetting == 0){
            TimeDateSetting = 5;           // If equal to zero, loop to 5
        }
        mutex2.unlock();                   // Unlock
        Menu_UpdateDisp();                 // Call function to update the menu on the LCD display
        ThisThread::sleep_for(100ms);      // Put thread to sleep for 100ms
    }
}

// Thread to monitor button D
void ButtonD_Monitor()
{
    while(1){
        while(ButtonD==0){};
        ThisThread::sleep_for(100ms);       // Put thread to sleep for 100ms
        while(ButtonD==1){};                // Wait for button D to be pressed and released 
        mutex4.lock();                      // Lock (global variable)
        menu_engaged = 1;                   // Set flag
        mutex4.unlock();                    // Unlock

        mutex2.lock();                      // Lock (global variables inside switch-statement cases)
        if (TimeDateSetting < 5)            // If less than 5, increment value
        {
            TimeDateSetting++;
        }
        else if (TimeDateSetting == 5)      // If equal to 5, loop to 0
        {
            TimeDateSetting = 0;
        }
        mutex2.unlock();                    // Unlock
        Menu_UpdateDisp();                  // Call function to update the menu on the LCD display
        ThisThread::sleep_for(100ms);       // Put thread to sleep for 100ms
    }
}

// Function to calculate the desired date and times epoch equivalent 
time_t Datetime(int year, int mon, int mday, int hour, int min, int sec) 
{
    struct tm   t;              // Initialise structure
    time_t t_of_day;
    // Set each variable of the date and time to the current values entered by the user
    t.tm_year = year-1900;
    t.tm_mon =  (mon-1);        
    t.tm_mday = mday;
    t.tm_hour = hour;
    t.tm_min = min;
    t.tm_sec = sec;
    t.tm_isdst = -1;            // Is Daylight saving time on? 1 = yes, 0 = no, -1 = unknown
    t_of_day = mktime(&t);
    return t_of_day;            // returns seconds elapsed since January 1, 1970 (start of the Epoch)
}

// Thread to monitor the blue button
void BlueButton_Monitor()
{
    while(1){
        while(ButtonC==0){}; 
        ThisThread::sleep_for(100ms);       // Put thread to sleep for 100ms
        while(ButtonC==1){};                // Wait for blue button to be pressed and released 
        mutex4.lock();                      // Lock (global variable)
        menu_engaged = 0;                   // Reset flag
        mutex4.unlock();                    // Unlock

        mutex3.lock();                      // Lock (global variables)
        TimeDate = Datetime(year, month, day, hour, minute, second);
        // Set the time on the RTC 
        uint64_t now = (long) TimeDate;
        set_time(now);
        time_t time_now = time(NULL);       // Get a time_t timestamp from the RTC
        struct tm* tt;                      // Create empty tm struct
        tt = localtime(&time_now);          // Convert time_t to tm struct using localtime
        mutex3.unlock();                    // Unlock
        printf("Date and Time have been changed to: %s\n",asctime(tt));         // Print date and time change

        DateTime_UpdateDisp(tt);            // Call function to update the date and time on the LCD display
    }
    ThisThread::sleep_for(100ms);           // Put thread to sleep for 100ms
}



















// Function to turn on or off sampling based on the users input
void Sampling()
{
    char Sampling_ON[5] = "[ON]";
    char Sampling_OFF[6] = "[OFF]";
    int sampling_on = strcmp(Sampling_ON, Sampling_value);
    int sampling_off = strcmp(Sampling_OFF, Sampling_value);
    if (sampling_on == 0){
        stop_t1 = 0;
        printf("%s %s\n", string_input, Sampling_value);
    }
    else if (sampling_off == 0){
        stop_t1 = 1;
        printf("%s %s\n", string_input, Sampling_value);
    }
    else{
        printf("Invalid Input\n");
    }
    ThisThread::sleep_for(100ms);
}

void split(char *str, const char *sep, const char *res[], size_t n)
{
    for (size_t i = 0; i < n; i++) {
        size_t len = strcspn(str, sep);    
        res[i] = str; 
        str += len;
        if (*str) *str++ = '\0';
    }
}

void ReadTerminal()
{
    while(1)
    {
        // Compare first 8 characters of input to each potential function name
        scanf("%8s", string_input);
        bool date_time_function = strcmp(string_input, "Datetime");
        bool select_function = strcmp(string_input, "select");
        bool Sampling_function = strcmp(string_input, "Sampling");
        bool flush_function = strcmp(string_input, "flush");
        
        // If the Datetime function is entered, read the next 18 characters in two sections, a sperate date and time section
        if (date_time_function == 0){
            scanf("%11s", date_value);
            scanf("%9s", time_value);
            // Print all setions together to show the users input
            printf("%s %s %s\n", string_input, date_value, time_value);

            // Check that the first and last characters entered are '[' and ']'
            char Compare_1 = '[';
            char Compare_2 = ']';
            bool check = 1;
            if (Compare_1 == date_value[0]){}
            else 
            {
                check = 0;
            }
            if (Compare_2 == time_value[8]){}
            else 
            {
                check = 0;
            }

            // Remove first character (remove '[')
            memmove(date_value, date_value + 1, sizeof date_value - 1);

            // Remove first character (remove ']')
            time_value[8] = '\0';

            // Split the date section into seperate tokens whenever a '/' is detected
            int n = 3;
            const char *token1[n];
            split(date_value, "/", token1, n);

            // Split the time section into seperate tokens whenever a ':' is detected
            const char *token2[n];
            split(time_value, ":", token2, n);

            for (int x = 0; x < 2; x++)
            {
                if (strlen(token1[x]) == 2) {}
                else 
                {
                    check = 0;
                }
            }

            if (strlen(token1[2]) == 4){}
            else 
            {
                check = 0;
            }

            for (int x = 0; x < 2; x++)
            {
                if (strlen(token2[x]) == 2){}
                else 
                {
                    check = 0;
                }
            }

            // Check only numbers were entered
            // Convert each section of the input from a char to an int, if 0 is returned than a non-integer was inputted
            mutex1.lock();
            day = atoi(token1[0]);
            month = atoi(token1[1]);
            year = atoi(token1[2]);
            
            hour = atoi(token2[0]);
            minute = atoi(token2[1]);
            second = atoi(token2[2]);
            mutex1.unlock();

            bool zero_hours_entered = strcmp(token2[0], "00");
            bool zeros_minutes_entered = strcmp(token2[1], "00");
            bool zeros_seconds_entered = strcmp(token2[2], "00");

            // Manually check if and of the time values have been set to 0 by the user, if so don't check for it in the if-statement
            if ((zero_hours_entered == 0) || (zeros_minutes_entered == 0) || (zeros_seconds_entered == 0))
            {
                if ((hour >= 0) && (hour < 24) && (minute >= 0) && (minute < 61) && (second >= 0) && (second < 61))
                {}
                else  
                {       
                    check = 0;
                }
            }
            // If no zero values were inputted by the user, check non of the values are 0 (no non-integers were entered)
            // If a non-integer was inputted or a value outside of the min-max, check will equal 0 
            // Check all values entered are within the min-max range for each variable
            else
            {
                if ((year > 1969) && (year < 2125) && (month > 0) && (month < 13) && (day > 0) && (day < 32) && (year*month*day != 0))
                {}
                else  
                {       
                    check = 0;
                }
                if ((hour >= 0) && (hour < 24) && (minute >= 0) && (minute < 61) && (second >= 0) && (second < 61) && (hour*minute*second != 0))
                {}
                else  
                {       
                    check = 0;
                }
            }

            // If all check equal 1 then a valid input was entered - run Datetime function
            if (check == 1)
            {   
                mutex3.lock();
                TimeDate = Datetime(year, month, day, hour, minute, second);
                //Set the time on the RTC (You can use https://www.epochconverter.com/ for testing)
                uint64_t now = (long) TimeDate;
                set_time(now);
                time_t time_now = time(NULL);   // Get a time_t timestamp from the RTC
                struct tm* tt;                  // Create empty tm struct
                tt = localtime(&time_now);      // Convert time_t to tm struct using localtime
                mutex3.unlock();
                printf("%s\n",asctime(tt));     // Print in human readable format

                DateTime_UpdateDisp(tt);        // Call function to update the date and time on the LCD display

////////////////printf("Valid Input\n");
            }
            else 
            {
                printf("Invalid Input\n");
            }

        }
        else if (select_function == 0)
        {
            scanf("%3s", select_value);
            printf("%s %s\n", string_input, select_value);
        }
        else if (Sampling_function == 0)
        {
            scanf("%5s", Sampling_value);
            Sampling();
        }
        else if (flush_function == 0)
        {
            printf("%s\n", string_input);
        }
        else
        {
            printf("Invalid Input\n");
        }
        ThisThread::sleep_for(100ms);
    }
}


int main()
{
    t1.start(callback(sample_data, &data));
    t1.set_priority(osPriorityRealtime);       // Set thread 1 to the highest priority
    t2.start(ButtonA_Monitor);
    //t3.start(ButtonC_Monitor);
    t4.start(ButtonB_Monitor);
    t5.start(ButtonD_Monitor);
    t6.start(BlueButton_Monitor);
    t7.start(ReadTerminal);

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
        if(data.light_level>0.5f){
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