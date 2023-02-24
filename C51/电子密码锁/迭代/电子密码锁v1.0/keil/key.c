#include <reg52.h>
#include <lcd.h>
#include <key.h>
#include <save.h>


//  检测有按键按下并读取键值
void KeyDown(u8 *t, u8 *mode, u8 *k, u8 *arr, u8 *flag)
{	
	u8 flag_c = 0; //  清除标志
	GPIO_KEY=0xf0;
	if(GPIO_KEY!=0xf0) //  读取按键是否按下
	{
		delay_ms(3); //  延时10ms进行消抖
		if(GPIO_KEY!=0xf0) //  再次检测键盘是否按下
		{	
			//  测试行
			GPIO_KEY=0XF0;
			switch(GPIO_KEY)
			{
				case(0X70): if(*k<6){*flag=1;arr[*k]=7;}break;
				case(0Xb0):	if(*k<6){*flag=1;arr[*k]=4;}break;
				case(0Xd0): if(*k<6){*flag=1;arr[*k]=1;}break;
				case(0Xe0): flag_c=1;break;
				default: break;
			}
		}
	}
	GPIO_KEY=0x0f;
	if(GPIO_KEY!=0x0f) //  读取按键是否按下
	{
		delay_ms(3); //  延时10ms进行消抖
		if(GPIO_KEY!=0x0f) //  再次检测键盘是否按下
		{	
			//  测试列
			GPIO_KEY=0X0F;
			switch(GPIO_KEY)
			{
				case(0X07): if(flag_c==1)*flag=3;else if(*k<6)arr[*k]=arr[*k]+0;break;
				case(0X0b):if(*k<6){*flag=1;if(flag_c==1)arr[*k]=0;else arr[*k]=arr[*k]+1;}break; //  数字作用					
				case(0X0d): if(flag_c==1)*flag=2;else if(*k<6)arr[*k]=arr[*k]+2;break;
				case(0X0e): 
					if(*k==0&flag_c==1&*t==1){*mode+=1;if(*mode==3)*mode=0;} //  调整模式	
					else *flag=0;break;
				default: break;
			}
		}
	}
	while(GPIO_KEY==0X07||GPIO_KEY==0X0b||GPIO_KEY==0X0d||GPIO_KEY==0X0e); //  检测按键松手检测
}


//  输入密码
void InPut(u8 *k, u8 *flag, u8 code *start_line, u8 *arr)
{
	if(*flag==1) //  输入密码
	{	
		u8 str[1];
		itoa(arr[*k],str,10);
		if(*k<=6){show_string(0x49+*k,str);delay_ms(250);show_string(0x49+*k,"*");}
		*k += 1;
		*flag = 0;
	}
	if(*flag==2) // 清除
	{
		Show_string2(start_line);
		*k = 0;
		*flag = 0;
	}
}