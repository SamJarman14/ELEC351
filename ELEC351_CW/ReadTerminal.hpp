#ifndef _READTERMINAL_HPP_
#define _READTERMINAL_HPP_
#include "uop_msb.h"

// Functions
void split(char *str, const char *sep, const char *res[], size_t n);
void Sampling();
void ReadTerminal();

// Global variables
//extern char string_input[31], Sampling_value[7], date_value[13], time_value[10], select_value[5]; 
extern bool stop_sampling;
extern int selection;

#endif