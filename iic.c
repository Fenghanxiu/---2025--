/*	#   I2C代码片段说明
	1. 	本文件夹中提供的驱动代码供参赛选手完成程序设计参考。
	2. 	参赛选手可以自行编写相关代码或以该代码为基础，根据所选单片机类型、运行速度和试题
		中对单片机时钟频率的要求，进行代码调试和修改。
*/
#include "iic.h"

sbit scl = P2^0;
sbit sda = P2^1;
#define DELAY_TIME	10

//
static void I2C_Delay(unsigned char n)
{
    do
    {
        _nop_();_nop_();_nop_();_nop_();_nop_();
        _nop_();_nop_();_nop_();_nop_();_nop_();
        _nop_();_nop_();_nop_();_nop_();_nop_();		
    }
    while(n--);      	
}

//
void I2CStart(void)
{
    sda = 1;
    scl = 1;
	I2C_Delay(DELAY_TIME);
    sda = 0;
	I2C_Delay(DELAY_TIME);
    scl = 0;    
}

//
void I2CStop(void)
{
    sda = 0;
    scl = 1;
	I2C_Delay(DELAY_TIME);
    sda = 1;
	I2C_Delay(DELAY_TIME);
}

//
void I2CSendByte(unsigned char byt)
{
    unsigned char i;
	
    for(i=0; i<8; i++){
        scl = 0;
		I2C_Delay(DELAY_TIME);
        if(byt & 0x80){
            sda = 1;
        }
        else{
            sda = 0;
        }
		I2C_Delay(DELAY_TIME);
        scl = 1;
        byt <<= 1;
		I2C_Delay(DELAY_TIME);
    }
	
    scl = 0;  
}

//
unsigned char I2CReceiveByte(void)
{
	unsigned char da;
	unsigned char i;
	for(i=0;i<8;i++){   
		scl = 1;
		I2C_Delay(DELAY_TIME);
		da <<= 1;
		if(sda) 
			da |= 0x01;
		scl = 0;
		I2C_Delay(DELAY_TIME);
	}
	return da;    
}

//
unsigned char I2CWaitAck(void)
{
	unsigned char ackbit;
	
    scl = 1;
	I2C_Delay(DELAY_TIME);
    ackbit = sda; 
    scl = 0;
	I2C_Delay(DELAY_TIME);
	
	return ackbit;
}

//
void I2CSendAck(unsigned char ackbit)
{
    scl = 0;
    sda = ackbit; 
	I2C_Delay(DELAY_TIME);
    scl = 1;
	I2C_Delay(DELAY_TIME);
    scl = 0; 
	sda = 1;
	I2C_Delay(DELAY_TIME);
}

//

unsigned char read_adc(unsigned char channal)
{
	unsigned char temp=0;
	
	I2CStart();
	I2CSendByte(0x90);
	I2CWaitAck();
	
	I2CSendByte(0x40|channal);
	I2CWaitAck();
	
	I2CStart();
	I2CSendByte(0x91);
	I2CWaitAck();
	
	I2CReceiveByte();
	I2CSendAck(0);
	
	temp=I2CReceiveByte();
	I2CSendAck(1);
	I2CStop();
	
	return temp;
}

void write_adc(unsigned char adc_level)
{
	I2CStart();
	I2CSendByte(0x90);
	I2CWaitAck();
	
	I2CSendByte(0x40);
	I2CWaitAck();
	
	I2CSendByte(adc_level);
	I2CWaitAck();
	
	I2CStop();
}

void stop_adc()
{
	I2CStart();
	I2CSendByte(0x90);
	I2CWaitAck();
	
	I2CSendByte(0x00);
	I2CWaitAck();
	
	I2CStop();
}

//eeprom
void e2prom_write(unsigned char addr,unsigned char *DATA,unsigned char len )
{
	I2CStart();
	I2CSendByte(0xa0);
	I2CWaitAck();
	
	I2CSendByte(addr);
	I2CWaitAck();
	while(len--)
	{
		I2CSendByte(*DATA);
		I2CWaitAck();
		DATA++;
	}
	I2CStop();
}

void e2prom_read(unsigned char addr,unsigned char *DATA,unsigned char len )
{
	I2CStart();
	I2CSendByte(0xa0);
	I2CWaitAck();
	
	I2CSendByte(addr);
	I2CWaitAck();
	
	I2CStart();
	I2CSendByte(0xa1);
	I2CWaitAck();
	
	*DATA=I2CReceiveByte();
	loop:
	if(--len)
	{
		I2CSendAck(0);
		DATA++;
		*DATA=I2CReceiveByte();
		goto loop;
	}
	I2CSendAck(1); 
	I2CStop();
}
