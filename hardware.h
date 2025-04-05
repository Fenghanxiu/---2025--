#ifndef __HARDWARE_H__
#define __HARDWARE_H__
#include <STC15F2K60S2.H>
#include <intrins.h>
//led
extern unsigned char xdata led_buf;
void led_output();
extern unsigned char xdata led_pwm_cnt;
extern unsigned char xdata led_pwm_duty;
extern unsigned char xdata led_pwm_period;
extern unsigned char xdata led_pwm_select;
//hign_power
extern unsigned char xdata hign_power_buf;
extern unsigned char xdata relay_pwm_cnt;
extern unsigned char xdata relay_pwm_duty;
extern unsigned char xdata relay_pwm_period;
void hign_power_output();
//digs
extern unsigned char xdata digs_buf[][8];
extern unsigned char xdata *dig_buf;
extern unsigned char xdata dig_com;
extern unsigned char xdata dig_blink;
extern void digs_output();

//key
extern unsigned char xdata key_value;
extern unsigned char xdata key_state;
void key_pad_scan();
void key_pad_scan2();
void key_scan();
//ne555
extern unsigned int xdata freq;
extern unsigned int xdata freq_cnt;
void Timer0Init(void);

//time
void Timer1Init(void);
extern unsigned int xdata timer_10ms;
extern unsigned int xdata timer_50ms;
extern unsigned int xdata timer_100ms;
extern unsigned int xdata timer_3000ms;
extern unsigned char xdata timer_3000ms_start;
#endif
