#include "Monitor_Buttons.hpp"
#include "UpdateDisp.hpp"
#include "Datetime.hpp"


// Inputs 
DigitalIn ButtonA(BTN1_PIN), ButtonB(BTN2_PIN), USER_Button(BUTTON1);
DigitalInOut ButtonC(BTN3_PIN,PIN_INPUT,PullDown,0), ButtonD(BTN4_PIN,PIN_INPUT,PullDown,0);


// Thread to monitor all the buttons
void Monitor_Buttons()
{
    while (true){
        // Wait for a button to be pressed
        while ((ButtonA == 0) && (ButtonB == 0) && (ButtonC == 0) && (ButtonD == 0) && (USER_Button == 0)){}
        ThisThread::sleep_for(20ms);          // Debounce (put thread to sleep for 20ms)

        // Button A
        if (ButtonA == 1){
            while (ButtonA == 1){}            // If button A was pressed, wait for it to be released
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
        }

        //Button C
        if (ButtonC == 1){

           while(ButtonC==1){};            // If button A was pressed, wait for it to be released
           mutex4.lock();                  // Lock (global variable)
           menu_engaged = 1;               // Set flag
           mutex4.unlock();                // Unlock
    
           mutex1.lock();                  // Lock (global variables inside switch-statement cases)
           switch (TimeDateSetting){
               case 0:                                                   
                   if (year > 1969){       // Only years greater than 1970 can be displayed                                                    
                       year--;             // Decrement year 
                   }
                   if (year == 1969){
                       year = 2124;        // If not greater than 1969, loop to 2124
                   }
                   break;
               case 1:
                   if (month > 0){         // Only months greater than 0 can be diaplyed                                            
                       month--;            // Decrement month
                   }
                   if (month == 0){        // If not greater than 0, loop to 12
                       month = 12;
                   }
                   break;
               case 2:
                   if (day > 0){           // Only days greater than 0 can be displayed                                               
                       day--;              // Decrement day
                   }
                   if (day == 0){          // If not greater than 0, loop to 31
                       day = 31;
                   }
                   break;
               case 3:
                  if (hour > -1){         // Only hours greater than -1 can be displayed                                              
                       hour--;             // Decrement hour
                   }
                   if (hour == -1){        // If not greater than -1, loop to 23
                       hour = 23;
                   }
                   break;
               case 4:
                   if (minute > -1){       // Only minutes greater than -1 can be displayed                                              
                       minute--;           // Decrement minute
                   }
                   if (minute == -1){      // If not greater than -1, loop to 59
                       minute = 59;
                   }
                   break;
               case 5:
                   if (second > -1){       // Only seconds greater than -1 can be displayed                                            
                      second--;           // Decrement second
                  }
                   if (second == -1){      // If not greater than -1, loop to 59
                       second = 59;
                   }
                   break;
           }
           mutex1.unlock();                // Unlock
           Menu_UpdateDisp();                   // Call function to update the LCD display 
         } 

        // Button B
        if (ButtonB == 1){
            while (ButtonB == 1){}             // If button B was pressed, wait for it to be released
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
        }

        // Button D
        if (ButtonD == 1){
            while(ButtonD==1){};                // If button D was pressed, wait for it to be released
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
        }

        // USER button
        if (USER_Button == 1){
            while (USER_Button == 1){}         // If the USER button was pressed, wait for it to be released
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

        ThisThread::sleep_for(100ms);         // Put thread to sleep for 100ms
    }
}