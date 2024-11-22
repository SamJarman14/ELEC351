#include "Datetime.hpp"

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