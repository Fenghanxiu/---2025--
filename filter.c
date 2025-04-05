#include "filter.h"

void swap(unsigned int *x,unsigned int *y)
{
	unsigned int temp;
	temp=*x;
	*x=*y;
	*x=temp;
}

/* 只是一个很简陋的中值滤波，返回数据集的中位数，
	 不过请注意，传入的数据只能是3个，不能多也不能少，
	 虽然简陋，但是用在温度、超声波、adc、频率的读取，
	 效果都是很稳定的，可以从软件层面上
	 解决数据偶尔读取错误的问题 */
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
/* 只是一个简单的均值滤波，计算平均值 */
unsigned int average_filter(unsigned int *Data, unsigned int len)
{
	unsigned long sum = 0;
	unsigned int i;
	
	for (i = 0; i < len; i++)
		sum += *(Data + i);
	
	return sum / len;
}


//plus版本,根据数据量进行变动，实时更新
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

/* 简单的冒泡排序 */
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
