#ifndef __SAVE_H__
#define __SAVE_H__
 

/*---端口定义---*/
#define DeviceAdress_WriteMode_Page1 0xA0
#define DeviceAdress_ReadMode_Page1 0xA1
#define Nop() {_nop_();_nop_();_nop_();_nop_();}
sbit SDA = P3^5;	                        //数据线
sbit SCL = P3^4;	                        //时钟线

/*---声明函数---*/
void Start(void);
void Stop(void);
void Ack(void);
void NoAck(void);
void Send(unsigned char Data);
unsigned char Readb(void);
void Write(unsigned char Data[],unsigned char Address,unsigned char Num);
void Read(unsigned char Data[],unsigned char Address,unsigned char Num);



#endif