#ifndef __SAVE_H__
#define __SAVE_H__
 

/*---�˿ڶ���---*/
sbit SDA = P3^5;	                        //������
sbit SCL = P3^4;	                        //ʱ����

/*---��������---*/
void start(); //����i2c
void stop(); //ֹͣi2c
void ack(); //Ӧ���ź�0	
void nack(); //��Ӧ��
bit cack();
void Wr1b(unsigned char x);
unsigned char Rr1b();
void WRNB(unsigned char *a, unsigned char sadr , unsigned char n);
void RDNB(unsigned char *b, unsigned char sadr , unsigned char n);


#endif