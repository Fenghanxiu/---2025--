#ifndef __ONEWIRE_H__
#define __ONEWIRE_H__
#include <STC15F2K60S2.H>
#include <intrins.h>
extern float xdata temperature;
extern unsigned int xdata temperature_int;
extern unsigned int xdata temperature_int_max;
void rend_temp();
#endif
