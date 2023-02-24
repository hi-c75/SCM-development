#ifndef __SAVE_H__
#define __SAVE_H__
 

/*---�˿ڶ���---*/
#define WriteDeviceAddress 0xA0
#define ReadDeviceAddress 0xA1
sbit SDA = P3^5;	                        //������
sbit SCL = P3^4;	                        //ʱ����

/*---��������---*/
void Start(); //  ����
void Stop(); //  ֹͣ
bit Write8bit(unsigned char input); //  д����	
unsigned char Read8Bit(); //  ������
void Ack(); //  Ӧ��	
void NoAck(); //  ��Ӧ��
bit TestAck(); //  ���Ӧ��
void WriteI2C(unsigned char *Wdata, unsigned char RomAddress, unsigned char number); //  дʱ��
void ReadI2C(unsigned char* RamAddress,unsigned char RomAddress,unsigned char bytes); //  ��ʱ��
char* itoa(int num,char* str,int radix); //  ����ת���ַ���


#endif