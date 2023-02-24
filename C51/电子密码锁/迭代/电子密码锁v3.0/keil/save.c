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
void start() //����i2c
{
 SDA = 1;
 SCL = 1;
 delay();//��ʱ�������4.7us����ԼΪ��΢��
 SDA = 0; //��SCLΪ�ߵ�ƽʱ��SDAһ���½���Ϊ�����ź�
 delay();
}
//stop:
void stop() //ֹͣi2c
{
 SDA = 0;
 SCL = 1;
 delay();
 SDA = 1; //��SCLΪ�ߵ�ƽʱ��SDASDAһ��������Ϊֹͣ�ź�
 delay();
}
//ack:
void ack() //Ӧ���ź�0
{
 u8 i = 0; //�ȴ�����
 SCL = 1;//��SCLΪ�ߵ�ƽ�ڼ�ȴ�Ӧ��
 delay();
 while((SDA == 1) && i < 250)//��ΪӦ��0���˳����ӻ�����������Ӧ���ź�
 i++;//�ȴ�һ��ʱ��
 SCL = 0; //Ӧ��֮��SCL����
 delay();
}
//nack:
void nack() //��Ӧ��
{
 SCL = 1;//��SCLΪ�ߵ�ƽ�ڼ䣬��������ӻ�����һ��1����Ӧ���ź�
 delay();
 SDA = 1;
 SCL = 0; //Ӧ��֮��SCL����
 delay();
}

//���Ӧ��
bit cack()
{SDA = 1;
SCL =1;
F0= SDA;
SCL= 0;
return(F0);}
//����һ�ֽ�
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

void WRNB(u8 a[], u8 n, u8 sadr){//дAT24C>cx n�ֽ��Ӻ���//�βΣ�д����������a[]��д�������ֽ���n��д�뵥Ԫ�׵�ַsadr
unsigned char i;//������ű���i
unsigned int t;//������ʱ����t
start();//�������ź�
Wr1b(0xa0);//����дѰַ�ֽ�
cack();//���Ӧ��
Wr1b(sadr); //����д��AT24CXXƬ���ӵ�ַ
cack();//���Ӧ��
for(i=0;i<n;i++){//ѭ��д��n�ֽ�
Wr1b(a[i]);//д��I2Cһ���ֽ�
cack();}//���Ӧ��
stop();//n������д����ϣ�����ֹ��
for(t=0;t<1000;t++);}//ҳд��ʱ5ms

void RDNB(u8 b[], u8 n, u8 sadr){//��AT24Cxxn�ֽ��Ӻ���//�βΣ�������������b[]�����������ֽ���n��������Ԫ�׵�ַsadr
unsigned char i;//������ű���i
start(); //�������ź�
Wr1b(0xa0);//����дѰַ�ֽ�
cack();//���Ӧ��
Wr1b(sadr);//���Ͷ�AT24CXXƬ���׵�ַ
cack();//���Ӧ��
start();//�ٴη������ź�
Wr1b(0xa1);//���Ͷ�Ѱַ�ֽ�
cack();//���Ӧ��
for(i=0;i<n-1;i++){//ѭ������(n-1)���ֽ�
  b[i]=Rd1b();//����1���ֽ�
ack();}//����Ӧ��A
b[i]=Rd1b();//�������һ���ֽ�
nack();//����Ӧ��a
stop();}//n�����ݽ�����ϣ�����ֹ

