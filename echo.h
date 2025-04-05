#ifndef __ECHO_H__
#define __ECHO_H__
#include <STC15F2K60S2.H>
#include <intrins.h>

extern unsigned int xdata distance;
void Get_distance();

void PCAInit();
void read_echo();

//void TimerPCAInit();
//void read_distance();

#endif
