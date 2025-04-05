#include "filter.h"

void swap(unsigned int *x,unsigned int *y)
{
	unsigned int temp;
	temp=*x;
	*x=*y;
	*x=temp;
}

/* ֻ��һ���ܼ�ª����ֵ�˲����������ݼ�����λ����
	 ������ע�⣬���������ֻ����3�������ܶ�Ҳ�����٣�
	 ��Ȼ��ª�����������¶ȡ���������adc��Ƶ�ʵĶ�ȡ��
	 Ч�����Ǻ��ȶ��ģ����Դ����������
	 �������ż����ȡ��������� */
unsigned int median_filter(unsigned int *Data)
{
	unsigned int temp;
	
	temp = MAX(*(Data + 0), *(Data + 1));
	if (temp <= *(Data + 2))
		return temp;
	else
	{
		temp = MIN(*(Data + 0), *(Data + 1));
		return MAX(temp, *(Data + 2));
	}
}
/* ֻ��һ���򵥵ľ�ֵ�˲�������ƽ��ֵ */
unsigned int average_filter(unsigned int *Data, unsigned int len)
{
	unsigned long sum = 0;
	unsigned int i;
	
	for (i = 0; i < len; i++)
		sum += *(Data + i);
	
	return sum / len;
}


//plus�汾,�������������б䶯��ʵʱ����
unsigned int med_filter(unsigned int *Data,unsigned int new_data)
{
	static unsigned char  median_filter_count=0;
	
	unsigned char min_12=0;
	unsigned char max_12=0;
	
	Data[2]=Data[1];
	Data[1]=Data[0];
	Data[0]=new_data;
	
	if(median_filter_count<3)median_filter_count++;
	//
	min_12=MIN(Data[0],Data[1]);
	max_12=MAX(Data[0],Data[1]);
	
	if(max_12<Data[2])
	{
		return max_12;
	}
	else
	{
		return min_12>Data[2]?min_12:Data[2];
	}
		
}

unsigned int ave_filter(unsigned int *Data,unsigned int new_data)
{
	static unsigned char  ave_filter_count=0;
	unsigned long sum = 0;
	unsigned char i=0;
	
	Data[2]=Data[1];
	Data[1]=Data[0];
	Data[0]=new_data;
	
	if(ave_filter_count<3)ave_filter_count++;
	
	//
	for (i = 0; i < ave_filter_count; i++) {
        sum += Data[i];
    }

    return (unsigned int)(sum /ave_filter_count);
}

/* �򵥵�ð������ */
void sort(unsigned int *Data, unsigned int len)
{
	int i, j;
	
	for (i = 0; i < len; i++)
	{
		for (j = i + 1; j < len; j++)
		{
			if (*(Data + i) > *(Data + j))
				swap((Data + i), (Data + j));
		}
	}
}

unsigned char reverse_bits(unsigned char b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}
