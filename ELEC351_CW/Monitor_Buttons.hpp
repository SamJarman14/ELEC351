#ifndef _MONITOR_BUTTONS_HPP_
#define _MONITOR_BUTTONS_HPP_
#include "uop_msb.h"

void Monitor_Buttons();
extern Mutex mutex1, mutex2, mutex3, mutex4;
extern time_t TimeDate;
extern bool menu_engaged;

#endif