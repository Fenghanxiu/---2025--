#ifndef __UART_H__
#define __UART_H__
#include <STC15F2K60S2.H>
#include <intrins.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define UART_LENGTH 30

extern unsigned char xdata uart_rx_buf[UART_LENGTH];
extern unsigned char xdata uart_rx_index;
extern unsigned char xdata uart_tx_buf[UART_LENGTH];

void UartInit(void);
void send_data(unsigned char dat);
void send_string(unsigned char *s);
extern unsigned int xdata uart_proc_flag;//串口处理结果标志
extern unsigned int xdata uart_error_flag;
extern int xdata position_x;
extern int xdata position_y;
#endif
