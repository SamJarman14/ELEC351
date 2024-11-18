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

DigitalIn ButtonA(BTN1_PIN);
DigitalIn ButtonB(BTN2_PIN);
DigitalInOut ButtonC(BTN3_PIN,PIN_INPUT,PullDown,0);
DigitalInOut ButtonD(BTN4_PIN,PIN_INPUT,PullDown,0);


//These objects have already been created for you in uop_msb.h
extern EnvSensor env;
extern LatchedLED latchedLEDs;
extern SDCard sd;
extern LCD_16X2_DISPLAY disp;

// Global variables
uint32_t sample_num = 0;
time_t TimeDate;
int year = 2000, month = 6, day = 15, hour = 12, minute = 30, second = 0;
int TimeDateSetting;
int start = 0;

// Mutexes
Mutex mutex1;
Mutex mutex2;

// Threads
Thread t1;
Thread t2;
Thread t3;
Thread t4;
Thread t5;
Thread t6;
Thread t7;

// Structure for data 
struct Data
{
    // Sample the data
    float temp;
    float pressure;
    float light_level;

    // function to update the data
    void update()
    {
        temp = env.getTemperature();
        pressure = env.getPressure();
        light_level = ldr.read();
    }
    
};

// Initialise struct
Data data;

// Function to sample the data
void sample_data(Data *data)
{
    while(1)
    {
        data->update();
        // Print the samples to the terminal
        printf("\n----- Sample %d -----\nTemperature:\t%3.1fC\nPressure:\t%4.1fmbar\nLight Level:\t%1.2f\n", sample_num,data->temp,data->pressure,data->light_level);

        sample_num++;

        ThisThread::sleep_for(std::chrono::seconds(10));
    }
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

void UpdateDisp()
{
    //Write the time and date on the LCD
    disp.cls();                                                      // Clear the LCD                 
    disp.locate(0,0);
    if (TimeDateSetting == 0)  
    {                                                     // Set LCD cursor to (0,0)
        disp.printf("Enter Year:");                                      // Write text to LCD
        
        disp.locate(1,0);                                                        // Set LCD cursor to (0,0)
        disp.printf("%d", year);                                        // Write text to LCD
        ThisThread::sleep_for(200ms);
    }
    else if (TimeDateSetting == 1)
    {
        disp.printf("Enter Month:");                                      // Write text to LCD
        
        disp.locate(1,0);                                                        // Set LCD cursor to (0,0)
        disp.printf("%d", month);                                        // Write text to LCD
        ThisThread::sleep_for(200ms);
    }
    else if (TimeDateSetting == 2)
    {
        disp.printf("Enter Day:");                                      // Write text to LCD
        
        disp.locate(1,0);                                                        // Set LCD cursor to (0,0)
        disp.printf("%d", day);                                        // Write text to LCD
        ThisThread::sleep_for(200ms);
    }
    else if (TimeDateSetting == 3)
    {
        disp.printf("Enter Hours:");                                      // Write text to LCD
        
        disp.locate(1,0);                                                        // Set LCD cursor to (0,0)
        disp.printf("%d", hour);                                        // Write text to LCD
        ThisThread::sleep_for(200ms);
    }
    else if (TimeDateSetting == 4)
    {
        disp.printf("Enter Minutes:");                                      // Write text to LCD
        
        disp.locate(1,0);                                                        // Set LCD cursor to (0,0)
        disp.printf("%d", minute);                                        // Write text to LCD
        ThisThread::sleep_for(200ms);
    }
    else if (TimeDateSetting == 5)
    {
        disp.printf("Enter Seconds:");                                      // Write text to LCD
        
        disp.locate(1,0);                                                        // Set LCD cursor to (0,0)
        disp.printf("%d", second);                                        // Write text to LCD
        ThisThread::sleep_for(200ms);
    }
}

void ButtonA_Monitor()
{
    while(1)
    {
        while(ButtonA==0){};
        ThisThread::sleep_for(100ms);
        while(ButtonA==1){};
        mutex1.lock();
    if (TimeDateSetting == 0)  
    {
        if (year < 2125)
        {                                                     // Set LCD cursor to (0,0)
            year++;
        }
        if (year == 2125)
        {
            year = 1970;
        }
    }
    else if (TimeDateSetting == 1)
    {
        if (month < 13)
        {                                                     // Set LCD cursor to (0,0)
            month++;
        }
        if (month == 13)
        {
            month = 1;
        }
    }
    else if (TimeDateSetting == 2)
    {
        if (day < 32)
        {                                                     // Set LCD cursor to (0,0)
            day++;
        }
        if (day == 32)
        {
            day = 1;
        }
    }
    else if (TimeDateSetting == 3)
    {
        if (hour < 24)
        {                                                     // Set LCD cursor to (0,0)
            hour++;
        }
        if (hour == 24)
        {
            hour = 0;
        }
    }
    else if (TimeDateSetting == 4)
    {
        if (minute < 60)
        {                                                     // Set LCD cursor to (0,0)
            minute++;
        }
        if (minute == 60)
        {
            minute = 0;
        }
    }
    else if (TimeDateSetting == 5)
    {
        if (second < 60)
        {                                                     // Set LCD cursor to (0,0)
            second++;
        }
        if (second == 60)
        {
            second = 0;
        }
    }
        UpdateDisp();
        mutex1.unlock();
        ThisThread::sleep_for(100ms);
    }
}

//void ButtonC_Monitor()
//{
   /// while(1)
   // {
       // while(ButtonC==0){};
       // ThisThread::sleep_for(100ms);
      //  while(ButtonC==1){};
       // mutex1.lock();
    //if (TimeDateSetting == 0)  
    //{                                                     // Set LCD cursor to (0,0)
       // if (year > 1969)
        //{                                                     // Set LCD cursor to (0,0)
         //   year--;
        //}
        //if (year == 1969)
        //{
       //     year = 2124;
      //  }
    //}
    //else if (TimeDateSetting == 1)
   // {
        //if (month > 0)
       // {                                                     // Set LCD cursor to (0,0)
         //   month--;
        //}
       // if (month == 0)
        //{
     //       month = 12;
       // }
    //}
    //else if (TimeDateSetting == 2)
   // {
        //if (day > 0)
       // {                                                     // Set LCD cursor to (0,0)
          //  day--;
        //}
       // if (day == 0)
       // {
      //      day = 31;
        //}
    //}
  //  else if (TimeDateSetting == 3)
   // {
      //  if (hour > -1)
       // {                                                     // Set LCD cursor to (0,0)
       //     hour--;
       // }
      //  if (hour == -1)
      //  {
      //      hour = 23;
     //   }
   // }
   // else if (TimeDateSetting == 4)
   // {
       // if (minute > -1)
       // {                                                     // Set LCD cursor to (0,0)
        //    minute--;
      //  }
      //  if (minute == -1)
      //  {
     //       minute = 59;
     //   }
    //}
    //else if (TimeDateSetting == 5)
   // {
        //if (second > -1)
        //{                                                     // Set LCD cursor to (0,0)
          //  second--;
        //}
       // if (second == -1)
       // {
     //       second = 59;
    //    }
   // }
       // UpdateDisp();
       // mutex1.unlock();
     //   ThisThread::sleep_for(100ms);
   // }
//}

void ButtonB_Monitor()
{
    while(1)
    {
        while(ButtonB==0){};
        ThisThread::sleep_for(200ms);
        while(ButtonB==1){};
        mutex2.lock();
        if (TimeDateSetting > 0)
        {
            TimeDateSetting--;

        }
        else if (TimeDateSetting == 0) 
        {
            TimeDateSetting = 5;
        }
        UpdateDisp();
        mutex2.unlock();
        ThisThread::sleep_for(200ms);
    }
}

void ButtonD_Monitor()
{
    while(1)
    {
        while(ButtonD==0){};
        ThisThread::sleep_for(200ms);
        while(ButtonD==1){};
        mutex2.lock();
        if (TimeDateSetting < 5)
        {
            TimeDateSetting++;

        }
        else if (TimeDateSetting == 5) 
        {
            TimeDateSetting = 0;
        }
        UpdateDisp();
        mutex2.unlock();
        ThisThread::sleep_for(200ms);
    }
}


time_t Datetime(int year, int mon, int mday, int hour, int min, int sec) 
{
    struct tm   t;
    time_t t_of_day;
    t.tm_year = year-1900;
    t.tm_mon =  (mon-1);        
    t.tm_mday = mday;
    t.tm_hour = hour;
    t.tm_min = min;
    t.tm_sec = sec;
    t.tm_isdst = -1;            // Is Daylight saving time on? 1 = yes, 0 = no, -1 = unknown
    t_of_day = mktime(&t);
 
    return t_of_day;          // returns seconds elapsed since January 1, 1970 (begin of the Epoch)
}


void BlueButton_Monitor()
{
    while(1)
    {
        while(ButtonC==0){};
        ThisThread::sleep_for(200ms);
        while(ButtonC==1){};
        // task 1 
        t1.start(callback(sample_data, &data));
        t1.set_priority(osPriorityRealtime);
        t2.terminate();
        t3.terminate();
        t4.terminate();
        t5.terminate();

        TimeDate = Datetime(year, month, day, hour, minute, second);

        // Set the time on the RTC (You can use https://www.epochconverter.com/ for testing)
        uint64_t now = (long) TimeDate;
        set_time(now);

        time_t time_now = time(NULL);   // Get a time_t timestamp from the RTC
        struct tm* tt;                  // Create empty tm struct
        tt = localtime(&time_now);      // Convert time_t to tm struct using localtime
        printf("%s\n",asctime(tt));     // Print in human readable format

        // Write the time and date on the LCD
        disp.cls();                                                             // Clear the LCD                 
        char lcd_line_buffer[17];           
        strftime(lcd_line_buffer, sizeof(lcd_line_buffer), "%a %d-%b-%Y", tt);  // Create a string DDD dd-MM-YYYY
        disp.locate(0,0);                                                       // Set LCD cursor to (0,0)
        disp.printf("%s",lcd_line_buffer);                                      // Write text to LCD
        
        strftime(lcd_line_buffer, sizeof(lcd_line_buffer), "     %H:%M", tt);   // Create a string HH:mm
        disp.locate(1,0);                                                       // Set LCD cursor to (0,0)
        disp.printf("%s",lcd_line_buffer);                                      // Write text to LCD

        start = 1;
        t6.terminate();
    }
}

// Define Variables 
char string_input[28]; 
char Sampling_value[6]; 
char date_value[10]; 
char time_value[9];
char select_value[4];

void Sampling()
{
    char Sampling_ON[5] = "[ON]";
    char Sampling_OFF[6] = "[OFF]";
    int sampling_on = strcmp(Sampling_ON, Sampling_value);
    int sampling_off = strcmp(Sampling_OFF, Sampling_value);
    if (sampling_on == 0)
        {
            printf("%s %s\n", string_input, Sampling_value);
            t1.start(callback(sample_data, &data));
            t1.set_priority(osPriorityRealtime);
            printf("%s %s\n", string_input, Sampling_value);
        }
    else if (sampling_off == 0)
        {
            printf("%s %s\n", string_input, Sampling_value);
            t1.terminate();
        }
    else 
    {
        printf("Invalid Input\n");
    }
}


void ReadTerminal()
{
    while(1)
    {
        scanf("%8s", string_input);
        int date_time_function = strcmp(string_input, "Datetime");
        int select_function = strcmp(string_input, "select");
        int Sampling_function = strcmp(string_input, "Sampling");
        int flush_function = strcmp(string_input, "flush");
        
        if (date_time_function == 0){
            scanf("%9s", date_value);
            scanf("%9s", time_value);
            printf("%s %s %s\n", string_input, date_value, time_value);
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
            //printf("%s %s\n", string_input, Sampling_value);
        }
        else if (flush_function == 0)
        {
            printf("%s\n", string_input);
        }
        else
        {
            printf("Invalid Input\n");
        }

        ThisThread::sleep_for(200ms);
    }
}







int main()
{

    printf("Set the time and date using buttons A,B,C and D...\n");                       


    //Write the time and date on the LCD
    disp.cls();                                                      // Clear the LCD                 
    disp.locate(0,0);                                                       // Set LCD cursor to (0,0)
    disp.printf("Enter Year:");                                      // Write text to LCD
    
    disp.locate(1,0);                                                        // Set LCD cursor to (0,0)
    disp.printf("%d", year);                                        // Write text to LCD
    

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

        if (start == 1)
        {
            // Print the time and date to terminal
            time_t time_now = time(NULL);   // Get a time_t timestamp from the RTC
            struct tm* tt;                  // Create empty tm struct
            tt = localtime(&time_now);      // Convert time_t to tm struct using localtime
            printf("%s\n",asctime(tt));     // Print in human readable format

            // Write the time and date on the LCD
            disp.cls();                                                             // Clear the LCD                 
            char lcd_line_buffer[17];           
            strftime(lcd_line_buffer, sizeof(lcd_line_buffer), "%a %d-%b-%Y", tt);  // Create a string DDD dd-MM-YYYY
            disp.locate(0,0);                                                       // Set LCD cursor to (0,0)
            disp.printf("%s",lcd_line_buffer);                                      // Write text to LCD
            
            strftime(lcd_line_buffer, sizeof(lcd_line_buffer), "     %H:%M", tt);   // Create a string HH:mm
            disp.locate(1,0);                                                       // Set LCD cursor to (0,0)
            disp.printf("%s",lcd_line_buffer);                                      // Write text to LCD
        }


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
        else{
            latchedLEDs.write_strip(0x0,RED);           // Turn off all LEDs on RGB bar
            latchedLEDs.write_strip(0x0,GREEN);
            latchedLEDs.write_strip(0x0,BLUE);
        }

        // Wait total of 10 seconds before next sample
        ThisThread::sleep_for(std::chrono::seconds(10));
    }
}

