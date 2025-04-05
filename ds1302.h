#ifndef __DS1302_H__
#define __DS1302_H__
#include <STC15F2K60S2.H>
#include <intrins.h>
extern unsigned char xdata time_16[3];
extern unsigned char xdata time_10[3];
void write_ds1302(void);
void read_ds1302(void);
#endif
