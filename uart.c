#include "uart.h"

void UartInit(void)		//9600bps@12.000MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	T2L = 0xC7;		//���ö�ʱ��ʼֵ
	T2H = 0xFE;		//���ö�ʱ��ʼֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
	EA=1;
	ES=1;
}

bit busy=0;
unsigned char xdata cmd=0;
unsigned char xdata uart_rx_buf[UART_LENGTH]={0};
unsigned char xdata uart_rx_index=0;

unsigned char xdata uart_tx_buf[UART_LENGTH]={0};
unsigned int xdata uart_proc_flag=0;//���ڴ�������־
unsigned int xdata uart_error_flag=0;//���ڴ�������־

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
		//���һ���ɱ��ַ�����ȡ����������
		//����һ��
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
		//����������ַ���ָ�������
		else if(buf=='#')
		{
			uart_proc_flag=2;
		}
		//����������ַ�����ʽָ�� �ж�֡ͷ֡β
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
