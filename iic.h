#ifndef __IIC_H__
#define __IIC_H__
#include <STC15F2K60S2.H>
#include <intrins.h>
unsigned char read_adc(unsigned char channal);
void write_adc(unsigned char adc_level);
void stop_adc();

void e2prom_write(unsigned char addr,unsigned char *DATA,unsigned char len );
void e2prom_read(unsigned char addr,unsigned char *DATA,unsigned char len );
#endif
