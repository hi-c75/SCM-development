#ifndef __LCD_H__
#define __LCD_H__
 

/*---对数据类型进行声明定义---*/
typedef unsigned char  u8;  
typedef unsigned int  u16;
typedef unsigned long u32;

/*---端口定义---*/
#define GPIO_KEY P1
#define D P0	
sbit E=P2^7;
sbit RW=P2^6;
sbit RS=P2^5;

/*---声明函数---*/
void delay_ms(u8 xms); //  延时函数
void delay_s(u8 xs); //  延时函数
void LcdWriteCom(u8 com);	//  LCD1602写入8位命令子函数
void LcdWriteData(u8 dat); //  LCD1602写入8位数据子函数
void LcdInit();	//  LCD1602初始化子程序
void Show_string1(u8 a[]); //  第一行从头开始显示 字符   "" 双信号内直接加显示内容即可
void Show_string2(u8 a[]);	//  第二行     "" 双信号内直接加显示内容即可
void show_string(u8 add,u8 a[]); //  指定位置显示字符串 注意 算好位置  0~27   40~67   地址值,"显示字符串"
void show_number(u8 add,u16 a);	//  show_number(0x加 要显示的地址，变量或数字
void show_float(u8 add,float a); //  显示浮点数 


#endif