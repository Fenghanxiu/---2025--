# **蓝桥杯代码考点整理**

# 总体芯片资源分配

## 1、引脚分配

![img](E:\a蓝桥杯资料\个人\蓝桥杯代码考点整理\98c5c0231f6de65235142ec90be4bf66.png)

超声波





## 2、资源分配

[蓝桥杯芯片资源分配及注意事项_蓝桥杯单片机p34复用了,矩阵键盘还有效吗-CSDN博客](https://blog.csdn.net/qq_37429313/article/details/113870390#:~:text=本文讨论了微控制器中定时器的资源分配，建议将定时器0专用于NE555频率测量，定时器1用于扫描功能，如延时、数码管和按键扫描，定时器2用于PWM输出或串口功能，PCA定时器则服务于超声波测距。 同时，注意P34引脚的使用限制，当用于NE555频率检测时，无法同时使用矩阵键盘。,NE555制造的频率测量最好是使用定时器0的计数器模式来测量，因此 定时器0 最好是专门留给NE555定时器。)



- NE555制造的频率测量最好是使用定时器0的计数器模式来测量，因此定时器0最好是专门留给NE555定时器。
- 定时器1用作扫描功能，用于制造延时、数码管扫描以及按键扫描、制造pwm输出
- 定时器2用作串口功能
- PCA定时器用作超声波测距的相关功能
- 

# 138相关

## 核心准则：

==1、P0赋值==

==2、P2或选通138==

==3、P2与非清空138==

## led

```C
//led
unsigned char xdata led_buff=0;
void led_output()
{
	P0=~led_buff;
	P2|=0x80;
	P2&=0x1F;
}

```

## high_power_state累（继电器，蜂鸣器，电机）

P04为继电器，P05为电机，P06为蜂鸣器

```c
//high_power_state
unsigned char xdata high_power_buff=0;
void high_power_output()
{
	P0=high_power_buff;
	P2|=0xa0;
	P2&=0x1F;
}
```

## 电机PWM输出电压测量曲线

测量条件：周期1khz，led_pwm_period=10；led_pwm_duty范围0-10；万用表采用UT33B+ 直流电压采集2000mv档位，单位mv

| led_pwm_duty | 对应占空比% | 直流电压值mv | 交流电压值mv（这逼万用表只有A+才有这功能） |  备注  |
| :----------: | :---------: | :----------: | :----------------------------------------: | :----: |
|      0       |     00%     |      18      |                                            | 不稳定 |
|      1       |     10%     |     -64      |                                            | 不稳定 |
|      2       |     20%     |    10-20     |                                            | 不稳定 |
|      3       |     30%     |    86-95     |                                            |  稳定  |
|      4       |     40%     |   155-160    |                                            |  稳定  |
|      5       |     50%     |     228      |                                            |  稳定  |
|      6       |     60%     |     295      |                                            |  稳定  |
|      7       |     70%     |     374      |                                            |  稳定  |
|      8       |     80%     |     450      |                                            |  稳定  |
|      9       |     90%     |     537      |                                            |  稳定  |
|      10      |    100%     |     629      |                                            |  稳定  |



## 数码管类

```C
//digs
/*************  本地常量声明    **************/
unsigned char code t_display[]={                       //标准字库
//   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
//black  -     H    J    K    L    N    o   P    U     t    G    Q    r   M    y
    0x00,0x40,0x76,0x1E,0x70,0x38,0x37,0x5C,0x73,0x3E,0x78,0x3d,0x67,0x50,0x37,0x6e,
    0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0x46};    //0. 1. 2. 3. 4. 5. 6. 7. 8. 9. -1

unsigned char code T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};      //位码
unsigned char xdata dig_buff[][8]=
{
	{21,16,16,16,17,16,16,16,},//坐标
	{14,1,16,16,16,16,16,16,},//速度
	{24,16,16,16,16,16,16,16,}//参数
};
unsigned char xdata *digs=dig_buff[0];
unsigned char xdata dig_com = 0; //当前显示的数码管位
unsigned char xdata dig_switch=0xff;

void digs_output()
{
	P0=0x00;
	P2|=0xC0;
	P2&=0x1F;
	

	P0=~t_display[digs[dig_com]];
	P2|=0xE0;
	P2&=0x1F;
	
	P0=T_COM[dig_com]&dig_switch;
	P2|=0xC0;
	P2&=0x1F;
	
	if(++dig_com>=8)dig_com=0;

}
```

# AT24C02 EEPROM器件（IIC类）

[蓝桥杯单片机比赛学习：10、AT24C02的基本原理和使用方法_单片机的这个24c02c芯片 运作原理是什么-CSDN博客](https://blog.csdn.net/qq_58872950/article/details/128932013)

[AT24C02详解(蓝桥杯单片机模块(IIC总线))_at24c02写入后-CSDN博客](https://blog.csdn.net/qq_37429313/article/details/115317374)

## 1、基本原理

AT24C02实际上就是一个具有 I2C 总线接口的存储器，可以读写数据。AT24C02 存储器特点如下：容量为 256×8（ 2KB）；具体的i2c协议这里就不讲了，我们这里只讲AT24C02在比赛中的使用方法，这样能让我们快速的上手参加比赛。

AT24C02实际上和PCF8591差不多，都是8位寄存器，如下图1，高4位是生产厂家规定器件地址为1010（不可编程更改）；低4位为可编程更改，其中A3、A1、A0默认接地（GND）如下图2，所以此寄存器的前7位都是不需要更改的，需要编程更改地址只有最后一位方向位 R/W，当主控器对 存储器进行读操作时为 1，进行写操作时为 0。也就是我们通常写的IIC_SendByte(0xA0)、IIC_SendByte(0xA1);。0xA0表示写操作；0xA1表示读操作。


## 2、代码实现（均为连读模式，读一个字节改成1即可）

```C
/* ------------------at24c02----------------- */
void e2prom_write(uint8_t addr, uint8_t *Data, uint8_t len)
{
	I2CStart();
	I2CSendByte(0xA0);
	I2CWaitAck();
	I2CSendByte(addr);
	I2CWaitAck();
	while(len--)
	{
		I2CSendByte(*Data);
		I2CWaitAck();
		Data++;
	}
	I2CStop();
}

void e2prom_read(uint8_t addr, uint8_t *Data, uint8_t len)
{
	I2CStart();
	I2CSendByte(0xA0);
	I2CWaitAck();
	I2CSendByte(addr);
	I2CWaitAck();
	

	I2CStart();
	I2CSendByte(0xA1);
	I2CWaitAck();
	
	loop:
	*Data = I2CReceiveByte();
	if (--len)
	{
		I2CSendAck(0); //发送应答
		Data++;
		goto loop;
	}
	I2CSendAck(1); //发送非应答
	I2CStop();

}
```

## 3、（重要）常见问题

### （1）连续写入/读取偶尔错误（本应写入0123456789abcdef, 实际写入0520456084ace）

[蓝桥杯嵌入式--E2PROM读写异常解决_e2prom写不进数据-CSDN博客](https://blog.csdn.net/weixin_54358182/article/details/129659609)

[关于AT24C02的写入读取巨坑 - 24小时必答区](http://www.51hei.com/bbs/dpj-151160-1.html)

原因：读取间隔太短

==但凡用IIC，对于速度就不会是很高的要求，加个延时就好了==

at24c02 写周期最长是5ms，数据手册上有， 你这个延时1ms也是有风险的；at24c02具有8字节写模式，连续写8字节后，需要延时5ms的；字节写模式也需要等待5ms是比较安全的；如果不等待5ms，再写下一字节是需要判段应答位的，应答位不对，等待再重新发写命令才可以的。

![img](E:\a蓝桥杯资料\个人\蓝桥杯代码考点整理\22c5ad8b0cbd1391b86ba5f8535921ed.png)

#### ==解决方法：相邻两次的读写操作的间隔应该在5ms以上，以保证数据正确==

### （2）掉电存储问题

[蓝桥杯I2C使用EEPROM教程（内附初始值设定）_蓝桥杯单片机eeprom的程序-CSDN博客](https://blog.csdn.net/qq_41649861/article/details/105407657)

这里我们模拟一个简单的存取场景，我们任找一个地址如：0x11作为存储数据的地址，然后从0开始，每次我们按下开发板的复位键后都自加1，来实现I2C模拟EEPROM掉电保存数据的情形。
出现了一个问题：往往在存储数据的地址中一开始就有一个值，比如在烧录程序之前，可能0x11地址中的值为12，那么我们开始就只能从12开始加1，这样不符合我们的要求；而如果我们在程序的开头加上_24c02_Write(0x11,0);,那么这样相信大家也知道，每次启动程序，值都会变为0，这就查看不了我们的EEPROM的变化。所以我们引进另一个地址来进行解决

```C
// 掉电统计（掉电计数）
e2prom_read(0xa0, &diaodian_flag, 1); // 从 EEPROM 地址 0xA0 读取掉电标志位（1字节）到 diaodian_flag 变量中
Delay5ms(); // 延迟 5 毫秒（用于等待 EEPROM 操作完成）

if(diaodian_flag == 0xbb) // 如果标志位是 0xBB，说明之前已经初始化过
{
    e2prom_read(0xa1, &diaodian_count, 1); // 从 EEPROM 地址 0xA1 读取掉电次数（1字节）到 diaodian_count
    Delay5ms();
    
    diaodian_count++; // 掉电次数加 1
    
    e2prom_write(0xa1, &diaodian_count, 1); // 将新的掉电次数写回 EEPROM 地址 0xA1
    Delay5ms();
}
else // 如果标志位不是 0xBB，说明是第一次统计（EEPROM尚未初始化）
{
    diaodian_count = 0; // 初始化掉电次数为 0
    diaodian_flag = 0xbb; // 设置标志位为 0xBB，表示已经初始化

    e2prom_write(0xa0, &diaodian_flag, 1); // 将标志位写入 EEPROM 地址 0xA0
    Delay5ms();

    e2prom_write(0xa1, &diaodian_count, 1); // 将掉电次数写入 EEPROM 地址 0xA1
    Delay5ms();
}

```

这样我们的0x11就可以从0开始自加了。这个可以解决蓝桥杯比赛中的一个非常重要的问题：
我们在比赛的时候可能在现场的板子上已经设定的特别好，但是当评委组拿到你的.axf文件烧录时，他们不管你怎么设定，可能只会看第一次烧录后的结果，而如果那时你从I2C中读出的初始值是不确定的一个数，那么无论你设计的多好，这一块也会酌情扣分的。

# PCF8591 ADC/DAC（IIC类)

[PCF8591详解(蓝桥杯单片机模块(IIC总线))-CSDN博客](https://blog.csdn.net/qq_37429313/article/details/115285842)

[蓝桥杯单片机—— PCF8591的基本原理及A/D转换应用（14）-CSDN博客](https://blog.csdn.net/m0_57263863/article/details/123487055)

## 1、基本概念

PCF8591是一个单片集成、单独供电、低功耗、8-bit CMOS数据获取器件。PCF8591具有4个模拟输入、1个模拟输出和1个串行I²C总线接口。PCF8591的3个地址引脚A0, A1和A2可用于硬件地址编程，允许在同个I2C总线上接入8个PCF8591器件，而无需额外的硬件。在PCF8591器件上输入输出的地址、控制和数据信号都是通过双线双向I2C总线以串行的方式进行传输。 

## 2、通道引脚

AIN0～AIN3：模拟信号输入端。A0～A2：引脚地址端。

==AIN0：接到右边排针上，可以用杜邦线连外部模拟电压信号==
==AIN1：接到光敏电阻上==
==AIN2：放大器的输出端==
==AIN3：接到滑动变阻器Rb2上==

省赛中考得最多的是通道1和通道3：

 光敏传感器接到AIN1，通道1；控制寄存器应写入：0x01。
 电位器Rb2接到AIN3，通道3；控制寄存器应写入：0x03。

## 3、器件地址字节  

PCF8591 的通信接口是 I2C，那么编程肯定是要符合这个协议的。单片机对 PCF8591 进行初始化，一共发送三个字节即可。第一个字节， 和 EEPROM 类似，是器件地址字节，其中 7 位代表地址， 1 位代表读写方向。==地址高 4 位固定是 0b1001==，低三位是 A2， A1， A0，如图 17-5 所示。  



![image-20250321210946289](E:\a蓝桥杯资料\个人\蓝桥杯代码考点整理\image-20250321210946289.png)

## 4、控制寄存器  

发送到 PCF8591 的第二个字节将被存储在控制寄存器，用于控制 PCF8591 的功能。其中第 3 位和第 7 位是固定的 0，另外 6 位各自有各自的作用，如图 17-6 所示，我逐一介绍。  



![image-20250321211033939](E:\a蓝桥杯资料\个人\蓝桥杯代码考点整理\image-20250321211033939.png)

控制字节的==第 6 位是 DA 使能位==，这一位置 1 表示 DA 输出引脚使能，会产生模拟电压输出功能。

==第 4 位和第 5 位可以实现把 PCF8591 的 4 路模拟输入配置成单端模式和差分模式==，单端模式和差分模式的区别，我们在 17.5 节有介绍，这里大家只需要知道这两位是配置 AD输入方式的控制位即可，如图 17-7 所示。  

![image-20250321211155933](E:\a蓝桥杯资料\个人\蓝桥杯代码考点整理\image-20250321211155933.png)

控制字节的==第 2 位是自动增量控制位==，自动增量的意思就是，比如我们一共有 4 个通道，当我们全部使用的时候，读完了通道 0，下一次再读，会自动进入通道 1 进行读取，不需要我们指定下一个通道，由于 A/D 每次读到的数据，都是上一次的转换结果，所以同学们在使用自动增量功能的时候，要特别注意，当前读到的是上一个通道的值。为了保持程序的通用性，我们的代码没有使用这个功能，直接做了一个通用的程序。

控制字节的==第 0 位和第 1 位就是通道选择位了==， 00、 01、 10、 11 代表了从 0 到 3 的一共4 个通道选择。发送给 PCF8591 的第三个字节 D/A 数据寄存器，表示 D/A 模拟输出的电压值。

 D/A 模拟我们一会介绍，大家知道这个字节的作用即可。

我们如果仅仅使用 A/D 功能的话，就可以不发送第三个字节。

## 5、代码

```C
/* ------------------pcf8591----------------- */
uint8_t control_byte = 0x04;
uint8_t dac_level = 0; //当前dac输出的等级，输出前先修改这个值再调用输出函数
uint8_t adc_level[4] = { 0 };

void dac_out(void)
{
	control_byte |= 0x40; //开启dac输出
	

	I2CStart();
	I2CSendByte(0x90);
	I2CWaitAck();
	I2CSendByte(control_byte);
	I2CWaitAck();
	I2CSendByte(dac_level);
	I2CWaitAck();
	I2CStop();

}

void dac_stop(void)
{
	control_byte &= ~0x40; //关闭dac输出
	

	I2CStart();
	I2CSendByte(0x90);
	I2CWaitAck();
	I2CSendByte(control_byte);
	I2CWaitAck();
	I2CStop();

}

void adc_read(void)//连读模式
{
	I2CStart();
	I2CSendByte(0x90);
	I2CWaitAck();
	I2CSendByte(control_byte);
	I2CWaitAck();
	

	I2CStart();
	I2CSendByte(0x91);
	I2CWaitAck();
	
	I2CReceiveByte(); //第一次读取的数据不要使用
	I2CSendAck(0); //发送应答
	adc_level[0] = I2CReceiveByte();
	I2CSendAck(0); //发送应答
	adc_level[1] = I2CReceiveByte();
	I2CSendAck(0); //发送应答
	adc_level[2] = I2CReceiveByte();
	I2CSendAck(0); //发送应答
	adc_level[3] = I2CReceiveByte();
	I2CSendAck(1); //发送非应答
	I2CStop();

}

unsigned char read_pcf8591(unsigned char channle)
{
	unsigned char buf;
	

	I2CStart();
	I2CSendByte(0x90);
	I2CWaitAck();
	I2CSendByte(channle | 0x40);
	I2CWaitAck();
	
	I2CStart();
	I2CSendByte(0x91);
	I2CWaitAck();
	I2CReceiveByte();
	I2CSendByte(0); //应答
	buf = I2CReceiveByte();
	I2CSendByte(1); //非应答
	I2CStop();
	
	return buf;

}

void write_pcf8591(unsigned char level)
{
	I2CStart();
	I2CSendByte(0x90);
	I2CWaitAck();
	I2CSendByte(0x40);
	I2CWaitAck();
	I2CSendByte(level);
	I2CWaitAck();
	I2CStop();
}

void stop_pcf8591(void)
{
	

	I2CStart();
	I2CSendByte(0x90);
	I2CWaitAck();
	I2CSendByte(0x00);
	I2CWaitAck();
	I2CStop();

}

```

## 6、（重要）常见问题

### （1）读取出来数据不稳定，偶尔会跳变

#### ==第一次读取的数据不要使用，先空读一次==

原因：第一次转换的数据可能不稳定

# DS1302时钟（SPI类）

[蓝桥杯单片机比赛学习：12、DS1302时钟基本原理与使用方法_ds1302怎么用-CSDN博客](https://blog.csdn.net/qq_58872950/article/details/129080392)

[蓝桥杯之单片机学习（十七）——DS1302的基本应用_蓝桥杯单片机1302-CSDN博客](https://blog.csdn.net/m0_52592798/article/details/123588835)

## 一、DS1302概述

DALLAS公司推出的高性能、低功耗的实时时钟。
采用SPI三线接口与MCU进行同步通信，并可采用突发方式一次传送多个字节的时钟参数和RAM数据。
附加31字节的静态RAM
简单来说，DS1302可以理解为-一个电子手表，里面带有一个31字节的RAM内存。
在DS1302中有两块存储器:日历时钟寄存器和静态RAM存储器。前者用于记录实时时间，后者用于记录其他数据。
关注重点：
日历时钟寄存器

## 二、日历时钟寄存器（重点）

![在这里插入图片描述](E:\a蓝桥杯资料\个人\蓝桥杯代码考点整理\c89ee62ee3ce9da7d720efdf56a41f78.png)

1、秒寄存器(80H和81H)的位**7 (CH)为时钟暂停标志。**
==CH为0时，时钟振荡停止;==
==CH为1时，时钟开始运行。==
2、控制寄存器(8EH 和8FH)的位**7 (WP)为写保护位。**
==WP为0时，可对任何的时钟或RAM寄存器进行写操作;==
==WP为1时，禁止对任一寄存器进行写操作。==

其他：

寄存器 0：最高位 CH 是一个时钟停止标志位。如果时钟电路有备用电源，上电后，我们要先检测一下这一位，如果这一位是 0，那说明时钟芯片在系统掉电后，由于备用电源的供给，时钟是持续正常运行的；如果这一位是 1，那么说明时钟芯片在系统掉电后，时钟部分不工作了。如果 Vcc1 悬空或者是电池没电了，当我们下次重新上电时，读取这一位，那这一位就是 1，我们可以通过这一位判断时钟在单片机系统掉电后是否还正常运行。剩下的7 位高 3 位是秒的十位，低 4 位是秒的个位，这里再提请注意一次， DS1302 内部是 BCD 码，而秒的十位最大是 5，所以 3 个二进制位就够了。

寄存器 1：最高位未使用，剩下的 7 位中高 3 位是分钟的十位，低 4 位是分钟的个位。

寄存器 2： ==bit7 是 1 的话代表是 12 小时制， 0 代表是 24 小时制； bit6 固定是 0， bit5 在12 小时制下 0 代表的是上午， 1 代表的是下午，在 24 小时制下和 bit4 一起代表了小时的十位，低 4 位代表的是小时的个位。==

寄存器 3：高 2 位固定是 0， bit5 和 bit4 是日期的十位，低 4 位是日期的个位。

寄存器 4：高 3 位固定是 0， bit4 是月的十位，低 4 位是月的个位。

寄存器 5：高 5 位固定是 0，低 3 位代表了星期。

寄存器 6：高 4 位代表了年的十位，低 4 位代表了年的个位。请特别注意，这里的 00～ 99 指的是 2000 年～2099 年。寄存器 7：最高位一个写保护位，如果这一位是 1，那么是禁止给任何其它寄存器或者那 31 个字节的 RAM 写数据的。因此在写数据之前，这一位必须先写成 0。

## 三、控制字格式与数据定义

![在这里插入图片描述](E:\a蓝桥杯资料\个人\蓝桥杯代码考点整理\c1dc65bb6f190acd20455b42a55a65dd.png)

BIT7: 必须为1,若为0，则不能把数据写入DS1302中。

BIT6: 0表示存取日历时钟数据，1表示存取RAM数据。

BIT5~BIT1:表示操作单元的地址。

BIT0: 0表示写操作，1表示读操作。

## 四、操作流程

1、当我们在设置时钟时，我们需要知道时分秒年月日星期对应的寄存器地址，也就是WRITE:0x80、0x82、0x84、0x86、0x88、0x8a、0x8c这几个地址，这里不需要牢记，在考试中有数据手册（只要会查会使用即可）

2、当我们读取时钟数据时，我们也需要知道时分秒年月日星期对应的寄存器地址，也就是READ:0x81、0x83、0x85、0x87、0x89、0x8b、0x8d这几个地址。
==注：当我们在设置时钟时（也就是写操作时），设置时间之前需要对0x8e地址写入0x00，也就是解除写保护（将WP标志位置0）；设置时间之后需要对0x8e地址写入0x80，也就是打开写保护（将WP标志位置1）==

## 五、BCD转换

由于从DS1302寄存器中读取的数据不是具体的数（是BCD码），所以我们不能直接读取时间显示在数码管中，这里我们需要转换一下。

```C
void time_10_to_16(void)
{
	uint8_t num;
	for (num = 0; num < DATA_NUM; num++) 
		time_16[num] = time_10[num] / 10 * 0x10 + time_10[num] % 10;
}

void time_16_to_10(void)
{
	uint8_t num;
	for (num = 0; num < DATA_NUM; num++) 
		time_10[num] = time_16[num] / 0x10 * 10 + time_16[num] % 0x10;
}
```

## 六、总代码

```C
/* ------------------------------------- */
#define DATA_NUM 3 //数据个数

uint8_t time_10[DATA_NUM] = { 23, 59, 55, };
static uint8_t time_16[DATA_NUM] = { 0x23, 0x59, 0x55, };

void time_10_to_16(void)
{
	uint8_t num;
	for (num = 0; num < DATA_NUM; num++) 
		time_16[num] = time_10[num] / 10 * 0x10 + time_10[num] % 10;
}

void time_16_to_10(void)
{
	uint8_t num;
	for (num = 0; num < DATA_NUM; num++) 
		time_10[num] = time_16[num] / 0x10 * 10 + time_16[num] % 0x10;
}

void write_ds1302(void)
{
	time_10_to_16();
	Write_Ds1302_Byte(0x8e,0x00);/* 打开写操作 */


	Write_Ds1302_Byte(0x84, time_16[0]); // 时
	Write_Ds1302_Byte(0x82, time_16[1]); // 分
	Write_Ds1302_Byte(0x80, time_16[2]); // 秒
    
    Write_Ds1302_Byte(0x8e,0x80);/* 关闭写操作 */


}

void read_ds1302(void)
{
	time_16[0] = Read_Ds1302_Byte(0x85); // 时
	time_16[1] = Read_Ds1302_Byte(0x83); // 分
	time_16[2] = Read_Ds1302_Byte(0x81); // 秒
	

	time_16_to_10();

}
```

## 七、常见问题

### 1、DS1302使用数字奇怪，且不会变化

原因：正常来说DS1302是不会断电的，但是蓝桥杯开发板会断电，导致芯片工作异常

#### ==解决方法：初始化时打开写保护进行初始化写入即可==



#  DS18B20温度传感器 （ONEWIRE类）

[蓝桥杯单片机开发及应用-基础模块14-温度采集模块DS18B20_蓝桥杯板子用ds18b20-CSDN博客](https://blog.csdn.net/m0_73664266/article/details/134203872?ops_request_misc=&request_id=&biz_id=102&utm_term=蓝桥杯ds18b20&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-0-134203872.142^v102^pc_search_result_base9&spm=1018.2226.3001.4187)



## 1、介绍

DS18B20 是美信公司的一款温度传感器，单片机可以通过 1-Wire 协议与 DS18B20 进行通信，最终将温度读出。 1-Wire 总线的硬件接口很简单，只需要把 DS18B20 的数据引脚和单片机的一个 IO 口接上就可以了。硬件的简单，随之而来的，就是软件时序的复杂。  

DS18B20是测温范围在-55℃~+125℃的可编程分辨率的单总线数字温度计。（可编程分辨率，也就是说在软件编程时可以实现读取温度保留整数、一位小数、两位小数抑或是三位四位小数。单总线，也就是说DS18B20是连在单根总线上的，需要使用到提供的onewire库。如何操作将在后文中说到）


##  2.DS18B20的数据处理与指令含义

### 1、数据读取

DS18B20是以16位带符号位扩展的二进制补码形式读出，==高五位为符号位（全部为0就是整数，全部为1就是负数），中间7位为整数部分，低四位为小数部分==。以+85℃为例按照高中低位断开：0x0550 = 00000 （+）1010101（85） 0000（.0）。如下图所示，

### 2、 ROM 操作指令。

我们学 I2C 总线的时候就了解到，总线上可以挂多个器件，通过不同的器件地址来访问不同的器件。同样， 1-Wire 总线也可以挂多个器件，但是它只有一条线，如何区分不同的器件呢？在每个 DS18B20 内部都有一个唯一的 64 位长的序列号，这个序列号值就存在 DS18B20内部的 ROM 中。开始的 8 位是产品类型编码（DS18B20 是 0x10），接着的 48 位是每个器件唯一的序号，最后的 8 位是 CRC 校验码。 DS18B20 可以引出去很长的线，最长可以到几十米，测不同位置的温度。单片机可以通过和 DS18B20 之间的通信，获取每个传感器所采集到的温度信息，也可以同时给所有的 DS18B20 发送一些指令。这些指令相对来说比较复杂，而且应用很少，所以这里大家有兴趣的话就自己去查手册完成吧，我们这里只讲一条总线上只接一个器件的指令和程序。

### Skip ROM（跳过 ROM）： ==0xCC==。

当总线上只有一个器件的时候，可以跳过 ROM，不进行 ROM 检测。

## 3、 RAM 存储器操作指令。

RAM 读取指令，只讲 2 条，其它的大家有需要可以随时去查资料。

### Read Scratchpad（读暂存寄存器）： ==0xBE==

这里要注意的是， DS18B20 的温度数据是 2 个字节，我们读取数据的时候，先读取到的是低字节的低位，读完了第一个字节后，再读高字节的低位，直到两个字节全部读取完毕。

### Convert Temperature（启动温度转换）： ==0x44==

当我们发送一个启动温度转换的指令后， DS18B20 开始进行转换。从转换开始到获取温度， DS18B20 是需要时间的，而这个时间长短取决于 DS18B20 的精度。前边说 DS18B20 最高可以用 12 位来存储温度，但是也可以用 11 位， 10 位和 9 位一共四种格式。位数越高，精度越高， 9 位模式最低位变化 1 个数字温度变化 0.5 度，同时转换速度也要快一些，如图 16-16所示。

![image-20250321193404661](E:\a蓝桥杯资料\个人\蓝桥杯代码考点整理\image-20250321193404661.png)

##  3.DS18B20的分辨率

​                DS18B20的分辨率为==0.0625==，那么为什么是0.0625呢？ 这就对应了上面说的最多到小数点后四位，在二进制转为十进制中就是2^(-4)=1/16=0.0625。

                当读出的数据为正温度时，将LSB和MSB（低八位和高八位）整合成16位数，直接乘以0.0625即可。（乘以0.0625=1/16就相当于把二进制的数右移四位去掉低四位表示的小数部分）
    
                当读出的数据为负温度时，需要将LSB和MSB整合成的16位整数，取反后再加1，再乘以0.0625。（所涉及到的就是数电的二进制正负问题的知识）

##  4.注意事项

​            ①使用DS18B20需要用到onewire的库，比赛时会提供onewire.c和onewire.h。

​            ②非常重要的一点！！！==就是组委会可能会在提供的文件中动一些手脚，以增加难度，这时读取的温度要么刷新很慢要么读出来的数就是错的。那么我们在使用之前就要认真分辨，哪里动了手脚，我们再改回来==（onewire、iic、ds1302文件都已上传资源，需要的可自行下载）；

​            ==③比赛时会提供onewire的库，但是有时候提供的是12T的，而蓝桥杯比赛的板子是1T的。可以把onewire文件中的延时都扩大10倍或12倍。至于这个倍数看情况，自己可以多试几个倍数，哪个倍数读取温度时更好用就用哪个，没有绝对情况。==

##  5.整数、一位小数、两位小数、三位小数、四位小数的实现

 			 ①整数
 	            直接乘以分辨率0.0625把小数部分去掉。

```
temp = 0.0625;
```

​                ②一位小数
​                相当于是乘以0.0625之后再乘以10，把整数扩大10倍（乘的数越大，右移的越少，这里就是相当于给小数留一个位置），然后对于（ LSB  & 0x0f ）0.625，相当于加上低八位中的低四位的第一位，那么就相当于是保留了一位小数。

```
temp *= 0.625;
temp += ( LSB & 0x0f ) * 0.625;
```

​                ③两位小数
​                相当于是乘以0.0625之后再乘以100，把整数扩大100倍，然后对于（ LSB  & 0x0f ）*6.25，相当于加上低八位中的低四位的前两位，那么就相当于是保留了两位小数。

```
temp *= 6.25;
temp += ( LSB & 0x0f ) * 6.25;
```

​                ④三位小数
​                相当于是乘以0.0625之后再乘以1000，把整数扩大1000倍，然后对于（ LSB  & 0x0f ）*62.5，相当于加上低八位中的低四位的前三位，那么就相当于是保留了三位小数。

```
temp *= 62.5;
temp += ( LSB & 0x0f ) * 62.5;
```

​                ⑤四位小数
​                 相当于是乘以0.0625之后再乘以10000，把整数扩大10000倍，然后对于（ LSB  & 0x0f ）*625，相当于加上低八位中的低四位，那么就相当于是保留了四位小数。

```
temp *= 625;
temp += ( LSB & 0x0f ) * 625
```

##  6.DS18B20的温度转换与读取流程（在使用时直接套用该框架）

​            Ⅰ.DS18B20复位；

​            Ⅱ.写入字节0xcc，跳过ROM指令；（一般的单总线上可以接很多设备，每个DS18B20都有一个独特的64位序列号以示区分，而蓝桥杯单片机上只有一个DS18B20，因此可以直接跳过ROM的匹配ID）

​            Ⅲ.写入字节0x44，开始温度转换；

​            Ⅳ.延时一段时间；

​            Ⅴ.DS18B20再次复位；

​            Ⅵ.写入字节0xcc，跳过ROM指令；

​            Ⅶ.写入字节0xbe，读取高速暂存器；

​            Ⅷ.读取暂存器的第0字节，即温度数据的LSB（低八位）；

​            Ⅸ.读取暂存器的第1字节，即温度数据的MSB（高八位）；

​            Ⅹ.将LSB和MSB整合成为一个16位的二进制数；

​            Ⅺ.判断读取数据的正负号，进行正负温度处理。

## 7.读取函数

```c
void read_temp(void)//温度要延迟700ms左右，不然读出来都是默认值
{
	uint16_t temp_H, temp_L;
	

	init_ds18b20();
	Write_DS18B20(0xCC);//跳过ROM指令
	Write_DS18B20(0x44);//开始温度转换
	
	init_ds18b20();
	Write_DS18B20(0xCC);//跳过ROM指令
	Write_DS18B20(0xBE);//读取高速暂存器；
	
	temp_L = Read_DS18B20();
	temp_H = Read_DS18B20();
	
	temperature = (float)(temp_H << 8 | temp_L) * 6.25; //温度放大100倍存储，一般来说为0.0625

}


```

## 8、（重点）坑点

参考：[蓝桥杯单片机DS18b20单总线测温模块常见问题解决_ds18b20读出85-CSDN博客](https://blog.csdn.net/qq_64257614/article/details/130828674)

芯片手册

### 本质原因

onewire为单总线，无时钟线，因此不像spi，iic等时序更据时钟线采样数据发出，而是根据低电平持续时间来判断1还是0，因此对于时间的延迟极其敏感，容易受到中断处理时间的影响。

处理核心：

==1、准确的延迟==

==2、减少中断的影响==

### （1）DS18b20开机显示85.00之类的

原因（取自参考手册）：

前边说 DS18B20 最高可以用 12 位来存储温度，但是也可以用 11 位， 10 位和 9 位一共四种格式（通过R0，R1寄存器决定）。位数越高，精度越高， 9 位模式最低位变化 1 个数字温度变化 0.5 度，同时转换速度也要快一些。  其中寄存器 R1 和 R0 决定了转换的位数，==出厂默认值就 11，也就是 12 位表示温度==，==最大的转换时间是 750ms==。==当启动转换后，至少要再等 750ms 之后才能读取温度==，否则读到的温度有可能是错误的值。这就是为什么很多同学读 DS18B20 的时候，第一次读出来的是 85度，这个值要么是==没有启动转换==，要么是==启动转换了，但还没有等待一次转换彻底完成，读到的是一个错误的数据。==

#### ==解决方法：温度要延迟700ms左右，不然读出来都是默认值（85度）==

```c
在程序初始化的部分加上

  Read_Temperature();//此时读取的温度值为默认的85

  Delay_1ms(750);//等待上一个read函数温度转换完成

  Read_Temperature();//此时再读取的温度值就变成了第一个read函数转换完成的温度值

先读，再延时较长时间，在进行后续要求的读取  
```

## （2）DS18b20温度显示会跳动、跳变

现象：例如需要显示的温度为：26.1，而数码管显示在26.1和3.5或者其他的一些不该出现的值）之间来回跳变。

原因： 没有考虑到单片机进入中断对onewire时序的影响。造成时序紊乱，读出来的温度值自然是乱的。

#### ==解决方法：在官方给出的驱动必要处加上EA = 0；==

```c
//
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	

	for(i=0;i<8;i++)
	{
		EA=0;//关
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(5);
		DQ = 1;
		dat >>= 1;
		EA=1;//开
	}
	Delay_OneWire(5);

}

//
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;


	for(i=0;i<8;i++)
	{
		EA=0;//关
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		Delay_OneWire(1);
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(5);
		EA=1;开
	}
	
	return dat;

}
```



## （3）DS18b20一直显示35.93度，且温度不刷新（官方已解决，新资源包不存在此问题）

原因：

最基础的原因:

用标志刷新读取手法时 标志位 数据类型容量溢出导致计数不到那个值，比如要计数到500的，但定义的count是char类型的，这根本不可能计数到500因为到255就溢出了

时序原因：

DS18B20最高具有12为精度（默认模式），分辨率达到0.0625摄氏度，且外围电路极其简单，甚至可以不接VCC，就靠数据流吊着就能工作。当然它工作时时序可没有那么简单，稍有不慎（延时错误），整个工作时序就混乱了，因此准确把握每次延时至关重要。

#### **==解决方法:检查时序==**

**<u>注意：老版本的赛点资源包的onewire驱动存在此问题，原因是老版本的官方给的驱动是针对STC89C52的（12T单片机），而我们的单片机是IAP15系列（1T单片机），快了12倍，所以要修改。==现在已经修复问题==</u>**

## （4）DS18b20超级快速跳动（部分板子存在这个问题）

原因：快速跳动说明onewire已经成功转换数据，但是读取Read_DS18B20（）时时序出现问题导致数据异常

#### ==解决方法：读取函数加一个延迟==

```C
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;

	EA=0;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		Delay_OneWire(1);//这里，加一单位即可
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(5);
	}
	EA=1;
	return dat;

}
```



## （5）需要修改的地方（如果读取不出来的话，一共三处）

读出85：

==延迟750ms；==

读取温度不稳定，加入三种解决方法，解决99%问题（从上向下依次尝试）

==（1）三个基本操作加入中断开关==

==（2）读操作加了1单位延迟==

==（3）读温度函数中间加了部分延迟==

采用滤波函数（后续介绍）



修改后的代码：

```c
/*	# 	单总线代码片段说明
	1. 	本文件夹中提供的驱动代码供参赛选手完成程序设计参考。
	2. 	参赛选手可以自行编写相关代码或以该代码为基础，根据所选单片机类型、运行速度和试题
		中对单片机时钟频率的要求，进行代码调试和修改。
*/

#include "onewire.h"

sbit DQ = P1^4;

//
void Delay_OneWire(unsigned int t)  
{
	unsigned char i;
	while(t--){
		for(i=0;i<6;i++);
	}
}

//
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		EA = 0; //------------关中断
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(5);
		DQ = 1;
		dat >>= 1;
		EA = 1; //------------开中断
	}
	Delay_OneWire(5);
}

//
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;

	for(i=0;i<8;i++)
	{
		EA = 0; //------------关中断
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		Delay_OneWire(1); //有些板子不用加这句
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(5);
		EA = 1; //------------开中断
	}
	return dat;

}

//
bit init_ds18b20(void)
{
  	bit initflag = 0;
  	

  	DQ = 1;
  	Delay_OneWire(12);
  	DQ = 0;
  	Delay_OneWire(80);
  	DQ = 1;
  		EA = 0; //------------关中断
  	Delay_OneWire(10); 
  	initflag = DQ;    
  		EA = 1; //------------开中断
  	Delay_OneWire(5);
  	  
  	return initflag;

}

/* -------------------------------------------- */
uint16_t temperature = 0;
void read_temp(void)
{
	uint16_t temp_H, temp_L;
	

	init_ds18b20();
	Write_DS18B20(0xCC);
	Write_DS18B20(0x44);
	
	init_ds18b20();
	Write_DS18B20(0xCC);
	Write_DS18B20(0xBE);
	
	temp_L = Read_DS18B20();
	temp_H = Read_DS18B20();
	
	temperature = (float)(temp_H << 8 | temp_L) * 6.25; //温度放大100倍存储

}
```

## （6）温度数据一开始能读出来，但是后面变成0（非底层驱动问题）

原因：读取速度过快，dis18b20还没转换完上一次的数据又发送指令导致传感器死机

#### ==解决方法：增加读取间隔，常见读取时间为200ms以上==

# 超声波（PCA中断方法）

参考：[蓝桥杯学习笔记 单片机CT107D 利用PCA模块实现超声波测距_蓝桥杯单片机pca-CSDN博客](https://blog.csdn.net/Hans_Rudle/article/details/136279847?ops_request_misc=%7B%22request%5Fid%22%3A%2240d15f9e5e4020cc284711f0bd799f33%22%2C%22scm%22%3A%2220140713.130102334.pc%5Fall.%22%7D&request_id=40d15f9e5e4020cc284711f0bd799f33&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~rank_v31_ecpm-4-136279847-null-null.142^v102^pc_search_result_base9&utm_term=超声波（PCA）&spm=1018.2226.3001.4187)

## 引入

![img](E:\a蓝桥杯资料\个人\蓝桥杯代码考点整理\9f5d1765d27e4bee986dbcf131f18dd4.png)

因为[51单片机](https://so.csdn.net/so/search?q=51单片机&spm=1001.2101.3001.7020)中的定时器数量较少，多个外设同时使用时可能会出现定时器不够用的情况

eg：

第十五届国赛题：同时使用了串口，ne555测频率超声波，同时我们需要一个定时器作为系统常规定时器

ne555==(定时器0==)

系统系统常规定时器（==定时器1==）

串口波特率发生器（==定时器2==）

超声波（==？==）

此时可以使用==单片机中的PCA模块==作为定时器来使用。

## 1.PCA介绍



![image-20250318133202717](E:\a蓝桥杯资料\个人\蓝桥杯代码考点整理\image-20250318133202717.png)

 [STC15](https://so.csdn.net/so/search?q=STC15&spm=1001.2101.3001.7020)单片机中有3路可编程计数器阵列，==PCA（Programmable Counter Array）==是其中之一。它具有多个定时器/计数器和捕获/比较单元。[PCA](https://so.csdn.net/so/search?q=PCA&spm=1001.2101.3001.7020)模块可以用于实现定时器、计数器、PWM（脉宽调制）和捕获等功能。

## 2.PCA寄存器介绍(部分)

### （1）CMOD寄存器 

==一般设为0x01(12分频，时钟为1MHZ，开启计数溢出中断)==

==不用中断0x00；==

![img](E:\a蓝桥杯资料\个人\蓝桥杯代码考点整理\91a69f2d8ac07cb17a47ef9ef1d6b8c7.png)

### （2）CCON寄存器:主要用于中断的设置,以及计时器的启动

==初始化时全部置0即可==

![img](E:\a蓝桥杯资料\个人\蓝桥杯代码考点整理\dfe73b99d3431fc3386fae9e659843ca.png)



### （3）CCAPMx寄存器(由于CCAPM0连接了P1^1端口，所以以CCAPM0举例)

==设置为0x11即可。由于接收到信号时RX位下降沿,所以CAPN0设为1，同时需要开启捕获标志CCF0==（CCON寄存器里）

![img](E:\a蓝桥杯资料\个人\蓝桥杯代码考点整理\dc55337dde3708077b202c6da39c6bf8.png)

## 3.捕获模式介绍

总结一下流程:在配置好寄存器后，打开计数标志位,在PCA捕获到信号后,计数器中CH,CL中的值会给到CCAPnH和CCAPnL中 ，通过读取这两个寄存器的值便可得到时间.

附:在PCA中,计时器溢出标志位和PCA模块标志位都会触发中断，在中断函数中注意判断是谁触发的中断并执行相应的操作.

![img](E:\a蓝桥杯资料\个人\蓝桥杯代码考点整理\45a7bced25a43218985e553583fd9ce6.png)

## 4、代码（机械狗模版）

### ultrasonic.h

```c
#ifndef __ULTRASONIC_H__
#define __ULTRASONIC_H__

/* 如果你发现测距的量程很短，那你应该看看
	 下载的时候IRC频率有没有调成12.000MHz，
	 而不是来质疑我的代码 (>w<) */
#include <STC15F2K60S2.H>

#include "main.h"
#include "base.h"

#include "intrins.h"

#define DISTANCE_OUTRANG (0xFFFF) //定义为超出量程时的距离数值

void TimerPCAInit();
void sand_ultrasonic();
void calculate_distance();
void read_distance();

extern uint16_t distance;
extern uint8_t ultrasonic_flag;

#endif
```

### ultrasonic.c

```C
#include "ultrasonic.h"

/* 如果你发现测距的量程很短，那你应该看看
	 下载的时候IRC频率有没有调成12.000MHz，
	 而不是来质疑我的代码 (>w<) */
void Delay13us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 36;
	while (--i);

}

void TimerPCAInit()
{
	CCON = 0;	//初始化PCA控制寄存器
						//PCA定时器停止
						//清除CF标志
						//清除模块中断标志
	CMOD = 0x01;		//设置PCA时钟源 1MHz
									//开启PCA定时器溢出中断
	CCAPM0 = 0x11;	//PCA模块0为下降沿触发
									//利用脉冲捕获来模拟P11下降沿外部中断
}

/* ultrasonic_flag 超声波状态标志
	 0：空闲阶段
	 1：发送阶段
	 2：确认接收到返回的超声波
	 3：接收不到超声波，或超出量程 */
uint8_t ultrasonic_flag = 0;
uint16_t distance = 0;

/* P11，即超声波RX，一旦出现下降沿，
	 则说明已经收到返回的超声波，进入此中断 */
void TimerPCAIsr() interrupt 7
{
	CR = 0; //优先结束计时
	

	if (ultrasonic_flag == 1) //当处于发送阶段
	{
		if (CCF0) /* 接收到返回的超声波 */
			ultrasonic_flag = 2; //确认接收到返回的超声波
		else if (CF) /* 超出量程 */
			ultrasonic_flag = 3; //接收不到超声波，或超出量程
		else
			ultrasonic_flag = 0; //意外情况
	}

//	else 这里本应该加这个else，但有bug，我暂时找不到
//		ultrasonic_flag = 0; //意外情况
	

	CF = 0;
	CCF0 = 0;
	CCF1 = 0; //这两个标志位虽然没有用到
	CCF2 = 0; //但是以防万一，导致中断卡住

}

/* 超声波发送 */
void sand_ultrasonic()
{ //计时在前还是发送在前都可以，在我的板子上我发现计时在前的数据比较准确
	/* 启动计时 */
	CH = 0x8D;		//设置定时初始值，这个值是这么来的：0x8D1C ≈ 65535 - 500（厘米） / 0.017（厘米/微秒），想改量程可以自己算
	CL = 0x1C;		//设置定时初始值
	/* 经过测试，板子能测的最长距离是4米（至少我这块板子是这样），
		 而定时器的理论量程远超4米，所以设置初值的意义在于缩短量程
		 （大概缩减到5米），以此来减少不必要的测量时间，
		 所以超声波传播的时间就是终值减初值 */
	CF = 0;		//清除CF标志
	

	/* 启动超声波发送 */
	EA = 0; //关闭中断，防止打断发送
	CR = 1;	//定时器开始计时
	P10 = 1; Delay13us(); P10 = 0; Delay13us(); //高电平在前低电平在后
	P10 = 1; Delay13us(); P10 = 0; Delay13us();
	P10 = 1; Delay13us(); P10 = 0; Delay13us();
	P10 = 1; Delay13us(); P10 = 0; Delay13us();
	P10 = 1; Delay13us(); P10 = 0; Delay13us();
	P10 = 1; Delay13us(); P10 = 0; Delay13us();
	P10 = 1; Delay13us(); P10 = 0; Delay13us();
	P10 = 1; Delay13us(); P10 = 0; //有时候，做人要直接一点（指直接复制8次）
	EA = 1; //重新打开中断

}

/* 计算距离 */
void calculate_distance()
{
	if (ultrasonic_flag == 2) //确认接收到返回的超声波
	{
//		distance = CCAP0H;
//		distance <<= 8;
//		distance |= CCAP0L;
//		distance -= 0x8D1C; //减掉定时器初值，减去初值原因在设置初值的地方有注解
//		distance = (float)distance * 0.017;
		distance = ((CCAP0H << 8 | CCAP0L) - 0x8D1C) * 0.017;
	}
	else //接收不到超声波，或超出量程
	{
		distance = DISTANCE_OUTRANG;
	}
}

/* 读取距离函数
	 不一定需要这么写
	 可以更据上面两个函数自行修改 */
void read_distance()
{ //可能你会觉得这个函数写的不好，但这应该是最简单的写法了
	if (ultrasonic_flag > 1)
	{
		calculate_distance();
		ultrasonic_flag = 0; //进入发送阶段
	}
	

	if (ultrasonic_flag == 0)
	{
		sand_ultrasonic();
		ultrasonic_flag = 1; //进入发送阶段
	}

}
```

# **超声波（PCA计时方法）**

## 代码

```c
sbit trig=P1^0;   //定义发送端
sbit echo=P1^1;   //定义接收端
unsigned int distance;      //距离数据
void Delay13us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 42;
	while (--i);

}

void Send_Wave()  //产生一个40Khz方波
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		trig=1;
		Delay13us();
		trig=0;
		Delay13us();
	}
}

unsigned int Get_Csb()
{
	

	unsigned int dis;
	CMOD&=0x00;   //1mhz

//	CH=0x00;      //计数高八位定时12T10001us
//	CL=0x00;      //计数低八位
	CH=0x8D;      //计数高八位定时12T10001us
	CL=0x1C;      //计数低八位
	Send_Wave();  //发送40Khz波
	CR=1;     //开始计时
	while((echo==1)&&(CF==0));//等待接收端接收到返回信号或者溢出
	CR=0; //关闭计时
	if(CF==0) //如果未超出测量范围，进行数据处理
	{
		//dis=((CH << 8 | CL)-0x8D1C) * 0.017;//cm
		dis=(unsigned int)((CH << 8 | CL)-0x8d1c) * 0.017;//cm
	}
	else  //如果超出测量范围，溢出标志位会硬件置一，我们需要在此软件清零
	{
	    CF=0;  //溢出标志位清零
		dis=65535;
	}
	return  dis;
}
```

# 超声波常见问题



## 1、能读到数据，但是数据异常（偏小，基本上在个位数）

原因：不同单片机可能对于40khz的方波的处理不一样

#### ==解决方法：12us，13us,14us的延迟都试一下==

# 串口类技巧

[蓝桥杯单片机串口通信学习提升笔记_基于单片机设计定时器用于生成方波,以及一个串口用于与上位机通信-CSDN博客](https://blog.csdn.net/qq_64257614/article/details/130755976?spm=1001.2014.3001.5502)

[蓝桥杯单片机串口通信接收字符串指令、发送拼接字符串_单片机串口接收字符串-CSDN博客](https://blog.csdn.net/qq_64257614/article/details/130808835)

## 1、串口重定向

[【蓝桥杯单片机组】串口通信_putchar重定向-CSDN博客](https://blog.csdn.net/amimax/article/details/125247095)

[51单片机串口通信调试printf函数重定向输出打印_51 printf 重定向-CSDN博客](https://blog.csdn.net/yangfeng20/article/details/116780703)

## （1）重定向printf函数

这个函数是不是很熟悉，而在串口当中我们也可以利用这个函数来实现串口打印到屏幕上，我们只需要头文件引用stdio，然后重定向putchar函数即可。

stdio.h 标准输入输出头文件

==**注意： 使用这个函数会占用大量资源，在比赛中注意**==

```c
void send_date(unsigned char date){
	SBUF = date;
	while(!TI);
	TI = 0;
}

char putchar (char ch){
	send_date(ch);
	return ch;
}
```

## 2、C语言中sprintf()和sscanf()函数详解

[C语言中sprintf()和sscanf()函数详解-CSDN博客](https://blog.csdn.net/mzgxinhua/article/details/140064734?ops_request_misc=%7B%22request%5Fid%22%3A%2275b9406bba158e4c78538028e5f71b9c%22%2C%22scm%22%3A%2220140713.130102334..%22%7D&request_id=75b9406bba158e4c78538028e5f71b9c&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduend~default-1-140064734-null-null.142^v102^pc_search_result_base9&utm_term=c语言sscanf与sprintf&spm=1018.2226.3001.4187)

在C语言中，sprintf()和sscanf()是两个非常重要且常用的函数，它们分别用于字符串的格式化输出和格式化输入。理解并熟练使用这两个函数，可以极大地提高我们在数据处理和格式转换方面的效率。本文将详细介绍sprintf()和sscanf()函数的用法、原理、常见用例以及注意事项。

1. ### sprintf()函数详解

- ## 描述

  C 库函数 **int sprintf(char \*str, const char \*format, ...)** 发送格式化输出到 **str** 所指向的字符串。

  ## 声明

  下面是 sprintf() 函数的声明。

  ```c
  int sprintf(char *str, const char *format, ...)
  ```

  ## 参数

  - **str** -- 这是指向一个字符数组的指针，该数组存储了 C 字符串。
  - **format** -- 这是字符串，包含了要被写入到字符串 str 的文本。它可以包含嵌入的 format 标签，format 标签可被随后的附加参数中指定的值替换，并按需求进行格式化。format 标签属性是 **%[flags][width][.precision][length]specifier**，具体讲解如下：

## 返回值

如果成功，则返回写入的字符总数，不包括字符串追加在字符串末尾的空字符。如果失败，则返回一个负数。

1.2 基本用法
以下是一个简单的示例，展示如何使用sprintf()函数将整数和浮点数格式化为字符串：

```C
#include <stdio.h>

int main() {
    char buffer[50];
    int n = 10;
    float f = 3.14;

    sprintf(buffer, "Integer: %d, Float: %.2f", n, f);
    printf("%s\n", buffer); // 输出：Integer: 10, Float: 3.14
     
    return 0;

}
```

在这个示例中，sprintf()将整数n和浮点数f按照格式控制字符串的要求写入buffer中，然后通过printf()输出buffer的内容。

1.3 常见格式控制符
sprintf()的格式控制符与printf()相同，常见的格式控制符包括：

%d：格式化整数。
%f：格式化浮点数。
%s：格式化字符串。
%c：格式化字符。
%x：格式化为十六进制整数。
%o：格式化为八进制整数。
以下是一些常见的示例：

```c
#include <stdio.h>

int main() {
    char buffer[100];
    

    int a = 255;
    sprintf(buffer, "Decimal: %d, Hex: %x, Octal: %o", a, a, a);
    printf("%s\n", buffer); // 输出：Decimal: 255, Hex: ff, Octal: 377
    
    float pi = 3.14159;
    sprintf(buffer, "Pi: %.2f", pi);
    printf("%s\n", buffer); // 输出：Pi: 3.14
    
    char str[] = "Hello, world!";
    sprintf(buffer, "String: %s", str);
    printf("%s\n", buffer); // 输出：String: Hello, world!
    
    return 0;


```

2. ### sscanf()函数详解

  #### 2.1 sscanf()的定义和用法

## 描述

C 库函数 **int sscanf(const char \*str, const char \*format, ...)** 从字符串读取格式化输入。

## 声明

下面是 sscanf() 函数的声明。

```c
int sscanf(const char *str, const char *format, ...)
```

## 参数

- **str** -- 这是 C 字符串，是函数检索数据的源。

- **format** -- 这是 C 字符串，包含了以下各项中的一个或多个：*空格字符、非空格字符* 和 *format 说明符*。
  format 说明符形式为 **[=%[\*][width][modifiers]type=]**，具体讲解如下：

  ## 返回值

  ==如果成功，该函数返回成功匹配和赋值的个数。如果到达文件末尾或发生读错误，则返回 EOF。==判断是否解析成功

以下是一个简单的示例，展示如何使用sscanf()函数从字符串中读取整数和浮点数：

```C
#include <stdio.h>

int main() {
    char buffer[] = "10 3.14";
    int n;
    float f;

    sscanf(buffer, "%d %f", &n, &f);
    printf("Integer: %d, Float: %.2f\n", n, f); // 输出：Integer: 10, Float: 3.14
     
    return 0;

}
```

在这个示例中，sscanf()从buffer字符串中读取一个整数和一个浮点数，并将它们分别存储在n和f中，然后通过printf()输出这些值。

#### 2.3 常见格式控制符

sscanf()的格式控制符与scanf()相同，常见的格式控制符包括：

%d：读取整数。
%f：读取浮点数。
%s：读取字符串。
%c：读取字符。
%x：读取十六进制整数。
%o：读取八进制整数。
以下是一些常见的示例：

```C
#include <stdio.h>

int main() {
    char buffer[] = "255 ff 377";
    int decimal, hex, octal;

    sscanf(buffer, "%d %x %o", &decimal, &hex, &octal);
    printf("Decimal: %d, Hex: %x, Octal: %o\n", decimal, hex, octal); // 输出：Decimal: 255, Hex: ff, Octal: 377
     
    char str[] = "Hello, world!";
    char extractedStr[20];
    sscanf(str, "%s", extractedStr);
    printf("Extracted String: %s\n", extractedStr); // 输出：Extracted String: Hello,
     
    return 0;

}
```

2.4 sscanf()的高级用法
处理复杂字符串
sscanf()可以处理复杂的字符串，例如：

```C
#include <stdio.h>

int main() {
    char buffer[] = "Name: John, Age: 25, Height: 5.9";
    char name[20];
    int age;
    float height;

    sscanf(buffer, "Name: %s, Age: %d, Height: %f", name, &age, &height);
    printf("Name: %s, Age: %d, Height: %.1f\n", name, age, height); // 输出：Name: John, Age: 25, Height: 5.9
     
    return 0;

}
```

在这个示例中，sscanf()从buffer字符串中提取了名称、年龄和身高，并分别存储在name、age和height中。

字符串拆分

```c
sscanf()还可以用于字符串拆分，例如：

#include <stdio.h>

int main() {
    char buffer[] = "key1=value1;key2=value2;key3=value3";
    char key1[10], value1[10], key2[10], value2[10], key3[10], value3[10];

    sscanf(buffer, "%[^=]=%[^;];%[^=]=%[^;];%[^=]=%s", key1, value1, key2, value2, key3, value3);
    printf("%s: %s\n", key1, value1); // 输出：key1: value1
    printf("%s: %s\n", key2, value2); // 输出：key2: value2
    printf("%s: %s\n", key3, value3); // 输出：key3: value3
     
    return 0;

}
```

在这个示例中，sscanf()通过分隔符将buffer字符串拆分成键值对，并分别存储在对应的变量中。



## 3、清空字符串缓存区

[C语言直接清空数组、字符串_字符数组清空-CSDN博客](https://blog.csdn.net/cool99781/article/details/106388593)

[c语言如何让字符串清空 | PingCode智库](https://docs.pingcode.com/baike/1057181)

（1）一、直接设置第一个字符为终止符

```c
char str[100] = "Hello, World!";
uart_tx_buf[0] = 0;    // 等价于 uart_tx_buf[0] = '\0';
```

（2）循环方法

```c
char a[ ]="aaaaaaaa";               //定义字符数组
for (unsigned int i = 0; i < strlen(a); i++)
      a[i] = '\0' ;                      //for循环清空数组 
```

（3） memset函数方法

```c
memset包含在头文件string.h中，函数原型为：memset(void *s,int ch,size_t n)。
char a[ ]="aaaaaaaa";            //定义字符数组
memset(a, 0, sizeof (a);          //清空数组 
//声明
//void *memset(void *ptr, int value, size_t num);
//下面是 memset() 函数的声明。

//void *memset(void *str, int c, size_t n)
//参数
//str -- 指向要填充的内存区域的指针。
//c -- 要设置的值，通常是一个无符号字符。
//n -- 要被设置为该值的字节数。
//返回值
//该值返回一个指向存储区 str 的指针。
```

（4）strcpy
直接使用strcpy将一个空串赋值给字符串就可以，需要string.h

```c
char ss[11] = {"hello world"}; //当前为hello world
strcpy(ss, "");
//现在的ss就是空串了
```

## 4、sprintf的数据转换格式问题

[蓝桥杯单片机-国赛4——基于sprintf()函数的串口通讯收与发_keil sprintf-CSDN博客](https://blog.csdn.net/qq_43323677/article/details/139161964)

[C51中的sprintf()函数使用_c51 sprintf-CSDN博客](https://blog.csdn.net/ChenWenHaoHaoHao/article/details/128695081)

### （1）问题：如果sprintf中的输出变量类型与输入的变量不一样，由于数据格式不同就会导致打印的数据出错

eg：

```c
//time_10[2]为unsigned char类型，假设当前值为2

sprintf(uart_tx_buf,"sd:%d \r\n",time_10[2]);

//输出结果为：sd:512
```

==错误原因就是数据格式不匹配。==

### （2）在实际操作中，有以下笔记：

官方数据格式表

\1) 格式字符及说明如下表所示。

| 格式字符 | 说明                                 | 示例               |
| -------- | ------------------------------------ | ------------------ |
| d 或 i   | 带符号十进制整数                     | 26、 285           |
| o        | 无符号八进制整数                     | 316、 501          |
| x        | 无符号十六进制整数                   | 8cf、32b7          |
| X        | 无符号十六进制整数（以大写形式输出） | 8CF、32B7          |
| u        | 无符号十进制整数                     | 1234、 32727       |
| f        | 小数形式的单、双精度浮点数           | 102.6、2987.58     |
| e 或 E   | 指数形式的单、双精度浮点数           | 1.026e+2、1.026E+2 |
| g 或 G   | 以 %f 或 %e 的形式输出浮点数         | 102.6              |
| c        | 单个字符                             | 'a'、A             |
| s        | 字符串                               | “hello”            |
| p        | 输出一个指针                         | 0012FF7C           |


\2) flags 控制输出数据的对齐方式，flags 及说明如下表所示。



| flags | 说明                                                         |
| ----- | ------------------------------------------------------------ |
| -     | 在给定的域宽内靠左端输出                                     |
| +     | 强制在正数前输出 + 号，在负数前输出 - 号                     |
| #     | 使用 o、x 或 X 格式时，在数据前面分别增加前导符 0、0x 或 0X 输出 |


\3) width 控制数据输出的宽度，width 及说明如下表所示。



| width | 说明                                                         |
| ----- | ------------------------------------------------------------ |
| m     | 输出字段的宽度，如果数据的宽度小于 m，则左端补上空格；否则按照实际位数输出 |


\4) precision 表示单精度和双精度浮点数的小数点个数，precision 及说明如下表所示。



| precisi on | 说明                                                         |
| ---------- | ------------------------------------------------------------ |
| n          | 对于浮点数，表示输出 n 位小数；对于字符串，表示输出字符串的个数 |


\5) length 表示整型数据是长整型还是短整型，length 及说明如下表所示。



| length | 说明                                               |
| ------ | -------------------------------------------------- |
| h      | 用来输出短整型数据（只对 i、d、0、u、x 和 X 有效） |
| l 或 L | 用来输出长整型数据和双精度型数据                   |

#### 3、argument

表示要格式化输出的数据，分别与格式字符对应。



存储缓冲区数组：

        我一般定义为unsigned char ri_value[8]，基本可以满足测试需要。可以根据实际赛题调整大小和类型
        由于是数组，因此ri_value就是一个指向索引值0的数组指针，可以直接写进sprintf函数

格式化字符：

        unsigned char : %bu（无符号字符格式输出）
        char : %bd（有符号字符格式输出）
        unsigned int、 int :%d
        float: %f （带8个0的浮点数输出）、%.2f（只带两位小数的四舍五入数值输出）
        数组：%s（将存储在数组中的数字，转变为字符形式输出）
        
      b 表示字节数据；
    
      bd 表示 8 位有符号整数（256）（KEIL里扩展出了b,h,l来对输入字节宽的设置：b八位(256)；h十六位(65535)；l三十二位）；
    
      bu 表示 8 位无符号整数；
    
      bx 表示 8 位十六进制整数；
    
      小数用f表示。
        此外：每一个格式化字符后要带一个\r\n，且顺序固定，用于在显示窗口换行

被格式化的变量：

        直接格式化：可以是温度传感器数值，时间数值……
        间接格式化：通常是我们从上位机接收的数据，要先放入一个数组中，在进行格式化转换

#### ==解决方法1:严格按照格式书写==

#### ==解决方法2:输入之前对待处理数据做强制转换即可==

### （3）实际编程中的sprintf()函数

![image-20250403171203686](E:\a蓝桥杯资料\个人\蓝桥杯代码考点整理\image-20250403171203686.png)

# **C语言技巧**

## **1、处理小数点后几位，四舍五入问题**

参考来源：[C语言学习——保留小数点后n位并四舍五入_c语言输入一个double类型数据,然后输入一个整数n代表需要保留的小数位数,自己实现-CSDN博客](https://blog.csdn.net/yajing1219/article/details/113568095)

### 方法1：针对用于运算时

 如一个double类型的变量x=123.456700，使该数保留**小数点后两位**，对第三位小数进行**四舍五入**处理，再输出。

```c
scanf("%lf",&x);

x=(int)100*x+0.5;

x=x/100;

printf("%f\n",x);
```

若保留三位小数，将100换成1000，以此类推。

### 方法2：利用printf自动四舍五入

 小数点后保留2位：

```cpp
printf("%.2f\n",x);
```

小数点后保留3位：

```cpp
printf("%.3f\n",x);
```

小数点后保留4位：

```cpp
printf("%.4f\n",x);
```

##  2、常用头文件函数

### （1）math.h



[第五章 C语言标准库＜math.h＞头文件用法汇总_math头文件-CSDN博客](https://blog.csdn.net/weixin_39972972/article/details/144967813)

## 3、常用头文件函数stdio.h



## 4、常用头文件函数string.h



[C语言string.h头文件函数_strrchr c语言头文件-CSDN博客](https://blog.csdn.net/creat2012/article/details/20548633)

## 5、C语言中的大小端转换与高低位颠倒

[C语言中的大小端转换与高低位颠倒_结构体的高低位翻转了怎么办-CSDN博客](https://blog.csdn.net/hexuyangguang/article/details/107554825)







