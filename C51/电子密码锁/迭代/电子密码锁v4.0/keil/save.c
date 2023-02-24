#include <reg52.h>
#include <intrins.h>
#include <save.h>
#include <lcd.h>


void I2CStart(void)//IIC�����������׵Ļ����Խ�����������ʱ��ͼ����
{
	SDA=1; 	
	SCL=1;     
	I2Cdelay();	
	SDA=0;         
	I2Cdelay();
	SCL=0;
}

void I2CStop(void)//IICֹͣ
{
	SCL=0; 
	SDA=0; 
 	I2Cdelay();
 	SCL=1;
 	I2Cdelay();
 	SDA=1;
 	I2Cdelay();
}

bit I2CWrite(unsigned char dat)//IIC����д����������һ���ӻ�Ӧ��ֵ�������ж�д���Ƿ�ɹ���
{
 	bit ack;
 	unsigned char mask;//����̽��ĳһλ���������

	for(mask=0x80;mask!=0;mask>>=1)
	{
  		if((mask&dat)==0)//��λ��ֵ�����SDA��
   			SDA = 0;
  		else
   			SDA = 1;
  		I2Cdelay();
  		SCL = 1;
  		I2Cdelay();
  		SCL = 0;//���һ��λ������
 	}
 	SDA = 1;//һ���ֽڷ�����ͷ�SDA�����ڼ��ӻ�Ӧ��
 	I2Cdelay();
 	SCL = 1;
 	ack = SDA;//��ʱ��SDA��Ϊ�ӻ�Ӧ��ֵ
 	I2Cdelay();
 	SCL = 0;
 	return(~ack);
}

unsigned char I2CReadnak()//IIC���߶����������ͷ�Ӧ���źţ�����ԭ����д��������
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

unsigned char I2CReadack()//IIC���߶�����������Ӧ���ź�
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

void Read(unsigned char *buf,unsigned char addr,unsigned char len)//buf-���ݽ���ָ�룬addr-C04��д�����ݵ���ʼ��ַ��len-��ȡ����
{
	do{//��ѯ��ǰ�Ƿ��ܹ����ж�д����
		I2CStart();
  		if(I2CWrite(DeviceAdress_WriteMode_Page1))//Ӧ��������ѭ�������������һ�β�ѯ
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

void Write(unsigned char *buf,unsigned char addr,unsigned char len)//buf-Դ����ָ�룬addr-C04�е���ʼ��ַ��len-д�볤��
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
