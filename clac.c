#include "reg51.h"
#include "intrins.h"
#define Display   P0
#define Matrix    P1
sbit Z = P2^2;
sbit Y = P2^3;
sbit X = P2^4;

unsigned char code list[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};    //���������0~9��Ӧ��Hex
unsigned char Record[8];
long int  FirstNum, SecNum = 0, result = 0;								//��Keil��longռ4���ֽڣ���Χ -2147483648~+2147483647 
unsigned char KeyValue = 0;                               //��¼�������
unsigned char i = 0;                                      //��¼�Ѿ������˶��ٸ�����
unsigned char Law;                                        //��¼���㷨��
void Delay10ms();					                                //��ʱ10ms
void Delay20ms();					                                //��ʱ20ms
void KeyScan();						                                //ɨ����󰴼��Զ�λ��ȷ���������ֵ
void Show();							                                //�ڶ�̬���������ʾ��Ϣ
long int Constr();
void Deconstr();
void Trans();
void Init();			                  //Initialization����ʼ��

void main(void)
{
	Init();
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
			  case 'c': 
				{
				Init();
				break;
				}
	  		case 'a': 
				{
					FirstNum = Constr();
					Law = 'a';
					Init();
					break;
				}
			  case 'm':
				{
					FirstNum = Constr();
					Law = 'm';
					Init();
					break;
				}
			  case 'M':
				{
					FirstNum = Constr();
					Law = 'M';
					Init();
					break;
				}
  			case 'd':
				{
					FirstNum = Constr();
					Law = 'd';
					Init();
					break;
				}
				case 'e':
				{
					if (FirstNum == 0)
						_nop_();
					else 
					{
						SecNum = Constr();
						switch (Law)
						{
							case 'a': result = FirstNum + SecNum; break;
							case 'm': result = FirstNum - SecNum; break;                                  
							case 'M': result = FirstNum * SecNum; break;
							case 'd': result = FirstNum / SecNum; break;                              //Ŀǰ�������������㲢�����ƣ�1.����ʹ�� long int��2.��δ���С���������
						}
						Deconstr();
						i = 7;
						for (i = 0; Record[i] != 0; i++);
						Show();
					}
				}
			}
		}
		Show();
	}
}

/*------------------------��ʱ����-----------------------------*/
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
/*-------------------------ɨ��������------------------------*/
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
/*-----------------���ݰ�����Ŷ��尴���Ĺ���---------------------*/
void Trans()
{
	switch (KeyValue)
	{
		case 1: KeyValue = 'c'; break; //���㣬clean
		case 2: KeyValue = 1; break;
		case 3: KeyValue = 4;	break;
		case 4: KeyValue = 7; break;
		case 5: KeyValue = 0; break;
		case 6: KeyValue = 2; break;
		case 7: KeyValue = 5; break;
		case 8: KeyValue = 8; break;
		case 9: KeyValue = 'e'; break; //���ڣ�equal
		case 10: KeyValue = 3; break;
		case 11: KeyValue = 6; break;
		case 12: KeyValue = 9; break;
		case 13: KeyValue = 'a'; break; //�ӣ�add
		case 14: KeyValue = 'm'; break; //����minus
		case 15: KeyValue = 'M'; break; //�ˣ�Multiplication
		case 16: KeyValue = 'd'; break; //���ԣ�Division
	}
}
/*--------------------�ڶ�̬���������ʾ��Ϣ---------------------*/
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
/*-----------------------�ϳ�ʵ���������ֵ------------------------*/
long int Constr()
{
	unsigned char a, b;
	long int Temp;
	for (a = 0, b = 1, Temp = 0; a <= 7; a++)
	{
		Temp = Temp + Record[a] * b;
		b *= 10;
	}
	return Temp;
}
/*-----------------------�⹹���������������ĸ����������---------*/
void Deconstr()
{
	unsigned char a;
	for (a = 0; a<= 7; a++)
	{
		Record[a] = result % 10;
		result = (result - Record[a]) / 10;
	}
}

void Init()
{
	X = 1; Y = 1; Z = 1;
	Display = 0x3f;
	for (i = 0; i <= 7; i++)
	  Record[i] = 0;
	i = 0;
}