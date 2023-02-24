#include <reg52.h>
#include <intrins.h>
#include <save.h>
#include <lcd.h>


void Start() //  ����
{
  SDA = 1;	                      	//������ʼ���������ź�
	delay_ms(1);
	SCL = 1;
	delay_ms(1);
	SDA = 0;	                      	//������ʼ�ź�
	delay_ms(1);
	SCL = 0;	                        //ʱ�Ӳ���
	delay_ms(1);
}

void Stop() //  ֹͣ
{
  SCL = 0;	                        //���ͽ��������������ź�		                  	//���ͽ���������ʱ���ź�
	delay_ms(1);
	SDA = 0;	                        //������������ʱ�����4us
	delay_ms(1);
	SCL = 1;	                        //����I2C���߽�������
	delay_ms(1);
	SDA = 1;
	delay_ms(1);
}

bit Write8bit(u8 input) //  д����
{
  u8 tmp;
  for(tmp =0;tmp!=8;tmp++)
  {
		SDA = (bit)(input&0x80);
		delay_ms(1);
		SCL = 1;
		delay_ms(1);
		SCL = 0;
		delay_ms(1);
		input = input << 1;
  }
	return 1;
}

u8 Read8Bit() //  ������
{
	u8 tmp,k,rbyte;
	SCL=0;delay_ms(1);
	SDA=1;
	for(tmp=8;tmp!=0;tmp--)
	{
		SCL = 1;
		delay_ms(1);
		if(SDA==1)k=1;else k=0;
		rbyte = rbyte<<1;
		rbyte = rbyte|k;
		SCL = 0;		
	}
	delay_ms(1);
	return (rbyte);
}


void Ack() //  Ӧ��
{
	SDA = 0;
	delay_ms(1);
	SCL = 1;
	delay_ms(1);
	SCL = 0;
	delay_ms(1);
	SDA = 1;
	delay_ms(1);
}

void NoAck() //  ��Ӧ��
{
	
	SDA = 1;
	delay_ms(1);
	SCL = 1;
	delay_ms(1);
	SCL = 0;
	delay_ms(1);
}

bit TestAck() //  ���Ӧ��
{
  bit ErrorBit;
  SDA = 1;
  delay_ms(1);
  SCL = 1;
  delay_ms(1);
  ErrorBit = SDA;
  SCL = 0;
  delay_ms(1);
  return(ErrorBit);
}

void WriteI2C(u8 *Wdata, u8 RomAddress, u8 number) //  дʱ��
{
  Start();//�ȷ���һ����ʼ�ź�
  Write8bit(WriteDeviceAddress);//����0xA0(��0λΪ0��ʾд)
  if (TestAck()){  //��Ӧ��Ļ��Ǿ�ֱ��ֹͣ����
	Stop();
	return;
  }
  Write8bit(RomAddress); //������Ҫ����ĵ�ַ
  if (TestAck()){
	Stop();
  }
  for(;number!=0;number--){ //д������
	  Write8bit(*Wdata);
	  if (TestAck()){
		Stop();
		return;
	  }
  	 Wdata++;
  }
  Stop(); //����д����
  delay_ms(10);
}

void ReadI2C(u8* RamAddress,u8 RomAddress,u8 bytes) //  ��ʱ��
{
	Start();
	Write8bit(WriteDeviceAddress); //�ȷ��͵�оƬ��ַ0xa0
	 if (TestAck()){
		Stop();
	 }
	Write8bit(RomAddress);//�ڷ���Ҫ��ȡ�ĵ�ַ
	 if (TestAck()){
		Stop();
	 }
	Start(); //���¸�����ʼ�ź�
	Write8bit(ReadDeviceAddress); //����0A1
	 if (TestAck()){
		Stop();
	 }
	while(bytes != 1)
	{
	  *RamAddress++ = Read8Bit(); //��ȡ����
	  Ack();//Ȼ�����Ӧ��
	  bytes--;
	}
  *RamAddress = Read8Bit();
	NoAck();//��ȡ����������һ����Ӧ���ź�
	Stop();
}
