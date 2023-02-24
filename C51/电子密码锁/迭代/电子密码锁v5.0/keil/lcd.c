#include <reg52.h>
#include <lcd.h>
#include <stdio.h>

//  延时函数 
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
//  向1602写命令函数   参数:com 命令  返回值 : 无
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
//  向1602写数据命令  参数: dat 数据  返回值: 无
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

//  LCD1602初始化函数
void LcdInit()
{
	LcdWriteCom(0x38);
	LcdWriteCom(0x0c);
	LcdWriteCom(0x06);
	LcdWriteCom(0x01);
	LcdWriteCom(0x80);
}

//  第一行显示字符串
void Show_string1(u8 a[])
{
	u8 i;
	LcdWriteCom(0x80);
	for(i=0;a[i]!='\0';i++)
	{
		LcdWriteData(a[i]);
	}
}

//  第二行显示字符串
void Show_string2(u8 a[])
{
	u8 i;
	LcdWriteCom(0x80+0X40);
	for(i=0;a[i]!='\0';i++)
	{
		LcdWriteData(a[i]);
	}
}

//  指定位置显示字符串  参数 add 地址 第一行 0~27  第二行 40~67  a[] 字符串 "abc" 
void show_string(u8 add,u8 a[])
{
	u8 i;
	LcdWriteCom(0x80+add);
	for(i=0;a[i]!='\0';i++)
	{
		LcdWriteData(a[i]);
	}
}

//  任意位置显示数字 
void show_number(u8 add,u16 a)
{
	char s[10];
	sprintf(s,"%d",a);				
	show_string(add,(u8 *)s);	//  格式转换
}
