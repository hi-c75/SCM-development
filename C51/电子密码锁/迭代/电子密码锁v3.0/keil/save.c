#include <reg52.h>
#include <save.h>
#include <lcd.h>


void delay()
{
	unsigned char a,b;
	for(b=1;b>0;b--)
		for(a=1;a>0;a--);
}

//start:
void start() //启动i2c
{
 SDA = 1;
 SCL = 1;
 delay();//延时必须大于4.7us，此约为五微秒
 SDA = 0; //在SCL为高电平时，SDA一个下降沿为启动信号
 delay();
}
//stop:
void stop() //停止i2c
{
 SDA = 0;
 SCL = 1;
 delay();
 SDA = 1; //在SCL为高电平时，SDASDA一个上升沿为停止信号
 delay();
}
//ack:
void ack() //应答信号0
{
 u8 i = 0; //等待变量
 SCL = 1;//在SCL为高电平期间等待应答
 delay();
 while((SDA == 1) && i < 250)//若为应答0即退出，从机向主机发送应答信号
 i++;//等待一段时间
 SCL = 0; //应答之后将SCL拉低
 delay();
}
//nack:
void nack() //非应答
{
 SCL = 1;//在SCL为高电平期间，由主机向从机发送一个1，非应答信号
 delay();
 SDA = 1;
 SCL = 0; //应答之后将SCL拉低
 delay();
}

//检查应答
bit cack()
{SDA = 1;
SCL =1;
F0= SDA;
SCL= 0;
return(F0);}
//发送一字节
void Wr1b(unsigned char x){
unsigned char i;
for(i=0;i<8;i++)
if((x&0x80)==0)SDA=0;
else SDA=1;
SCL=1;
SCL=0;
x<<=1;}

unsigned char Rd1b(){
unsigned char i,x=0;
SDA=1;
for(i=0;i<8;i++){
SCL=1;
x=(x<<1)|SDA;
SCL=0;}
return(x);}

void WRNB(u8 a[], u8 n, u8 sadr){//写AT24C>cx n字节子函数//形参：写入数据数组a[]，写入数据字节数n，写入单元首地址sadr
unsigned char i;//定义序号变量i
unsigned int t;//定义延时参数t
start();//发启动信号
Wr1b(0xa0);//发送写寻址字节
cack();//检查应答
Wr1b(sadr); //发送写入AT24CXX片内子地址
cack();//检查应答
for(i=0;i<n;i++){//循环写入n字节
Wr1b(a[i]);//写入I2C一个字节
cack();}//检查应答
stop();//n个数据写入完毕，发终止信
for(t=0;t<1000;t++);}//页写延时5ms

void RDNB(u8 b[], u8 n, u8 sadr){//读AT24Cxxn字节子函数//形参：接收数据数组b[]，接收数据字节数n，读出单元首地址sadr
unsigned char i;//定义序号变量i
start(); //发启动信号
Wr1b(0xa0);//发送写寻址字节
cack();//检查应答
Wr1b(sadr);//发送读AT24CXX片内首地址
cack();//检查应答
start();//再次发启动信号
Wr1b(0xa1);//发送读寻址字节
cack();//检查应答
for(i=0;i<n-1;i++){//循环读出(n-1)个字节
  b[i]=Rd1b();//接收1个字节
ack();}//发送应答A
b[i]=Rd1b();//接收最后一个字节
nack();//发送应答a
stop();}//n个数据接收完毕，发终止

