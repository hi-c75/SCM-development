#include <reg52.h>
#include <intrins.h>
#include <save.h>
#include <lcd.h>


/*起始条件*/

void Start(void)
{
 	SDA=1;
 	SCL=1;
 	Nop();
 	SDA=0;
 	Nop();
}


 /*停止条件*/
void Stop(void)
{
 	SDA=0;
 	SCL=1;
 	Nop();
 	SDA=1;
 	Nop();
}

/*应答位*/
void Ack(void)
{
	SDA=0;
	Nop();
	SCL=1;
	Nop();
	SCL=0;
}

/*反向应答位*/
void NoAck(void)
{
 	SDA=1;
 	Nop();
 	SCL=1;
 	Nop();
 	SCL=0;
}

 /*发送数据子程序，Data为要求发送的数据*/
void Send(u8 Data)
{
	u8 BitCounter=8;
	u8 temp;
	do
	{
		temp=Data;
		SCL=0;
		Nop();
		if((temp&0x80)==0x80)SDA=1;else SDA=0;
		SCL=1;
		temp=Data<<1;
		Data=temp;
		BitCounter--;
	}
	while(BitCounter);
	SCL=0;
}

/*读一字节的数据，并返回该字节值*/
u8 Readb(void)
{
  u8 temp=0;
	u8 temp1=0;
	u8 BitCounter=8;
	SDA=1;
	do{
		SCL=0;
		Nop();
		SCL=1;
		Nop();
		if(SDA)temp=temp|0x01;else temp=temp&0xfe;
		if(BitCounter-1){temp1=temp<<1;temp=temp1;}
		BitCounter--;
	}
	while(BitCounter);
	return(temp);
	}

void Write(u8 Data[],u8 Address,u8 Num)
{
  u8 i;
  u8 *PData;
  PData=Data;
  for(i=0;i<Num;i++){
		Start();
		Send(0xa0);
		Ack();
		Send(Address+i);
		Ack();
		Send(*(PData+i));
		Ack();
		Stop();
		delay_ms(20);
  }
}

void Read(u8 Data[],u8 Address,u8 Num)
{
  u8 i;
  u8 *PData;
  PData=Data;
  for(i=0;i<Num;i++){
		Start();
		Send(0xa0);
		Ack();
		Send(Address+i);
		Ack();
		Start();
		Send(0xa1);
		Ack();
		*(PData+i)=Readb();
		SCL=0;
		NoAck();
		Stop();
  }
}

