#include "reg51.h"
#include "intrins.h"
#define Display   P0
#define Matrix    P1
sbit Z = P2^2;
sbit Y = P2^3;
sbit X = P2^4;

unsigned char code list[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};    //在数码管上0~9对应的Hex
unsigned char Record[8] = {0};
unsigned int  FirstNum, SecNum = 0, result = 0;
unsigned char KeyValue = 0;         //记录按键序号
unsigned char i = 0;                //记录已经输入了多少个数字
void Delay10ms();					          //延时10ms
void Delay20ms();					          //延时20ms
void KeyScan();						          //扫描矩阵按键以定位，确定输入的数值
void Show();							          //在动态数码管上显示信息
void Constr();
void Deconstr();
void Trans();
void Init();			                  //Initialization，初始化

void main(void)
{
	Display = 0x3f;
	
	while (1)
	{
		Matrix = 0x0f;
		if (Matrix != 0x0f)
		{
			KeyScan();
			Trans();
			switch (KeyValue)
			{
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				{
					unsigned char j;
					i++;
					if (i == 1)
						Record[0] = KeyValue;
					else
						for (j = i ; j > 0; j--)
							Record[j] = Record[j - 1];
					Record[0] = KeyValue;
					KeyValue = 16;
					Show();
					break;
				}
	//			case 'c':
	//			case 'a':
	//			case 'm':
	//			case 'M':
	//			case 'd':
			}
		}
		Show();
	}
}

/*------------------------延时函数-----------------------------*/
void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}

void Delay20ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 1;
	j = 216;
	k = 35;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
/*-------------------------扫描矩阵键盘------------------------*/
void KeyScan()
{
	if (Matrix != 0x0f)
		Delay10ms();
	{
		if (Matrix != 0x0f)
		{
//			Matrix = 0x0f;
			KeyValue = 0;
			switch (Matrix)
				{
					case 0x07:
						KeyValue = 1; break;
					case 0x0b:
						KeyValue = 2; break;
					case 0x0d:
						KeyValue = 3; break;
					case 0x0e:
						KeyValue = 4; break;
				}
			Matrix = 0xf0;
			switch (Matrix)
			{
				case 0xe0:
					break;
				case 0xd0:
					KeyValue = KeyValue + 1 * 4; break;
				case 0xb0:
					KeyValue = KeyValue + 2 * 4; break;
				case 0x70:
					KeyValue = KeyValue + 3 * 4; break;
			}
		}
	}
	Delay20ms();
}
/*-----------------根据按键序号定义按键的功能---------------------*/
void Trans()
{
	switch (KeyValue)
	{
		case 1: KeyValue = 'c'; break; //清零，clean
		case 2: KeyValue = 1; break;
		case 3: KeyValue = 4;	break;
		case 4: KeyValue = 7; break;
		case 5: KeyValue = 0; break;
		case 6: KeyValue = 2; break;
		case 7: KeyValue = 5; break;
		case 8: KeyValue = 8; break;
		case 9: KeyValue = 'e'; break; //等于，equal
		case 10: KeyValue = 3; break;
		case 11: KeyValue = 6; break;
		case 12: KeyValue = 9; break;
		case 13: KeyValue = 'a'; break; //加，add
		case 14: KeyValue = 'm'; break; //减，minus
		case 15: KeyValue = 'M'; break; //乘，Multiplication
		case 16: KeyValue = 'd'; break; //除以，Division
	}
}
/*--------------------在动态数码管上显示信息---------------------*/
void Show()
{
	int j, x; 
	for (j = 0; j < i; j++)
	{
		switch(j)
		{
			case 0:
				X = 1; Y = 1; Z = 1; break;
			case 1:
				X = 1, Y = 1; Z = 0; break;
			case 2:
				X = 1; Y = 0; Z = 1; break;
			case 3:
				X = 1; Y = 0; Z = 0; break;
			case 4:
				X = 0; Y = 1; Z = 1; break;
			case 5:
				X = 0, Y = 1; Z = 0; break;
			case 6:
				X = 0; Y = 0; Z = 1; break;
			case 7:
				X = 0; Y = 0; Z = 0; break;
		}
		Display = list[Record[j]];
		x = 10;
		while (x--);
		Display = 0x00;
	}
}
void Init()
{
}