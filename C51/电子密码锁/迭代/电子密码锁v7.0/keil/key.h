#ifndef __KEY_H__
#define __KEY_H__


/*---�˿ڶ���---*/
#define GPIO_KEY P1
sbit ALERT = P2^1;
sbit LED = P3^6;


/*---��������---*/
void KeyDown(u8 *t, u8 *mode, u8 *k, u8 *arr, u8 *flag); //  ����а������²���ȡ��ֵ
void InPut(u8 *k, u8 *flag, u8 code *start_line, u8 *arr);


#endif