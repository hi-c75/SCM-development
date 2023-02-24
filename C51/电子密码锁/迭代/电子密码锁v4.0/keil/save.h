#ifndef __SAVE_H__
#define __SAVE_H__
 

/*---端口定义---*/
#define DeviceAdress_WriteMode_Page1 0xA0
#define DeviceAdress_ReadMode_Page1 0xA1
#define I2Cdelay() {_nop_();_nop_();_nop_();_nop_();}
sbit SDA = P3^5;	                        //数据线
sbit SCL = P3^4;	                        //时钟线

/*---声明函数---*/
void I2CStart(void);//IIC启动，不明白的话可以结合上面给出的时序图分析
void I2CStop(void);//IIC停止
bit I2CWrite(unsigned char dat);//IIC总线写操作，返回一个从机应答值（用于判断写入是否成功）
unsigned char I2CReadnak();//IIC总线读操作，发送非应答信号，大致原理与写操作类似
unsigned char I2CReadack();//IIC总线读操作，发送应答信号
void Read(unsigned char *buf,unsigned char addr,unsigned char len);//buf-数据接收指针，addr-C04中写入数据的起始地址，len-读取长度
void Write(unsigned char *buf,unsigned char addr,unsigned char len);//buf-源数据指针，addr-C04中的起始地址，len-写入长度



#endif