#ifndef __KEY_H__
#define __KEY_H__


/*---端口定义---*/
#define GPIO_KEY P1
sbit ALERT = P2^1;
sbit LED = P3^6;


/*---声明函数---*/
void KeyDown(u8 *t, u8 *mode, u8 *k, u8 *arr, u8 *flag); //  检测有按键按下并读取键值
void InPut(u8 *k, u8 *flag, u8 code *start_line, u8 *arr);


#endif