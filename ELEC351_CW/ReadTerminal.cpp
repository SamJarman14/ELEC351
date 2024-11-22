#include "ReadTerminal.hpp"
#include "UpdateDisp.hpp"
#include "Monitor_Buttons.hpp"
#include "Datetime.hpp"


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