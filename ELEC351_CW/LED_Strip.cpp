////////////// Sam Jarman //////////////
//////////////  10704486  //////////////

#include "LED_Strip.hpp"
#include "Sample_Mailbox.hpp"
#include "ReadTerminal.hpp"

void flash_and_buzz(){
    buzz.playTone("C");                     // Buzzer on
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


void LED_Strip(int selection){

    latchedLEDs.enable(true);

    // If the light level has been selected to be displayed by the user (this is also the default)
    if (selection == 0){
        // Write the current light level as a float to the 7-segment display
        latchedLEDs.write_seven_seg(Light_Level);
        // Quantise the light level value into 24 levels
        int32_t value = Light_Level/(0.9/24);
        // Bit-shift the number of LED's we want turned on, minus 1 
        value = (1 << value) - 1;

        // Turn on the required LED's
        if (value <= 255){
            latchedLEDs.write_strip(value,RED);
            latchedLEDs.write_strip(0x0,GREEN);
            latchedLEDs.write_strip(0x0,BLUE);
        }
        else if ((value > 255) && (value <= 65535)){
            latchedLEDs.write_strip(0xFF,RED);
            value = value >> 8;
            latchedLEDs.write_strip(value,GREEN);
            latchedLEDs.write_strip(0x0,BLUE);
        }
        else if ((value > 65535) && (value <= 16777215)){
            latchedLEDs.write_strip(0xFF,RED);
            latchedLEDs.write_strip(0xFF,GREEN);
            value = value >> 16;
            latchedLEDs.write_strip(value,BLUE); 
        }
        else if (value > 16777215){
            for(int i=0;i<4;i++){
                flash_and_buzz();
            }
        }
    }

    // If the temperature has been selected to be displayed by the user 
    else if (selection == 1){
        // Write the current temperature as a float to the 7-segment display
        latchedLEDs.write_seven_seg(Temperature);
        // Quantise the temperature value into 24 levels
        int32_t value = Temperature/(100.0/24);
        // Bit-shift the number of LED's we want turned on, minus 1 
        value = (1 << value) - 1;

        // Turn on the required LEDs' 
        if (value <= 255){
            latchedLEDs.write_strip(value,RED);
            latchedLEDs.write_strip(0x0,GREEN);
            latchedLEDs.write_strip(0x0,BLUE);
        }
        else if ((value > 255) && (value <= 65535)){
            latchedLEDs.write_strip(0xFF,RED);
            value = value >> 8;
            latchedLEDs.write_strip(value,GREEN);
            latchedLEDs.write_strip(0x0,BLUE);
        }
        else if ((value > 65535) && (value <= 16777215)){
            latchedLEDs.write_strip(0xFF,RED);
            latchedLEDs.write_strip(0xFF,GREEN);
            value = value >> 16;
            latchedLEDs.write_strip(value,BLUE); 
        }
        else if (value > 16777215){
            for(int i=0;i<4;i++){
                flash_and_buzz();
            }
        }
    }

    // If the pressure has been selected to be displayed by the user
    else if (selection == 2){
        // Pressure value is too big to be written on the 7-segment display, so write 00
        float a = 0.0;
        latchedLEDs.write_seven_seg(a);
        // Quantise the pressure value into 24 levels
        // Scale is 1 to 1000 but I divide by 2000 so the regular pressure is in the middle of the LED strip
        int32_t value = Pressure/(2000.0/24);
        // Bit-shift the number of LED's we want turned on, minus 1 
        value = (1 << value) - 1;

        // Turn on the required LED's 
        if (value <= 255){
            latchedLEDs.write_strip(value,RED);
            latchedLEDs.write_strip(0x0,GREEN);
            latchedLEDs.write_strip(0x0,BLUE);
        }
        else if ((value > 255) && (value <= 65535)){
            latchedLEDs.write_strip(0xFF,RED);
            value = value >> 8;
            latchedLEDs.write_strip(value,GREEN);
            latchedLEDs.write_strip(0x0,BLUE);
        }
        else if ((value > 65535) && (value <= 16777215)){
            latchedLEDs.write_strip(0xFF,RED);
            latchedLEDs.write_strip(0xFF,GREEN);
            value = value >> 16;
            latchedLEDs.write_strip(value,BLUE); 
        }
        else if (value > 16777215){
            for(int i=0;i<4;i++){
                flash_and_buzz();
            }
        }
    }
}