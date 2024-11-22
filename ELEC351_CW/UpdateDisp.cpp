#include "UpdateDisp.hpp"

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