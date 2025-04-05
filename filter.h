#ifndef __ECHO_H__
#define __ECHO_H__
#include <STC15F2K60S2.H>
#include <intrins.h>

#define MAX(a,b) (a>b?a:b)
#define MIN(a,b) (a<b?a:b)
void swap(unsigned int *x,unsigned int *y);
unsigned int median_filter(unsigned int *Data);
unsigned int average_filter(unsigned int *Data, unsigned int len);

unsigned int med_filter(unsigned int *Data,unsigned int new_data);
unsigned int ave_filter(unsigned int *Data,unsigned int new_data);

void sort(unsigned int *Data, unsigned int len);
#endif