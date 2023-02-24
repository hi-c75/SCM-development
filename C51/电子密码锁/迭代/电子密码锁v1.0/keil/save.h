#ifndef __SAVE_H__
#define __SAVE_H__
 

/*---端口定义---*/
#define WriteDeviceAddress 0xA0
#define ReadDeviceAddress 0xA1
sbit SDA = P3^5;	                        //数据线
sbit SCL = P3^4;	                        //时钟线

/*---声明函数---*/
void Start(); //  启动
void Stop(); //  停止
bit Write8bit(unsigned char input); //  写数据	
unsigned char Read8Bit(); //  读数据
void Ack(); //  应答	
void NoAck(); //  非应答
bit TestAck(); //  检测应答
void WriteI2C(unsigned char *Wdata, unsigned char RomAddress, unsigned char number); //  写时序
void ReadI2C(unsigned char* RamAddress,unsigned char RomAddress,unsigned char bytes); //  读时序
char* itoa(int num,char* str,int radix); //  数字转换字符串


#endif