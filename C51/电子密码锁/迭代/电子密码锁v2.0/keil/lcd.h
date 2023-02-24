#ifndef __LCD_H__
#define __LCD_H__
 

/*---���������ͽ�����������---*/
typedef unsigned char  u8;  
typedef unsigned int  u16;
typedef unsigned long u32;

/*---�˿ڶ���---*/
#define GPIO_KEY P1
#define D P0	
sbit E=P2^7;
sbit RW=P2^6;
sbit RS=P2^5;

/*---��������---*/
void delay_ms(u8 xms); //  ��ʱ����
void delay_s(u8 xs); //  ��ʱ����
void LcdWriteCom(u8 com);	//  LCD1602д��8λ�����Ӻ���
void LcdWriteData(u8 dat); //  LCD1602д��8λ�����Ӻ���
void LcdInit();	//  LCD1602��ʼ���ӳ���
void Show_string1(u8 a[]); //  ��һ�д�ͷ��ʼ��ʾ �ַ�   "" ˫�ź���ֱ�Ӽ���ʾ���ݼ���
void Show_string2(u8 a[]);	//  �ڶ���     "" ˫�ź���ֱ�Ӽ���ʾ���ݼ���
void show_string(u8 add,u8 a[]); //  ָ��λ����ʾ�ַ��� ע�� ���λ��  0~27   40~67   ��ֵַ,"��ʾ�ַ���"
void show_number(u8 add,u16 a);	//  show_number(0x�� Ҫ��ʾ�ĵ�ַ������������
void show_float(u8 add,float a); //  ��ʾ������ 


#endif