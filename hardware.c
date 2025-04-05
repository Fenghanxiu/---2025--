#include "hardware.h"

//led
unsigned char xdata led_buf=0;
unsigned char xdata led_pwm_cnt=0;
unsigned char xdata led_pwm_duty=10;
unsigned char xdata led_pwm_period=10;
unsigned char xdata led_pwm_select=0x00;
void led_output()
{
	P0=~led_buf;
	P2|=0x80;
	P2&=0x1f;
}
void led_pwm_output()
{
	P0=~led_pwm_select;
	P2|=0x80;
	P2&=0x1f;
}
//hign_power
unsigned char xdata hign_power_buf=0;
unsigned char xdata relay_pwm_cnt=0;
unsigned char xdata relay_pwm_duty=10;
unsigned char xdata relay_pwm_period=10;
void hign_power_output()
{
	P0=hign_power_buf;
	P2|=0xa0;
	P2&=0x1f;
}

//digs
/*************  ���س�������    **************/
unsigned char code t_display[]={                       //��׼�ֿ�
//   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
//black  -     H    J    K    L    N    o   P    U     t    G    Q    r   M    y
    0x00,0x40,0x76,0x1E,0x70,0x38,0x37,0x5C,0x73,0x3E,0x78,0x3d,0x67,0x50,0x37,0x6e,
    0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0x46};    //0. 1. 2. 3. 4. 5. 6. 7. 8. 9. -1

unsigned char code T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};      //λ��

unsigned char xdata digs_buf[][8]=
{
	{16,16,17,16,16,17,16,16,},//ʱ��
	{12,16,16,16,17,16,16,16,},//�����¶�
	{18,16,16,16,17,16,16,16,},//ʪ�Ȼ���
	{15,16,16,16,16,17,16,16,},//ʱ�����
	{24,16,16,16,16,16,16,16,},//��������
	{14,16,16,16,16,16,16,16,},//ʪ�¶Ƚ���
	{1,16,16,16,16,16,16,16,},//��������봥��ʱ��
	{2,16,16,16,16,16,16,16,},//����������
	{3,16,16,16,17,16,16,16,},//���ڽ���
	{4,16,16,16,17,16,16,16,},//dac/adc����
};

unsigned char xdata *dig_buf=digs_buf[0];
unsigned char xdata dig_com=0x00;
unsigned char xdata dig_blink=0xff;


void digs_output()
{
	P0=0x00;
	P2|=0xc0;
	P2&=0x1f;
	
	P0=~t_display[dig_buf[dig_com]];
	P2|=0xe0;
	P2&=0x1f;
	
	P0=T_COM[dig_com]&dig_blink;
	P2|=0xc0;
	P2&=0x1f;
	
	if(++dig_com>=8)dig_com=0;
}

//key

unsigned char code key_pad_values[4][4]=
{
	{7,11,15,19},
	{6,10,14,18},
	{5,9,13,17},
	{4,8,12,16},
};
unsigned char xdata key_value=0;
unsigned char xdata key_state=0;

void key_pad_scan()
{
	unsigned char r=255,c=255;
	
	//P30=1;
	//P31=1;
	P32=1;
	P33=1;
	
	P44=0;
	P42=0;
	P35=0;
	//P34=0;
	
	if(~P3&0x0f)
	{
		//if(!P30)r=0;
		//if(!P31)r=1;
		if(!P32)r=2;
		if(!P33)r=3;
		
		//P30=0;
		//P31=0;
		P32=0;
		P33=0;
		
		P44=1;
		P42=1;
		P35=1;
		//P34=1;
		
		if(!P44)c=0;
		if(!P42)c=1;
		if(!P35)c=2;
		//if(!P34)c=3;
		
		
		
		if(r!=255&&c!=255)
		{
			key_value=key_pad_values[r][c];
		}
		else
		{
			key_value=0;key_state=0;
		}
		//
		if(key_state<250)key_state++;
	}
	else
	{
		if(key_value&&key_state <= 250)
		{
			if (key_state < 100) //�����µ�����ʱ��û�г���1��
				key_state = 255;
			else if (key_state < 200) //�����µ�����ʱ�䳬��1���û����2��
				key_state = 254;
			else //�����µ�����ʱ�䳬��2��
				key_state = 253;
		}
		else
		{
			key_value=0;key_state=0;
		}
	}
	
}

