////////////// Sam Jarman //////////////
//////////////  10704486  //////////////

#include "ReadTerminal.hpp"
#include "UpdateDisp.hpp"
#include "Monitor_Buttons.hpp"
#include "Datetime.hpp"
#include "Sample_Mailbox.hpp"
#include "LED_Strip.hpp"

// Variables
char string_input[31], Sampling_value[7], date_value[13], time_value[10], select_value[5]; 

// Function to split string at a desired character
// e.g. can split 'hello/world' at "/" to get two separate strings 'hello' and 'world'
void split(char *str, const char *sep, const char *res[], size_t n)
{
    for (size_t i = 0; i < n; i++) {
        size_t len = strcspn(str, sep);    
        res[i] = str; 
        str += len;
        if (*str) *str++ = '\0';
    }
}

// Function to turn on or off sampling based on the users input
void Sampling()
{
    // Initialise strings to compare with the user input
    char Sampling_ON[5] = "[ON]";
    char Sampling_OFF[6] = "[OFF]";

    // Compare with the user input 
    int sampling_on = strcmp(Sampling_ON, Sampling_value);
    int sampling_off = strcmp(Sampling_OFF, Sampling_value);

    // If Sampling [ON] was entered
    if (sampling_on == 0){
        // Disable flag to enable sampling
        stop_sampling = 0;
        printf("%s %s\n", string_input, Sampling_value);
    }
    // If Sampling [OFF] was entered
    else if (sampling_off == 0){
        // Enable flag to stop sampling
        stop_sampling = 1;
        printf("%s %s\n", string_input, Sampling_value);
    }
    else{
        // If an incorrect input was entered by the user
        printf("Invalid Input\n");
    }
    ThisThread::sleep_for(100ms);
}

// Function to change with sampling variable to display on the LED strip
void Select()
{
    // Initialise strings to compare with the user input
    char Select_L[4] = "[L]";
    char Select_T[4] = "[T]";
    char Select_P[4] = "[P]";

    // Compare with the user input 
    int select_L = strcmp(Select_L, select_value);
    int select_T = strcmp(Select_T, select_value);
    int select_P = strcmp(Select_P, select_value);

    // If select [L] was entered
    if (select_L == 0){
        printf("%s %s\n", string_input, select_value);
        // Change the selection variable and run function to update the LED strip
        selection = 0;
        LED_Strip(selection);
    }
    // If select [T] was entered
    else if (select_T == 0){
        printf("%s %s\n", string_input, select_value);
        // Change the selection variable and run function to update the LED strip
        selection = 1;
        LED_Strip(selection);
    }
    // If select [P] was entered
    else if (select_P == 0){
        printf("%s %s\n", string_input, select_value);
        // Change the selection variable and run function to update the LED strip
        selection = 2;
        LED_Strip(selection);
    }
    // If an incorrect input was entered by the user
    else{
        printf("Invalid Input\n");
    }
    ThisThread::sleep_for(100ms);
}

// Thread to read user inputs via the terminal
void ReadTerminal()
{
    while(true)
    {
        // Compare first 8 characters of input to each potential function name
        scanf("%8s", string_input);
        bool date_time_function = strcmp(string_input, "Datetime");
        bool select_function = strcmp(string_input, "select");
        bool Sampling_function = strcmp(string_input, "Sampling");
        bool flush_function = strcmp(string_input, "flush");
        
        // If the Datetime function is entered, read the next 18 characters in two sections (sperate date and time sections)
        if (date_time_function == 0){
            scanf("%11s", date_value);
            scanf("%9s", time_value);
            // Print all setions together to show the users input
            printf("Date and Time have been changed to: %s %s %s\n", string_input, date_value, time_value);

            // Check that the first and last characters entered are '[' and ']'
            char Compare_1 = '[';
            char Compare_2 = ']';
            // Set check to 1, we will see if this is still 1 at the end of all checks 
            bool check = 1;
            if (Compare_1 == date_value[0])
            // Do nothing, check passed (check variable still equals 1)
            {}
            else 
            {
                // If the first character of the date string isn't '[' then an invalid input was entered
                check = 0;
            }
            if (Compare_2 == time_value[8])
            // Do nothing, check passed (check variable still equals 1)
            {}
            else 
            {
                // If the last character of the time string isn't ']' then an invalid input was entered
                check = 0;
            }

            // Remove first character of date string (remove '[')
            memmove(date_value, date_value + 1, sizeof date_value - 1);

            // Remove last character of time string (remove ']')
            time_value[8] = '\0';

            // Split the date section into seperate tokens whenever a '/' is detected
            int n = 3;
            const char *token1[n];
            split(date_value, "/", token1, n);

            // Split the time section into seperate tokens whenever a ':' is detected
            const char *token2[n];
            split(time_value, ":", token2, n);

            // Checking the right amount of characters were entered for each part of the date (xx/xx/xxxx)
            // Checking the first two tokens are made up of 2 characters
            for (int x = 0; x < 2; x++)
            {
                if (strlen(token1[x]) == 2) 
                {}
                else 
                {
                    check = 0;
                }
            }
            // Checking the last token is made up of 4 characters
            if (strlen(token1[2]) == 4)
            {}
            else 
            {
                check = 0;
            }

            // Checking the right amount of characters were entered for each part of the time (xx/xx/xx)
            // Checking all three tokens are made up of 2 characters
            for (int x = 0; x < 3; x++)
            {
                if (strlen(token2[x]) == 2)
                {}
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
            // Check all values entered are within the min-max range for each variable
            // If a non-integer was inputted or a value outside of the min-max, check will equal 0 
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

            // If 'check' still equals 1 then a valid input was entered - run Datetime function
            if (check == 1)
            {   
                mutex3.lock();
                TimeDate = Datetime(year, month, day, hour, minute, second);
                //Set the time on the RTC 
                uint64_t now = (long) TimeDate;
                set_time(now);
                time_t time_now = time(NULL);   // Get a time_t timestamp from the RTC
                struct tm* tt;                  // Create empty tm struct
                tt = localtime(&time_now);      // Convert time_t to tm struct using localtime
                mutex3.unlock();
                printf("%s\n",asctime(tt));     // Print in human readable format

                DateTime_UpdateDisp(tt);        // Call function to update the date and time on the LCD display
            }
            // If 'check' no longer equals 1, an invalid input was entered
            else 
            {
                printf("Invalid Input\n");
            }
        }
        // If the select function was entered, run the select function
        else if (select_function == 0)
        {
            scanf("%3s", select_value);
            Select();
        }
        // If the sampling function was entered, run the sampling function
        else if (Sampling_function == 0)
        {
            scanf("%5s", Sampling_value);
            Sampling();
        }
        // If the flush function was entered, empty the mailbox
        else if (flush_function == 0)
        {
            printf("%s\n", string_input);
            empty_Mailbox();
        }
        // If an incorrect function was entered, the user didn't enter a valid input
        else
        {
            printf("Invalid Input\n");
        }
        ThisThread::sleep_for(100ms);               // Put this thread to sleep for 100ms
    }
}