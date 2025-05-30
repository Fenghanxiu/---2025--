/*	# 	DS1302代码片段说明
	1. 	本文件夹中提供的驱动代码供参赛选手完成程序设计参考。
	2. 	参赛选手可以自行编写相关代码或以该代码为基础，根据所选单片机类型、运行速度和试题
		中对单片机时钟频率的要求，进行代码调试和修改。
*/								
#include "ds1302.h"
sbit SCK = P1^7;
sbit SDA = P2^3;
sbit RST = P1^3;

//
void Write_Ds1302(unsigned  char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	
	{ 
		SCK = 0;
		SDA = temp&0x01;
		temp>>=1; 
		SCK=1;
	}
}   

//
void Write_Ds1302_Byte( unsigned char address,unsigned char dat )     
{
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1; 	_nop_();  
 	Write_Ds1302(address);	
 	Write_Ds1302(dat);		
 	RST=0; 
}

//
unsigned char Read_Ds1302_Byte ( unsigned char address )
{
 	unsigned char i,temp=0x00;
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1;	_nop_();
 	Write_Ds1302(address);
 	for (i=0;i<8;i++) 	
 	{		
		SCK=0;
		temp>>=1;	
 		if(SDA)
 		temp|=0x80;	
 		SCK=1;
	} 
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
	SCK=1;	_nop_();
	SDA=0;	_nop_();
	SDA=1;	_nop_();
	return (temp);			
}
//

unsigned char xdata time_16[3]={0x23,0x59,0x55};
unsigned char xdata time_10[3]={21,13,55};
void time_16_to_10()
{
	unsigned char i=0;
	for(i=0;i<3;i++)
	{
		time_10[i]=time_16[i]/0x10*10+time_16[i]%0x10;
	}
}

void time_10_to_16()
{
	unsigned char i=0;
	for(i=0;i<3;i++)
	{
		time_16[i]=time_10[i]/10*0x10+time_10[i]%10;
	}
}

void write_ds1302(void)
{
	time_10_to_16();
	Write_Ds1302_Byte(0x8e,0x00);
	
	Write_Ds1302_Byte(0x80,time_16[2]);
	Write_Ds1302_Byte(0x82,time_16[1]);
	Write_Ds1302_Byte(0x84,time_16[0]);
	
	Write_Ds1302_Byte(0x8e,0x80);
}

void read_ds1302(void)
{
	
	time_16[2]=Read_Ds1302_Byte(0x81);
	time_16[1]=Read_Ds1302_Byte(0x83);
	time_16[0]=Read_Ds1302_Byte(0x85);
	
	time_16_to_10();

}