#include <reg52.h>
#include <intrins.h>
#include <save.h>
#include <lcd.h>


void Start() //  启动
{
  SDA = 1;	                      	//发送起始条件数据信号
	delay_ms(1);
	SCL = 1;
	delay_ms(1);
	SDA = 0;	                      	//发送起始信号
	delay_ms(1);
	SCL = 0;	                        //时钟操作
	delay_ms(1);
}

void Stop() //  停止
{
  SCL = 0;	                        //发送结束条件的数据信号		                  	//发送结束条件的时钟信号
	delay_ms(1);
	SDA = 0;	                        //结束条件建立时间大于4us
	delay_ms(1);
	SCL = 1;	                        //发送I2C总线结束命令
	delay_ms(1);
	SDA = 1;
	delay_ms(1);
}

bit Write8bit(u8 input) //  写数据
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

u8 Read8Bit() //  读数据
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


void Ack() //  应答
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

void NoAck() //  非应答
{
	
	SDA = 1;
	delay_ms(1);
	SCL = 1;
	delay_ms(1);
	SCL = 0;
	delay_ms(1);
}

bit TestAck() //  检测应答
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

void WriteI2C(u8 *Wdata, u8 RomAddress, u8 number) //  写时序
{
  Start();//先发送一个起始信号
  Write8bit(WriteDeviceAddress);//发送0xA0(第0位为0表示写)
  if (TestAck()){  //非应答的话那就直接停止返回
	Stop();
	return;
  }
  Write8bit(RomAddress); //发送你要存入的地址
  if (TestAck()){
	Stop();
  }
  for(;number!=0;number--){ //写入数据
	  Write8bit(*Wdata);
	  if (TestAck()){
		Stop();
		return;
	  }
  	 Wdata++;
  }
  Stop(); //结束写操作
  delay_ms(10);
}

void ReadI2C(u8* RamAddress,u8 RomAddress,u8 bytes) //  读时序
{
	Start();
	Write8bit(WriteDeviceAddress); //先发送的芯片地址0xa0
	 if (TestAck()){
		Stop();
	 }
	Write8bit(RomAddress);//在发送要读取的地址
	 if (TestAck()){
		Stop();
	 }
	Start(); //重新给个起始信号
	Write8bit(ReadDeviceAddress); //发送0A1
	 if (TestAck()){
		Stop();
	 }
	while(bytes != 1)
	{
	  *RamAddress++ = Read8Bit(); //读取数据
	  Ack();//然后给个应答
	  bytes--;
	}
  *RamAddress = Read8Bit();
	NoAck();//读取结束，发出一个非应答信号
	Stop();
}
