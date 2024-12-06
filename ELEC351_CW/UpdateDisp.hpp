#ifndef _UPDATEDISP_HPP_
#define _UPDATEDISP_HPP_
#include "uop_msb.h"

// Functions
void DateTime_UpdateDisp(struct tm* tt);
void Menu_UpdateDisp();

// Global variables
extern int TimeDateSetting, year, month, day, hour, minute, second;

#endif