#ifndef __SAVE_H__
#define __SAVE_H__
 

/*---�˿ڶ���---*/
#define DeviceAdress_WriteMode_Page1 0xA0
#define DeviceAdress_ReadMode_Page1 0xA1
#define I2Cdelay() {_nop_();_nop_();_nop_();_nop_();}
sbit SDA = P3^5;	                        //������
sbit SCL = P3^4;	                        //ʱ����

/*---��������---*/
void I2CStart(void);//IIC�����������׵Ļ����Խ�����������ʱ��ͼ����
void I2CStop(void);//IICֹͣ
bit I2CWrite(unsigned char dat);//IIC����д����������һ���ӻ�Ӧ��ֵ�������ж�д���Ƿ�ɹ���
unsigned char I2CReadnak();//IIC���߶����������ͷ�Ӧ���źţ�����ԭ����д��������
unsigned char I2CReadack();//IIC���߶�����������Ӧ���ź�
void Read(unsigned char *buf,unsigned char addr,unsigned char len);//buf-���ݽ���ָ�룬addr-C04��д�����ݵ���ʼ��ַ��len-��ȡ����
void Write(unsigned char *buf,unsigned char addr,unsigned char len);//buf-Դ����ָ�룬addr-C04�е���ʼ��ַ��len-д�볤��



#endif