#ifndef __SAVE_H__
#define __SAVE_H__
 

/*---端口定义---*/
sbit SDA = P3^5;	                        //数据线
sbit SCL = P3^4;	                        //时钟线

/*---声明函数---*/
void start(); //启动i2c
void stop(); //停止i2c
void ack(); //应答信号0	
void nack(); //非应答
bit cack();
void Wr1b(unsigned char x);
unsigned char Rr1b();
void WRNB(unsigned char *a, unsigned char sadr , unsigned char n);
void RDNB(unsigned char *b, unsigned char sadr , unsigned char n);


#endif