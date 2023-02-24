#include <reg52.h>
#include <intrins.h>
#include <save.h>
#include <lcd.h>


void I2CStart(void)//IIC启动，不明白的话可以结合上面给出的时序图分析
{
	SDA=1; 	
	SCL=1;     
	I2Cdelay();	
	SDA=0;         
	I2Cdelay();
	SCL=0;
}

void I2CStop(void)//IIC停止
{
	SCL=0; 
	SDA=0; 
 	I2Cdelay();
 	SCL=1;
 	I2Cdelay();
 	SDA=1;
 	I2Cdelay();
}

bit I2CWrite(unsigned char dat)//IIC总线写操作，返回一个从机应答值（用于判断写入是否成功）
{
 	bit ack;
 	unsigned char mask;//用来探测某一位的掩码变量

	for(mask=0x80;mask!=0;mask>>=1)
	{
  		if((mask&dat)==0)//该位的值输出到SDA上
   			SDA = 0;
  		else
   			SDA = 1;
  		I2Cdelay();
  		SCL = 1;
  		I2Cdelay();
  		SCL = 0;//完成一个位的周期
 	}
 	SDA = 1;//一个字节发完后，释放SDA，用于检测从机应答
 	I2Cdelay();
 	SCL = 1;
 	ack = SDA;//此时的SDA即为从机应答值
 	I2Cdelay();
 	SCL = 0;
 	return(~ack);
}

unsigned char I2CReadnak()//IIC总线读操作，发送非应答信号，大致原理与写操作类似
{
	unsigned char mask,dat;
	SDA = 1;
 
 	for(mask=0x80;mask!=0;mask>>=1)
 	{
  		I2Cdelay();
  		SCL = 1;
  		if(SDA == 0)
   			dat &= ~mask;
  		else
   			dat |= mask;
  		I2Cdelay();
  		SCL = 0;
 	}
 	SDA = 1;
 	I2Cdelay();
 	SCL = 1;
 	I2Cdelay();
 	SCL = 0;
 	return dat;
}

unsigned char I2CReadack()//IIC总线读操作，发送应答信号
{
	unsigned char mask,dat;
	SDA = 1;
 
 	for(mask=0x80;mask!=0;mask>>=1)
 	{
  		I2Cdelay();
  		SCL = 1;
  		if(SDA == 0)
   			dat &= ~mask;
  		else
   			dat |= mask;
  		I2Cdelay();
  		SCL = 0;
 	}
 	SDA = 0;
 	I2Cdelay();
 	SCL = 1;
 	I2Cdelay();
 	SCL = 0;
 	return dat;
}

void Read(unsigned char *buf,unsigned char addr,unsigned char len)//buf-数据接收指针，addr-C04中写入数据的起始地址，len-读取长度
{
	do{//查询当前是否能够进行读写操作
		I2CStart();
  		if(I2CWrite(DeviceAdress_WriteMode_Page1))//应答则跳出循环，否则进行下一次查询
   			break;
  		I2CStop();
 	  }while(1);
 	I2CWrite(addr);
 	I2CStart();
 	I2CWrite(DeviceAdress_ReadMode_Page1);
 	while(len>1)
 	{
 		*buf++ = I2CReadack();
  		len--;
 	}
 	*buf = I2CReadnak();
 	I2CStop();
}

void Write(unsigned char *buf,unsigned char addr,unsigned char len)//buf-源数据指针，addr-C04中的起始地址，len-写入长度
{
	while(len--)
	{
  		do{
   			I2CStart();
   			if(I2CWrite(DeviceAdress_WriteMode_Page1))
   			{
    				break;
   			}
   			I2CStop();
  		}while(1);
  		I2CWrite(addr++);
  		I2CWrite(*buf++);
  		I2CStop();
 	}
}
