#include "main.h"


void Delay700ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 32;
	j = 236;
	k = 16;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
void Delay5ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 59;
	j = 90;
	do
	{
		while (--j);
	} while (--i);
}

void main()
{
	unsigned char xdata screen=0;
	unsigned char xdata screen_huixian=0;
	
	unsigned char xdata work_flag=0;
	
	int xdata temp_param=30;
	int xdata trig_count=0;
	unsigned char xdata trig_250ms_count=0;
	unsigned char xdata trig_250ms_count_last=0;
	unsigned int xdata shidu=0;
	unsigned int xdata shidu_max=0;
	unsigned long int xdata temperature_int_avg=0;
	unsigned long int xdata temperature_int_all=0;
	unsigned long int xdata shidu_avg=0;
	unsigned long int xdata shidu_all=0;
	unsigned int xdata temperature_int_last=0;
	unsigned int xdata shidu_last=0;
	
	unsigned char xdata adc_read_level=0;
	unsigned char xdata adc_read_level_pin0=0;
	unsigned char xdata adc_read_level_pin3=0;
	
	bit light_state_now=0;
	bit light_state_before=0;
	unsigned char xdata time_trig[3]={0,0,0};
	unsigned char xdata time_trig_clear[3]={0,0,0};
	
	bit led_blink=0;
	
	unsigned char xdata diaodian_count=0;
	unsigned char xdata diaodian_flag=0;
	
	rend_temp();
	Delay700ms();
	rend_temp();
	Timer0Init();
	Timer1Init();
	PCAInit();
	UartInit();
	//TimerPCAInit();
	write_ds1302();
	//µôµçÍ³¼Æ
	e2prom_read(0xa0,&diaodian_flag,1);Delay5ms();
	if(diaodian_flag==0xbb)
	{
		e2prom_read(0xa1,&diaodian_count,1);Delay5ms();
		diaodian_count++;
		e2prom_write(0xa1,&diaodian_count,1);Delay5ms();
	}
	else
	{
		diaodian_count=0;
		diaodian_flag=0xbb;
		e2prom_write(0xa0,&diaodian_flag,1);Delay5ms();
		e2prom_write(0xa1,&diaodian_count,1);Delay5ms();
	}
	while(1)
	{
		if(timer_10ms>=10)
		{
			timer_10ms=0;
			//input
			key_pad_scan();
			//process
			//hign
			if(timer_50ms>=30)
			{
				timer_50ms=0;
				
				if(work_flag==0)
				{
					read_ds1302();
				}
				else if(work_flag==1)
				{
					rend_temp();
					temperature_int=temperature_int/10;
					temperature_int=temperature_int*10;
				}
				else if(work_flag==2)
				{
					light_state_before=light_state_now;
					adc_read_level=read_adc(0x01);
					if(adc_read_level>120)light_state_now=0;
					else light_state_now=1;
					
					if(light_state_before==0&&light_state_now==1&&timer_3000ms_start==0)
					{
						
						timer_3000ms_start=1;
						
						time_trig[0]=time_10[0];
						time_trig[1]=time_10[1];
						time_trig[2]=time_10[2];
						
						//
						trig_250ms_count++;
				
						if(freq>=200&&freq<=2000)
						{
							
							shidu=(unsigned int)((freq-200)*20.0/45.0+100);
							shidu=shidu/10;
							shidu=shidu*10;
							if(shidu_max<shidu)shidu_max=shidu;
							shidu_all+=shidu;
							
							
							led_buf&=~0x10;
						}
						else
						{
							shidu=0xff;
							led_buf|=0x10;
						}
						
						if(temperature_int_max<temperature_int)temperature_int_max=temperature_int;
						temperature_int_all+=temperature_int;
						
						shidu_avg=shidu_all/trig_250ms_count;
						temperature_int_avg=temperature_int_all/trig_250ms_count;
						
						
						//
						
						
						if(trig_250ms_count>=2)
						{
							if(temperature_int_last<temperature_int&&shidu_last<shidu)
							{
								led_buf|=0x20;
							}
							else
							{
								led_buf&=~0x20;
							}
						}
						
						if(freq>=200&&freq<=2000)shidu_last=shidu;
						temperature_int_last=temperature_int;
						trig_count++;
					}
				}
				else if(work_flag==3)
				{
					//Get_distance();
					read_echo();
					//read_distance();
					write_adc(adc_read_level_pin0);
				}
				else if(work_flag==4)
				{
					e2prom_read(0x50,time_trig_clear,3);
				}
				else if(work_flag==5)
				{
					adc_read_level_pin3=read_adc(0x03);
					adc_read_level_pin0=adc_read_level_pin3;
					led_pwm_duty=(unsigned char)(adc_read_level_pin3*(led_pwm_period)/255.0);
					relay_pwm_duty=(unsigned char)(adc_read_level_pin3*(relay_pwm_period)/255.0);
				}
				else if(work_flag==6)
				{
					if(uart_proc_flag!=0)
					{
						if(uart_proc_flag==1)
						{
							sprintf(uart_tx_buf,"x=%d,y=%d\r\n",(unsigned int)position_x,(unsigned int)position_y);
							send_string(uart_tx_buf);
							//memset(uart_tx_buf,0,sizeof(uart_tx_buf));
							strcpy(uart_tx_buf,"");
						}
						else if(uart_proc_flag==2)
						{
							sprintf(uart_tx_buf,"screen=%d\r\n",(unsigned int)screen);
							send_string(uart_tx_buf);
							//memset(uart_tx_buf,0,sizeof(uart_tx_buf));
							strcpy(uart_tx_buf,"");
							
						}
						else if(uart_proc_flag==3)
						{
							sprintf(uart_tx_buf,"pwm=%d\r\n",(unsigned int)led_pwm_duty);
							send_string(uart_tx_buf);
							uart_tx_buf[0]=0;
						}
						else if(uart_proc_flag==999)
						{
							sprintf(uart_tx_buf,"erro=%d\r\n",(unsigned int)uart_error_flag);
							send_string(uart_tx_buf);
							uart_tx_buf[0]=0;
							send_string(uart_rx_buf);
							uart_error_flag=0;
						}
						
						//
						uart_proc_flag=0;
					}
				}

				if(++work_flag>=7)work_flag=0;
			}
			
			//key
			
			if(key_state==255)
			{
				if(key_value==4)
				{
					if(++screen>=7)screen=0;
					if(screen==1)screen_huixian=0;
				}
				else if(key_value==5)
				{
					if(screen==1)
					{
						if(++screen_huixian>=3)screen_huixian=0;
					}
				}
				else if(key_value==8)
				{
					if(screen==2)
					{
						temp_param+=1;
						if(temp_param>99)temp_param=99;
					}
				}
				else if(key_value==9)
				{
					if(screen==2)
					{
						temp_param-=1;
						if(temp_param<0)temp_param=0;
					}
				}
			}
			else if(key_state==253)
			{
				if(key_value==9)
				{
					trig_count=0;

					shidu_avg=0;
					shidu_all=0;
					temperature_int_max=0;
					temperature_int_all=0;
					temperature_int_last=0;
					shidu_last=0;
					trig_250ms_count=0;
					
					
					e2prom_write(0x50,time_10,3);
				}
				else if(key_value==8)
				{
					diaodian_count=0;
					e2prom_write(0xa1,&diaodian_count,1);
				}
			}
			//proc

			if(timer_3000ms>=3000)
			{
				timer_3000ms=0;
				timer_3000ms_start=0;
				
				trig_250ms_count_last=trig_250ms_count;
				
			}
			
			
			if(timer_100ms>=100)
			{
				timer_100ms=0;
				led_blink=!led_blink;
			}
			
			//output
			//digs
			if(screen==0)dig_buf=digs_buf[0];
			if(screen==1&&screen_huixian==0)dig_buf=digs_buf[1];
			if(screen==1&&screen_huixian==1)dig_buf=digs_buf[2];
			if(screen==1&&screen_huixian==2)dig_buf=digs_buf[3];
			if(screen==2)dig_buf=digs_buf[4];
			if(screen==3)dig_buf=digs_buf[6];
			if(screen==4)dig_buf=digs_buf[7];
			if(screen==5)dig_buf=digs_buf[8];
			if(screen==6)dig_buf=digs_buf[9];
			if(timer_3000ms_start==1)dig_buf=digs_buf[5];
			
			if(screen==0&&trig_count%2==1&&timer_3000ms_start==0)
			{
				if(led_blink)
					dig_blink|=~0x24;
				else 
					dig_blink&=0x24;
			}
			else
			{
				dig_blink|=0xff;
			}
			digs_buf[0][0]=time_10[0]/10;
			digs_buf[0][1]=time_10[0]%10;
			digs_buf[0][3]=time_10[1]/10;
			digs_buf[0][4]=time_10[1]%10;
			digs_buf[0][6]=time_10[2]/10;
			digs_buf[0][7]=time_10[2]%10;
			
			if(trig_count==0)
			{
				digs_buf[1][2]=16;
				digs_buf[1][3]=16;
				digs_buf[1][4]=16;
				digs_buf[1][5]=16;
				digs_buf[1][6]=16;
				digs_buf[1][7]=16;
				
				digs_buf[2][2]=16;
				digs_buf[2][3]=16;
				digs_buf[2][4]=16;
				digs_buf[2][5]=16;
				digs_buf[2][6]=16;
				digs_buf[2][7]=16;
				
				digs_buf[3][1]=trig_count/10%10;
				digs_buf[3][2]=trig_count%10;
				digs_buf[3][3]=16;
				digs_buf[3][4]=16;
				digs_buf[3][5]=16;
				digs_buf[3][6]=16;
				digs_buf[3][7]=16;
			}
			else
			{
				digs_buf[1][2]=temperature_int_max/100%10;
				digs_buf[1][3]=temperature_int_max/10%10;
				digs_buf[1][4]=17;
				digs_buf[1][5]=temperature_int_avg/100%10;
				digs_buf[1][6]=temperature_int_avg/10%10+32;
				digs_buf[1][7]=temperature_int_avg%10;
				
				digs_buf[2][2]=shidu_max/100%10;
				digs_buf[2][3]=shidu_max/10%10;
				digs_buf[2][4]=17;
				digs_buf[2][5]=shidu_avg/100%10;
				digs_buf[2][6]=shidu_avg/10%10+32;
				digs_buf[2][7]=shidu_avg%10;
				
				digs_buf[3][1]=trig_count/10%10;
				digs_buf[3][2]=trig_count%10;
				digs_buf[3][3]=time_trig[0]/10%10;
				digs_buf[3][4]=time_trig[0]%10;
				digs_buf[3][5]=17;
				digs_buf[3][6]=time_trig[1]/10%10;
				digs_buf[3][7]=time_trig[1]%10;
			}
			digs_buf[4][2]=freq/100000%10;
			digs_buf[4][3]=freq/10000%10;
			digs_buf[4][4]=freq/1000%10;
			digs_buf[4][5]=freq/100%10;
			digs_buf[4][6]=freq/10%10;
			digs_buf[4][7]=freq%10;
			
//			digs_buf[3][2]=adc_read_level/100000%10;
//			digs_buf[3][3]=adc_read_level/10000%10;
//			digs_buf[3][4]=adc_read_level/1000%10;
//			digs_buf[3][5]=adc_read_level/100%10;
//			digs_buf[3][6]=adc_read_level/10%10;
//			digs_buf[3][7]=adc_read_level%10;
			
//			digs_buf[4][6]=temp_param/10%10;
//			digs_buf[4][7]=temp_param%10;
			
			digs_buf[5][3]=temperature_int/100%10;
			digs_buf[5][4]=temperature_int/10%10;
			if(shidu!=0xff)
			{
				digs_buf[5][6]=shidu/100%10;
				digs_buf[5][7]=shidu/10%10;
			}
			else
			{
				digs_buf[5][6]=10;
				digs_buf[5][7]=10;
			}
			
			digs_buf[6][0]=diaodian_count/10%10;
			digs_buf[6][1]=diaodian_count%10;
			digs_buf[6][2]=time_trig_clear[0]/10%10;
			digs_buf[6][3]=time_trig_clear[0]%10;
			digs_buf[6][4]=time_trig_clear[1]/10%10;
			digs_buf[6][5]=time_trig_clear[1]%10;
			digs_buf[6][6]=time_trig_clear[2]/10%10;
			digs_buf[6][7]=time_trig_clear[2]%10;
			
			digs_buf[7][1]=adc_read_level_pin3>=100?adc_read_level_pin3/100%10:16;
			digs_buf[7][2]=adc_read_level_pin3>=10?adc_read_level_pin3/10%10:16;
			digs_buf[7][3]=adc_read_level_pin3%10;
			
			digs_buf[7][4]=distance>=1000?distance/1000%10:16;
			digs_buf[7][5]=distance>=100?distance/100%10:16;
			digs_buf[7][6]=distance>=10?distance/10%10:16;
			digs_buf[7][7]=distance%10;
			
			digs_buf[8][1]=position_x>=100?position_x/100%10:16;
			digs_buf[8][2]=position_x>=10?position_x/10%10:16;
			digs_buf[8][3]=position_x%10;
			
			digs_buf[8][5]=position_y>=100?position_y/100%10:16;
			digs_buf[8][6]=position_y>=10?position_y/10%10:16;
			digs_buf[8][7]=position_y%10;
			
			digs_buf[9][1]=relay_pwm_duty*10>=100?relay_pwm_duty*10/100%10:16;
			digs_buf[9][2]=relay_pwm_duty*10>=10?relay_pwm_duty*10/10%10:16;
			digs_buf[9][3]=relay_pwm_duty*10%10;
			
			digs_buf[9][5]=adc_read_level>=100?adc_read_level/100%10:16;
			digs_buf[9][6]=adc_read_level>=10?adc_read_level/10%10:16;
			digs_buf[9][7]=adc_read_level%10;
			
			
			//led
			if(screen==0)led_buf|=0x01;
			else led_buf&=~0x01;
			
			if(screen==1)led_buf|=0x02;
			else led_buf&=~0x02;
			
			if(screen==2)led_buf|=0x04;
			else led_buf&=~0x04;
			
			if(temperature_int_last>temp_param*10)
			{
				if(led_blink)
				led_buf|=0x08;
				else
				led_buf&=~0x08;
			}
			else led_buf&=~0x08;
			
		}

	}
}