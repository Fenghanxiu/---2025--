#include "echo.h"

sbit trig=P1^0;   //���巢�Ͷ�
sbit echo=P1^1;   //������ն�
//PCA��ʱ�汾
void Delay14us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 39;
	while (--i);
}

void send_wave()
{
	unsigned char i=0;
	EA=0;
	for(i=0;i<8;i++)
	{
		trig=1;Delay14us();
		trig=0;Delay14us();
	}
	EA=1;
}
unsigned int xdata distance=0;
void Get_distance()
{
	CMOD=0x00;
	CH=0;
	CL=0;
	send_wave();
	CR=1;
	while((CF==0)&&(echo==1));
	CR=0;
	if(!CF)
	{
		distance=(unsigned int)(CH<<8|CL)*0.017;//cm
	}
	else
	{
		CF=0;
		distance=0;
	}
}

//PCA�ж�

void PCAInit()
{
	CCON=0x00;
	CMOD=0x01;
	CCAPM0=0x11;
}

unsigned char xdata echo_flag=0;
//unsigned int xdata distance=0;

/* ultrasonic_flag ������״̬��־
	 0�����н׶�
	 1�����ͽ׶�
	 2��ȷ�Ͻ��յ����صĳ�����
	 3�����ղ������������򳬳����� */
void TimerPCAIsr() interrupt 7
{
	CR=0;
	
	if(echo_flag==1)//�����ڷ��ͽ׶�
	{
		if(CCF0==1)/* ���յ����صĳ����� */
		{
			echo_flag=2;//ȷ�Ͻ��յ����صĳ�����
		}
		else if(CF==1)//���ղ������������򳬳�����
		{
			echo_flag=3;//�������
		}
		else
		{
			echo_flag=0;
		}
	}
	
	CF=0;
	CCF0=0;
	CCF1=0;//��������־λ��Ȼû���õ�
	CCF2=0;//�����Է���һ�������жϿ�ס
}

/* ���������� */
void sand_ultrasonic()
{ 
	unsigned char i=0;
	//��ʱ��ǰ���Ƿ�����ǰ�����ԣ����ҵİ������ҷ��ּ�ʱ��ǰ�����ݱȽ�׼ȷ
	CH=0;
	CL=0;
	CF=0;
	
	EA=0;
	CR=1;
	for(i=0;i<8;i++)
	{
		trig=1;Delay14us();
		trig=0;Delay14us();
	}
	
	EA=1;
	
}

void calc_echo()
{
	if(echo_flag==2)
	{
		distance=(unsigned int)(CCAP0H<<8|CCAP0L)*0.017;//cm
	}
	else
	{
		distance=0;
	}
}

void read_echo()
{
	if(echo_flag>0)
	{
		calc_echo();
		echo_flag=0;
	}
	
	if(echo_flag==0)
	{
		sand_ultrasonic();
		echo_flag=1;
	}
}

///* ����㷢�ֲ������̺̣ܶ�����Ӧ�ÿ���
//	 ���ص�ʱ��IRCƵ����û�е���12.000MHz��
//	 �������������ҵĴ��� (>w<) */
//void Delay13us()		//@12.000MHz
//{
//	unsigned char i;

//	_nop_();
//	_nop_();
//	i = 39;
//	while (--i);
//}

//void TimerPCAInit()
//{
//	CCON = 0;	//��ʼ��PCA���ƼĴ���
//						//PCA��ʱ��ֹͣ
//						//���CF��־
//						//���ģ���жϱ�־
//	CMOD = 0x01;		//����PCAʱ��Դ 1MHz
//									//����PCA��ʱ������ж�
//	CCAPM0 = 0x11;	//PCAģ��0Ϊ�½��ش���
//									//�������岶����ģ��P11�½����ⲿ�ж�
//}

///* ultrasonic_flag ������״̬��־
//	 0�����н׶�
//	 1�����ͽ׶�
//	 2��ȷ�Ͻ��յ����صĳ�����
//	 3�����ղ������������򳬳����� */
//unsigned char ultrasonic_flag = 0;