void key_pad_scan2()
{
	unsigned char row=255,column=255;
	#ifndef USE_UART //�й���Щ#if��������main.h������ҵ�
	P30 = 1; //��1��
	P31 = 1; //��2��
	#endif //USE_UART
	P32 = 1; //��3��
	P33 = 1; //��4��
	
	P44 = 0; //��1��
	#ifndef MM_MODE
	P42 = 0; //��2��
	#endif //MM_MODE
	P35 = 0; //��3��
	#ifndef USE_NE555
	P34 = 0; //��4��
	#endif //USE_NE555
	if(~P3&0x0f)
	{
		/* ������ֵ�ж� */
		if (!P30) row = 0;
		if (!P31) row = 1;
		if (!P32) row = 2;
		if (!P33) row = 3;
		
		P3 = 0xF0;
		P4 = 0xFF;
		

		P30 = 0; //��1��
		P31 = 0; //��2��
		P32 = 0; //��3��
		P33 = 0; //��4��
		
		P44 = 1; //��1��
		P42 = 1; //��2��
		P35 = 1; //��3��
#ifndef USE_NE555
		P34 = 1; //��4��
#endif //USE_NE555
		if (!P34) column = 3;
		if (!P35) column = 2;
		if (!P42) column = 1;
		if (!P44) column = 0;
		
				/* ��ֵ�ж� */
		if (row != 255 && column != 255) //���������Ч
		{
			key_value = key_pad_values[row][column];
		}
		
		/* ������ʱ */
		if (key_state < 250)
			key_state++;
	}
	else
	{
		if (key_value && key_state <= 250) //����������ɿ�
		{
			if (key_state < 100) //�����µ�����ʱ��û�г���1��
				key_state = 255;
			else if (key_state < 200) //�����µ�����ʱ�䳬��1���û����2��
				key_state = 254;
			else //�����µ�����ʱ�䳬��2��
				key_state = 253;
		}
		else //������ȫ�ɿ�
		{
			key_value = 0;
			key_state = 0;
		}
	}
}
//���������������Ϻ;���˼·һ����ֻ����ֻ��Ҫɨ��һ��
void key_scan()
{
	unsigned char c=255;
	
	P30=1;
	P31=1;
	P32=1;
	P33=1;
	
	if(~P3&0x0f)
	{
		if(!P30)c=7;
		if(!P31)c=6;
		if(!P32)c=5;
		if(!P33)c=4;
		
		if(c!=255)
		{
			key_value=c;
		}
		else
		{
			key_value=0;key_state=0;
		}
		
		if(key_state<250)key_state++;
	}
	else
	{
		if(key_value&&key_state<=250)
		{
			if(key_state>100)
			{
				key_state=255;
			}
			else
			{
				key_state=254;
			}
		}
		else
		{
			key_value=0;key_state=0;
		}
	}
}
//ne555
unsigned int xdata freq=0;
unsigned int xdata freq_cnt=0;

void Timer0Init(void)		//100΢��@12.000MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x04;		//���ö�ʱ��ģʽ
	TL0 = 0x00;		//���ö�ʱ��ʼֵ
	TH0 = 0x00;		//���ö�ʱ��ʼֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	EA=1;
	ET0=1;
}

//time
void Timer1Init(void)		//100΢��@12.000MHz
{
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x50;		//���ö�ʱ��ʼֵ
	TH1 = 0xFB;		//���ö�ʱ��ʼֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
	EA=1;
	ET1=1;
}
//void Timer1Init(void)		//1����@12.000MHz
//{
//	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
//	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
//	TL1 = 0x20;		//���ö�ʱ��ʼֵ
//	TH1 = 0xD1;		//���ö�ʱ��ʼֵ
//	TF1 = 0;		//���TF1��־
//	TR1 = 1;		//��ʱ��1��ʼ��ʱ
//	EA=1;
//	ET1=1;
//}


unsigned int xdata timer_1ms=0;
unsigned int xdata timer_10ms=0;
unsigned int xdata timer_50ms=0;
unsigned int xdata timer_100ms=0;
unsigned int xdata timer_3000ms=0;
unsigned char xdata timer_3000ms_start=0;
void Timer1(void) interrupt 3
{
	timer_1ms++;
	
	
	if (led_pwm_cnt < led_pwm_duty) //��Ч��ƽ
	{
		led_buf|=0x80;
	}
	else
	{
		led_buf&=~0x80;
	}
	led_output();
	
	if(++led_pwm_cnt>=led_pwm_period)led_pwm_cnt=0;
	//
	if (relay_pwm_cnt < relay_pwm_duty) //��Ч��ƽ
	{
		hign_power_buf|=0x20;
	}
	else
	{
		hign_power_buf&=~0x20;
	}
	hign_power_output();
	
	if(++relay_pwm_cnt>=relay_pwm_period)relay_pwm_cnt=0;
	if(timer_1ms>=10)
	{
		timer_1ms=0;
		
		timer_10ms++;
		timer_50ms++;
		timer_100ms++;
		
		if(timer_3000ms_start==1)timer_3000ms++;
		freq_cnt++;
		
		led_output();
		hign_power_output();
		digs_output();
		if(freq_cnt>=100)
		{
			freq_cnt=0;
			TR0=0;
			freq=(unsigned int)(TH0<<8|TL0)*10;
			TH0=0;
			TL0=0;
			TR0=1;
		}
	}
}

//void Timer1(void) interrupt 3
//{

//	timer_1ms=0;
//	
//	timer_10ms++;
//	timer_50ms++;
//	timer_100ms++;
//	
//	if(timer_3000ms_start==1)timer_3000ms++;
//	freq_cnt++;
//	
//	led_output();
//	hign_power_output();
//	digs_output();
//	if(freq_cnt>=100)
//	{
//		freq_cnt=0;
//		TR0=0;
//		freq=(unsigned int)(TH0<<8|TL0)*10;
//		TH0=0;
//		TL0=0;
//		TR0=1;
//	}

//}


