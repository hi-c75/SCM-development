#include <reg52.h>
#include <lcd.h>
#include <stdio.h>

//  ��ʱ���� 
void delay_ms(u8 xms)
{
	u8 i,j;
	for(i=xms;i>0;i--)
	for(j=112;j>0;j--);
}

void delay_s(u8 xs)
{
	u8 i,j;
	for (i=xs;i>0;i--)
	for(j=10;j>0;j--)
	delay_ms(100);
}
//  ��1602д�����   ����:com ����  ����ֵ : ��
void LcdWriteCom(u8 com)
{
	E=0;
	RW=0;
	RS=0;
	D=com;
	delay_ms(5);
	E=1;
	delay_ms(5);
	E=0;
}
//  ��1602д��������  ����: dat ����  ����ֵ: ��
void LcdWriteData(u8 dat)
{
	E=0;
	RW=0;
	RS=1;
	D=dat;
	delay_ms(5);
	E=1;
	delay_ms(5);
	E=0;
}

//  LCD1602��ʼ������
void LcdInit()
{
	LcdWriteCom(0x38);
	LcdWriteCom(0x0c);
	LcdWriteCom(0x06);
	LcdWriteCom(0x01);
	LcdWriteCom(0x80);
}

//  ��һ����ʾ�ַ���
void Show_string1(u8 a[])
{
	u8 i;
	LcdWriteCom(0x80);
	for(i=0;a[i]!='\0';i++)
	{
		LcdWriteData(a[i]);
	}
}

//  �ڶ�����ʾ�ַ���
void Show_string2(u8 a[])
{
	u8 i;
	LcdWriteCom(0x80+0X40);
	for(i=0;a[i]!='\0';i++)
	{
		LcdWriteData(a[i]);
	}
}

//  ָ��λ����ʾ�ַ���  ���� add ��ַ ��һ�� 0~27  �ڶ��� 40~67  a[] �ַ��� "abc" 
void show_string(u8 add,u8 a[])
{
	u8 i;
	LcdWriteCom(0x80+add);
	for(i=0;a[i]!='\0';i++)
	{
		LcdWriteData(a[i]);
	}
}

//  ����λ����ʾ���� 
void show_number(u8 add,u16 a)
{
	char s[10];
	sprintf(s,"%d",a);				
	show_string(add,(u8 *)s);	//  ��ʽת��
}