///* P11����������RX��һ�������½��أ�
//	 ��˵���Ѿ��յ����صĳ�������������ж� */
//void TimerPCAIsr() interrupt 7
//{
//	CR = 0; //���Ƚ�����ʱ
//	
//	if (ultrasonic_flag == 1) //�����ڷ��ͽ׶�
//	{
//		if (CCF0) /* ���յ����صĳ����� */
//			ultrasonic_flag = 2; //ȷ�Ͻ��յ����صĳ�����
//		else if (CF) /* �������� */
//			ultrasonic_flag = 3; //���ղ������������򳬳�����
//		else
//			ultrasonic_flag = 0; //�������
//	}
////	else ���ﱾӦ�ü����else������bug������ʱ�Ҳ���
////		ultrasonic_flag = 0; //�������
//	
//	CF = 0;
//	CCF0 = 0;
//	CCF1 = 0; //��������־λ��Ȼû���õ�
//	CCF2 = 0; //�����Է���һ�������жϿ�ס
//}

///* ���������� */
//void sand_ultrasonic()
//{ //��ʱ��ǰ���Ƿ�����ǰ�����ԣ����ҵİ������ҷ��ּ�ʱ��ǰ�����ݱȽ�׼ȷ
//	/* ������ʱ */
//	CH = 0x8D;		//���ö�ʱ��ʼֵ�����ֵ����ô���ģ�0x8D1C �� 65535 - 500�����ף� / 0.017������/΢�룩��������̿����Լ���
//	CL = 0x1C;		//���ö�ʱ��ʼֵ
//	/* �������ԣ������ܲ���������4�ף�����������������������
//		 ����ʱ������������Զ��4�ף��������ó�ֵ������������������
//		 �����������5�ף����Դ������ٲ���Ҫ�Ĳ���ʱ�䣬
//		 ���Գ�����������ʱ�������ֵ����ֵ */
//	CF = 0;		//���CF��־
//	
//	/* �������������� */
//	EA = 0; //�ر��жϣ���ֹ��Ϸ���
//	CR = 1;	//��ʱ����ʼ��ʱ
//	P10 = 1; Delay13us(); P10 = 0; Delay13us(); //�ߵ�ƽ��ǰ�͵�ƽ�ں�
//	P10 = 1; Delay13us(); P10 = 0; Delay13us();
//	P10 = 1; Delay13us(); P10 = 0; Delay13us();
//	P10 = 1; Delay13us(); P10 = 0; Delay13us();
//	P10 = 1; Delay13us(); P10 = 0; Delay13us();
//	P10 = 1; Delay13us(); P10 = 0; Delay13us();
//	P10 = 1; Delay13us(); P10 = 0; Delay13us();
//	P10 = 1; Delay13us(); P10 = 0; //��ʱ������Ҫֱ��һ�㣨ֱָ�Ӹ���8�Σ�
//	EA = 1; //���´��ж�
//}

///* ������� */
//void calculate_distance()
//{
//	if (ultrasonic_flag == 2) //ȷ�Ͻ��յ����صĳ�����
//	{
////		distance = CCAP0H;
////		distance <<= 8;
////		distance |= CCAP0L;
////		distance -= 0x8D1C; //������ʱ����ֵ����ȥ��ֵԭ�������ó�ֵ�ĵط���ע��
////		distance = (float)distance * 0.017;
//		distance = ((CCAP0H << 8 | CCAP0L) - 0x8D1C) * 0.017;
//	}
//	else //���ղ������������򳬳�����
//	{
//		//distance = DISTANCE_OUTRANG;
//		distance = 0;
//	}
//}

///* ��ȡ���뺯��
//	 ��һ����Ҫ��ôд
//	 ���Ը��������������������޸� */
//void read_distance()
//{ //�����������������д�Ĳ��ã�����Ӧ������򵥵�д����
//	if (ultrasonic_flag > 1)
//	{
//		calculate_distance();
//		ultrasonic_flag = 0; //���뷢�ͽ׶�
//	}
//	
//	if (ultrasonic_flag == 0)
//	{
//		sand_ultrasonic();
//		ultrasonic_flag = 1; //���뷢�ͽ׶�
//	}
//}
