#include <reg52.h>
#include <intrins.h>
#include <save.h>
#include <lcd.h>

void nod2()
{
	u8 i;
	for(i=0;i<13;i++)_nop_();
}

void Start() //  ����
{
  SDA = 1;	                      	//������ʼ���������ź�
	nod2();
	SCL = 1;
	nod2();
	SDA = 0;	                      	//������ʼ�ź�
	nod2();
	SCL = 0;	                        //ʱ�Ӳ���
	nod2();
}

void Stop() //  ֹͣ
{
  SDA = 0;	                        //���ͽ��������������ź�		                  	//���ͽ���������ʱ���ź�
	nod2();
	SCL = 1;	                        //������������ʱ�����4us
	nod2();
	SDA = 1;	                        //����I2C���߽�������
	nod2();
	SCL = 0;
	nod2();
}

bit Write8bit(unsigned char input) //  д����
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

unsigned char Read8Bit() //  ������
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

void Ack() //  Ӧ��
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

void NoAck() //  ��Ӧ��
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

bit TestAck() //  ���Ӧ��
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

void WriteI2C(unsigned char *Wdata, unsigned char RomAddress, unsigned char number) //  дʱ��
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

void ReadI2C(unsigned char* RamAddress,unsigned char RomAddress,unsigned char bytes) //  ��ʱ��
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
	  *RamAddress = Read8Bit(); //��ȡ����
	  Ack();//Ȼ�����Ӧ��
	  RamAddress++; 
	  bytes--;
	}
  *RamAddress = Read8Bit();
	NoAck();//��ȡ����������һ����Ӧ���ź�
	Stop();
}

char* itoa(int num,char* str,int radix)
{
    char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";//������
    unsigned unum;//���Ҫת���������ľ���ֵ,ת�������������Ǹ���
    int i=0,j,k;//i����ָʾ�����ַ�����Ӧλ��ת��֮��i��ʵ�����ַ����ĳ��ȣ�ת����˳��������ģ��������������k����ָʾ����˳��Ŀ�ʼλ��;j����ָʾ����˳��ʱ�Ľ�����
		char temp;
    //��ȡҪת���������ľ���ֵ
    if(radix==10&&num<0)//Ҫת����ʮ�����������Ǹ���
    {
        unum=(unsigned)-num;//��num�ľ���ֵ����unum
        str[i++]='-';//���ַ�����ǰ������Ϊ'-'�ţ�����������1
    }
    else unum=(unsigned)num;//����numΪ����ֱ�Ӹ�ֵ��unum
 
    //ת�����֣�ע��ת�����������
    do
    {
        str[i++]=index[unum%(unsigned)radix];//ȡunum�����һλ��������Ϊstr��Ӧλ��ָʾ������1
        unum/=radix;//unumȥ�����һλ
 
    }while(unum);//ֱ��unumΪ0�˳�ѭ��
 
    str[i]='\0';//���ַ���������'\0'�ַ���c�����ַ�����'\0'������
 
    //��˳���������
    if(str[0]=='-') k=1;//����Ǹ��������Ų��õ������ӷ��ź��濪ʼ����
    else k=0;//���Ǹ�����ȫ����Ҫ����//��ʱ��������������ֵʱ�õ�
    for(j=k;j<=(i-1)/2;j++)//ͷβһһ�Գƽ�����i��ʵ�����ַ����ĳ��ȣ��������ֵ�ȳ�����1
    {
        temp=str[j];//ͷ����ֵ����ʱ����
        str[j]=str[i-1+k-j];//β����ֵ��ͷ��
        str[i-1+k-j]=temp;//����ʱ������ֵ(��ʵ����֮ǰ��ͷ��ֵ)����β��
    }
 
    return str;//����ת������ַ���
 
}

