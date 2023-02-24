#include <reg52.h>
#include <intrins.h>
#include <save.h>
#include <lcd.h>

void nod2()
{
	u8 i;
	for(i=0;i<13;i++)_nop_();
}

void Start() //  启动
{
  SDA = 1;	                      	//发送起始条件数据信号
	nod2();
	SCL = 1;
	nod2();
	SDA = 0;	                      	//发送起始信号
	nod2();
	SCL = 0;	                        //时钟操作
	nod2();
}

void Stop() //  停止
{
  SDA = 0;	                        //发送结束条件的数据信号		                  	//发送结束条件的时钟信号
	nod2();
	SCL = 1;	                        //结束条件建立时间大于4us
	nod2();
	SDA = 1;	                        //发送I2C总线结束命令
	nod2();
	SCL = 0;
	nod2();
}

bit Write8bit(unsigned char input) //  写数据
{
  unsigned char tmp;
  for(tmp =0;tmp!=8;tmp++)
  {
		input<<= 1;
		SDA = CY;
		nod2();
		SCL = 1;
		nod2();
		SCL = 0;
		nod2();
		SDA = 0;
		nod2();
  }
	return 1;
}

unsigned char Read8Bit() //  读数据
{
	unsigned char tmp,rbyte = 0;
	for(tmp=8;tmp!=0;tmp--)
	{
		SDA = 1;
		nod2();
		SCL = 1;
		nod2();
		rbyte = (rbyte<<1)|((u8)SDA);
		SCL = 0;
		nod2();		
	}
	return (rbyte);
}

void Ack() //  应答
{
	SDA = 0;
	nod2();
	SCL = 1;
	nod2();
	SCL = 0;
	nod2();
	SDA = 1;
	nod2();
}

void NoAck() //  非应答
{
	
	SDA = 1;
	nod2();
	SCL = 1;
	nod2();
	SCL = 0;
	nod2();
	SDA = 0;
	nod2();
}

bit TestAck() //  检测应答
{
  bit ErrorBit;
  SDA = 1;
  nod2();
  SCL = 1;
  nod2();
  ErrorBit = SDA;
  SCL = 0;
  nod2();
  return(ErrorBit);
}

void WriteI2C(unsigned char *Wdata, unsigned char RomAddress, unsigned char number) //  写时序
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

void ReadI2C(unsigned char* RamAddress,unsigned char RomAddress,unsigned char bytes) //  读时序
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
	  *RamAddress = Read8Bit(); //读取数据
	  Ack();//然后给个应答
	  RamAddress++; 
	  bytes--;
	}
  *RamAddress = Read8Bit();
	NoAck();//读取结束，发出一个非应答信号
	Stop();
}

char* itoa(int num,char* str,int radix)
{
    char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";//索引表
    unsigned unum;//存放要转换的整数的绝对值,转换的整数可能是负数
    int i=0,j,k;//i用来指示设置字符串相应位，转换之后i其实就是字符串的长度；转换后顺序是逆序的，有正负的情况，k用来指示调整顺序的开始位置;j用来指示调整顺序时的交换。
		char temp;
    //获取要转换的整数的绝对值
    if(radix==10&&num<0)//要转换成十进制数并且是负数
    {
        unum=(unsigned)-num;//将num的绝对值赋给unum
        str[i++]='-';//在字符串最前面设置为'-'号，并且索引加1
    }
    else unum=(unsigned)num;//若是num为正，直接赋值给unum
 
    //转换部分，注意转换后是逆序的
    do
    {
        str[i++]=index[unum%(unsigned)radix];//取unum的最后一位，并设置为str对应位，指示索引加1
        unum/=radix;//unum去掉最后一位
 
    }while(unum);//直至unum为0退出循环
 
    str[i]='\0';//在字符串最后添加'\0'字符，c语言字符串以'\0'结束。
 
    //将顺序调整过来
    if(str[0]=='-') k=1;//如果是负数，符号不用调整，从符号后面开始调整
    else k=0;//不是负数，全部都要调整//临时变量，交换两个值时用到
    for(j=k;j<=(i-1)/2;j++)//头尾一一对称交换，i其实就是字符串的长度，索引最大值比长度少1
    {
        temp=str[j];//头部赋值给临时变量
        str[j]=str[i-1+k-j];//尾部赋值给头部
        str[i-1+k-j]=temp;//将临时变量的值(其实就是之前的头部值)赋给尾部
    }
 
    return str;//返回转换后的字符串
 
}

