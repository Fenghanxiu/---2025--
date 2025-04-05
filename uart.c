#include "uart.h"

void UartInit(void)		//9600bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0xC7;		//设置定时初始值
	T2H = 0xFE;		//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时
	EA=1;
	ES=1;
}

bit busy=0;
unsigned char xdata cmd=0;
unsigned char xdata uart_rx_buf[UART_LENGTH]={0};
unsigned char xdata uart_rx_index=0;

unsigned char xdata uart_tx_buf[UART_LENGTH]={0};
unsigned int xdata uart_proc_flag=0;//串口处理结果标志
unsigned int xdata uart_error_flag=0;//串口错误结果标志

int xdata position_x=0;
int xdata position_y=0;
int xdata recv_x=0;
int xdata recv_y=0;
void Uart() interrupt 4
{
	unsigned char buf=0;
	
	if(TI)
	{
		TI=0;
		busy=0;
	}
	
	if(RI)
	{
		RI=0;
		
		buf=SBUF;
		
		uart_rx_buf[uart_rx_index]=buf;
		
		//proc
		//情况一：可变字符串提取不定长数据
		//方法一：
		//
		if(buf==')'&&uart_rx_buf[0]=='(')
		{
			if(sscanf(uart_rx_buf,"(%d,%d)",&recv_x,&recv_y)==2)
			{
				if(recv_x>=0&&recv_x<=999&&recv_y>=0&&recv_y<=999)
				{
					position_x=recv_x;
					position_y=recv_y;
					
					uart_proc_flag=1;
				}
				else
				{
					uart_proc_flag=999;
					uart_error_flag=1;
				}
			}
			else
			{
				uart_proc_flag=999;
				uart_error_flag=2;
			}
			
		}
		//情况二：单字符串指令，最简单情况
		else if(buf=='#')
		{
			uart_proc_flag=2;
		}
		//情况三：多字符串格式指令 判断帧头帧尾
		else if(uart_rx_buf[uart_rx_index-1]=='\r'&&uart_rx_buf[uart_rx_index]=='\n')
		{
			
			if(strcmp(uart_rx_buf,"pwm\r\n")==0)
			{
				uart_proc_flag=3;
			}
			else
			{
				uart_error_flag=3;
			}
		}
		else
		{
//			uart_proc_flag=999;
//			uart_error_flag=999;
		}
		
		uart_rx_index++;
		if(uart_proc_flag!=0)
		{
			uart_rx_index=0;
			uart_rx_buf[0]=0;
		}
		
		//
	}
}

void send_data(unsigned char dat)
{
	while(busy);
	busy=1;
	SBUF=dat;
}

void send_string(unsigned char *s)
{
	while(*s)
	{
		send_data(*s++);
	}
}
