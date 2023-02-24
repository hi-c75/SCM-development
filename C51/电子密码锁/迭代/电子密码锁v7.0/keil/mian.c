/**************************************************************************************
*		              电子密码锁												  *
硬件连接：
           P1 --> 矩阵键盘
					 P34--> EEPROM模块SCL
					 P35--> EEPROM模块SDA
					 P21--> 蜂鸣器
					 P36--> LED_GREEN
					 
					 矩阵键盘：
					 7      8      9       $
					 
					 4      5      6       $
					 
					 1      2      3       $
					 
					确定    0  清除/返回  模式
					
软件设置：
					main.c --> 主函数
					key.c  --> 按键矩阵模块
					lcd.c  --> LCD702模块
					save.c --> EEPROM模块（AT24C04A）

模式设置：
					模式0：登录界面
					模式1：修改密码界面
						- 修改初始密码
						- 修改用户添加密码
					模式2：添加新用户密码界面
					模式3：删除用户密码界面
					  - 删除所有用户添加密码
						- 删除一位用户添加密码
注意事项：
        ·密码只能设置为6位
				·新用户最多只能添加32位
				·密码错误3次自动锁死，只能通过初始密码解锁
				
存在问题：
        ·输入密码后确定存在一定等待时间（尚待改进）
        ·转换模式存在一定的延迟，卡顿（尚待改进）
***************************************************************************************/
#include <stdio.h>
#include <reg52.h>
#include <string.h>
#include <lcd.h> //  导入LCD702库
#include <key.h> //  导入矩阵按键库
#include <save.h> //  导入24C04库

/*---全局变量声明---*/
//  lcd
u8 code start_line[]	= {"password:       "}; 
u8 code name[] 	 	= {"Coded Lock( /3) "};	//显示名称
u8 code LockOpen[]	= {"      open      "};		//OPEN
u8 code ResetOK[] 	= {"ResetPasswordOK "};
u8 code initword[]	= {"Init password..."};
u8 code mode_line[] = {"==Mode Setting=="};
u8 code mode1[] = {"-Change Password"};
u8 code mode2[] = {"-Add New User   "};
u8 code mode2_0[] = {"=Change Setting="};
u8 code mode2_1[] = {"==>Init    Alone"};
u8 code mode2_2[] = {"   Init ==>Alone"};
u8 code mode3[] = {"-Delete Password"};
u8 code mode3_0[] = {"=Delete Setting="};
u8 code mode3_1[] = {"==>All     Alone"};
u8 code mode3_2[] = {"   All  ==>Alone"};
u8 code delete_s[] = {"Delete succeeded"};
u8 code delete_s2[] = {"No password set "};
u8 code Er_try[]		= {"error,try again!"};
u8 code try_again[] = {"Warn:A Lenth of "};
u8 code try_again2[] = {"password is six"};
u8 code username[] = {"=Administrator  "};
u8 code old_p[] = {"==Old Password=="};
u8 code new_p[] = {"==New Password=="};
u8 code warn_a[] = {"-Password exists"};
u8 code right_a[] = {"Adding succeeded"};
u8 code Locked[] = {"====UI locked==="};

//  AT24C02A
u8 CurrentPassword[6]; 			//读取EEPROM密码暂存数组
u8 TempPassword[6];
u8 InitPassword[6]={0,0,0,0,0,0}; //  初始密码
u8 num=1; //  设置密码人数
u8 t=0; //  是否设置初始密码
u8 flag_d=0; //  判断密码是否正确
u8 flag_c; //  修改密码所在位置
u8 flag_chg=0; // 删除选择
//u8 temp=1; // 修改密码限制（只在修改密码是运行1次）
u8 n_alert = 0; // 错误次数

//  按键
u8 flag=0; //  判断按键是否按下
u8 mode=0; //  模式设置
u8 k=0; //  输入密码位数



void main()
{
	u8 i,j; 
	LcdInit(); //  LCD702初始化函数 
//	Show_string1(initword);
//	Show_string2(start_line);
	while(1){
		//  初始密码设置
/*		while(mode==0&t==0) {
			KeyDown(&t, &mode, &k, TempPassword, &flag);
			InPut(&k, &flag, start_line, TempPassword);
			if(flag==3&k<5){flag=0;k=0;Show_string1(try_again);Show_string2(try_again2);delay_s(1);Show_string1(initword);Show_string2(start_line);}
			if(flag==3&k>=5){t=1;k=0;num+=1;flag=0;memcpy(InitPassword,TempPassword,sizeof(InitPassword));mode=0;Write(TempPassword,0,6);}			
		}*/
		// 初始密码输入
		while(mode==0&t==0){Write(InitPassword,0,6);t=1;} 
		
		//  模式0：登录界面
		if(mode==0) {		
			Show_string1(name);
			show_number(0x0b,3-n_alert);
			Show_string2(start_line);
			while(mode==0&t==1){	
				KeyDown(&t, &mode, &k, TempPassword, &flag);
				InPut(&k, &flag, start_line, TempPassword);
				if(flag==3&k<5){flag=0;k=0;Show_string1(try_again);Show_string2(try_again2);delay_s(1);Show_string1(name);show_number(0x0b,3-n_alert);Show_string2(start_line);}
				//  判断密码正误
				if(flag==3&k>=5){
					for(i=0;i<num;i++){
						Read(CurrentPassword,6*i,6);
						for(j=0;j<6;j++){
							if(CurrentPassword[j]==TempPassword[j]){flag_d=1;}
							else{flag_d=0;break;}
							//show_number(0x49+j,CurrentPassword[j]);
						}
						if(flag_d==1)break;
					}
					delay_s(2);
					if(flag_d==0){
						ALERT=0;delay_ms(200);ALERT=1;Show_string2(Er_try);n_alert+=1;delay_s(1);
						//  锁定
						if (n_alert==3){
							k=0,flag=0;
							while(n_alert==3){
								Show_string1(Locked);
								KeyDown(&t, &mode, &k, TempPassword, &flag);
								InPut(&k, &flag, start_line, TempPassword);
								if(flag==3&k<5){flag=0;k=0;Show_string1(try_again);Show_string2(try_again2);delay_s(1);Show_string1(Locked);Show_string2(start_line);}
								while(flag==3&k>=5){
									Read(CurrentPassword,0,6);
									for(j=0;j<6;j++){
										if(CurrentPassword[j]==TempPassword[j]){flag_d=1;}
										else{flag_d=0;break;}
										//show_number(0x49+j,InitPassword[j]);
									}
									if(flag_d==0){Show_string2(Er_try);delay_s(1);Show_string2(start_line);flag=0;k=0;}
									else {n_alert=0,k=0;Show_string1(name);show_number(0x0b,3-n_alert);Show_string2(start_line);}
								}
							}
						}else{Show_string1(name);show_number(0x0b,3-n_alert);Show_string2(start_line);}
					}
					else{Show_string2(LockOpen);n_alert=0;LED=0;delay_s(1);Show_string1(name);show_number(0x0b,3-n_alert);Show_string2(start_line);LED=1;}
					flag=0;
					k=0;
					flag_d=0;
				}
			}			
		}
		
		
		
    //  模式1：修改密码
		if(mode==1){ 
			while(mode==1){
				//  模式选择页面
				Show_string1(mode_line);
				Show_string2(mode1);
				KeyDown(&t, &mode, &k, TempPassword, &flag);
				if(flag==3){
					flag=0;
					mode=3;
					while(flag!=3){
						// 修改模式选择					
						Show_string1(mode2_0);
						if(mode==3){Show_string2(mode2_1);flag_chg=1;}
						if(mode==0){Show_string2(mode2_2);mode=2;flag_chg=2;}
						KeyDown(&t, &mode, &k, TempPassword, &flag);
						if(flag==2&k==0){flag=3;mode=1;k='a';}  // 返回
						//  初始密码修改
						if(flag==3&flag_chg==1&k!='a'){
							//  旧密码 
							flag=0;
							Show_string1(old_p);
							Show_string2(start_line);
							while(flag!=3){							
								KeyDown(&t, &mode, &k, TempPassword, &flag);
								if(flag==2&k==0){flag=3;mode=1;k='a';}  // 返回
								InPut(&k, &flag, &start_line, TempPassword);
								if(flag==3&k<5&k!='a'){flag=0;k=0;Show_string1(try_again);Show_string2(try_again2);delay_s(1);Show_string1(old_p);Show_string2(start_line);}
								//  初始密码浏览
								if(flag==3&k>=5&k!='a'){
									Read(CurrentPassword,0,6);
									for(j=0;j<6;j++){
										if(CurrentPassword[j]==TempPassword[j]){flag_d=1;}
										else{flag_d=0;break;}
									}
									if(flag_d==0){Show_string2(Er_try);delay_s(1);Show_string2(start_line);flag=0;k=0;}else {
										//  新密码
										flag=0 ;
										flag_d=0;
										k=0;
										Show_string1(new_p);
										Show_string2(start_line);
										while(flag!=3){							
											KeyDown(&t, &mode, &k, InitPassword, &flag);
											if(flag==2&k==0){flag=3;mode=1;k='a';}  // 返回
											InPut(&k, &flag, &start_line, InitPassword);
											if(flag==3&k<5&k!='a'){flag=0;k=0;Show_string1(try_again);Show_string2(try_again2);delay_s(1);Show_string1(new_p);Show_string2(start_line);}
											if(flag==3&k>=5&k!='a'){k='a';flag=3;mode=0;Write(InitPassword,0,6);Show_string2(ResetOK);delay_s(1);}
										}
									}
								}
							}
							flag_chg=0;
							flag=0;
							if(k=='a'){flag=3;mode=0;}
						}
						//  用户设置密码修改
						if(flag==3&flag_chg==2&k!='a'){
							if(num>1){
								//  旧密码 
								flag=0;
								Show_string1(old_p);
								Show_string2(start_line);
								while(flag!=3){							
									KeyDown(&t, &mode, &k, TempPassword, &flag);
									if(flag==2&k==0){flag=3;mode=1;k='a';}  // 返回
									InPut(&k, &flag, &start_line, TempPassword);
									if(flag==3&k<5&k!='a'){flag=0;k=0;Show_string1(try_again);Show_string2(try_again2);delay_s(1);Show_string1(old_p);Show_string2(start_line);}
									//  已保存密码浏览
									if(flag==3&k>=5&k!='a'){
										for(i=1;i<num;i++){
											Read(CurrentPassword,6*i,6);
											for(j=0;j<6;j++){
												if(CurrentPassword[j]==TempPassword[j]){flag_d=1;flag_c=6*i;}
												else{flag_d=0;break;}
											}
											if(flag_d==1)break;
										}
										if(flag_d==0){Show_string2(Er_try);delay_s(1);Show_string2(start_line);flag=0;k=0;}else {
											//  新密码
											flag=0 ;
											flag_d=0;
											k=0;
											Show_string1(new_p);
											Show_string2(start_line);
											while(flag!=3){							
												KeyDown(&t, &mode, &k, TempPassword, &flag);
												if(flag==2&k==0){flag=3;mode=1;k='a';}  // 返回
												InPut(&k, &flag, &start_line, TempPassword);
												if(flag==3&k<5&k!='a'){flag=0;k=0;Show_string1(try_again);Show_string2(try_again2);delay_s(1);Show_string1(new_p);Show_string2(start_line);}
												if(flag==3&k>=5&k!='a'){k='a';flag=3;mode=0;Write(TempPassword,flag_c,6);Show_string2(ResetOK);delay_s(1);}
											}
										}
									}
								}
								flag_chg=0;
								flag=0;
								if(k=='a'){flag=3;mode=0;}
							}else{Show_string2(delete_s2);delay_s(1);flag=0;mode=0;}
						}					
					}
				}
				flag_chg=0;
				flag=0;
				k=0;
		  }
    }
		
		
    
		//  模式2：添加新用户
		if(mode==2){ 
			while(mode==2){
				// 模式选择页面
				Show_string1(mode_line);
				Show_string2(mode2);
				KeyDown(&t, &mode, &k, TempPassword, &flag);
				
				// 页面设置
				if(flag==3){
					flag=0;
					Show_string1(username);
					show_number(0X0E,num);show_string(0X0F,"=");
					Show_string2(start_line);
					while(flag!=3){		
						KeyDown(&t, &mode, &k, TempPassword, &flag);
						if(flag==2&k==0){flag=3;mode=2;k='a';}  // 返回
						InPut(&k, &flag, start_line, TempPassword);
						if(flag==3&k<5&k!='a'){flag=0;k=0;Show_string1(try_again);Show_string2(try_again2);delay_s(1);Show_string1(username);show_number(0X0E,num);show_string(0X0F,"=");Show_string2(start_line);} //  密码位数不足
						if(flag==3&k>=5&k!='a'){
							for(i=0;i<num;i++){
								Read(CurrentPassword,6*i,6);
								for(j=0;j<6;j++){
									if(CurrentPassword[j]==TempPassword[j]){flag_d=1;}
									else{flag_d=0;break;}
								}
								if(flag_d==1)break;
							}
							if(flag_d==1){Show_string2(warn_a);delay_s(1);Show_string2(start_line);flag=0;k=0;}
							else {mode=0;k=0;Write(TempPassword,6*num,6);num+=1;Show_string2(right_a);delay_s(1);}
							}
						}
					flag=0;
					k=0;
				}
			}
		}
		
		
		//  删除密码
		if(mode==3){
			while(mode==3){
				// 模式选择页面
				Show_string1(mode_line);
				Show_string2(mode3);
				KeyDown(&t, &mode, &k, TempPassword, &flag);
				if(flag==3){
					flag=0;
					while(flag!=3){
						// 删除模式选择					
						Show_string1(mode3_0);
						if(mode==3){Show_string2(mode3_1);flag_chg=1;}
						if(mode==0){Show_string2(mode3_2);mode=2;flag_chg=2;}
						KeyDown(&t, &mode, &k, TempPassword, &flag);
						if(flag==2&k==0){flag=3;mode=3;k='a';}  // 返回						
						if(flag==3&flag_chg==1&k!='a'){	// 删除全部(不包括初始密码)
							if(num>1){
								num=1;
								flag=3;mode=0;
								Show_string2(delete_s);delay_s(1);
							}else{Show_string2(delete_s2);delay_s(1);flag=0;mode=3;}
						}
						if(flag==3&flag_chg==2&k!='a'){ // 删除1个(不包括初始密码)
							if(num>1){
								flag=0;
								Show_string1(old_p);
								Show_string2(start_line);
								while(flag!=3){							
									KeyDown(&t, &mode, &k, TempPassword, &flag);
									if(flag==2&k==0){flag=3;mode=1;k='a';}  // 返回
									InPut(&k, &flag, &start_line, TempPassword);
									if(flag==3&k<5&k!='a'){flag=0;k=0;Show_string1(try_again);Show_string2(try_again2);delay_s(1);Show_string1(old_p);Show_string2(start_line);}
									//  已保存密码浏览
									if(flag==3&k>=5&k!='a'){
										for(i=1;i<num;i++){
											Read(CurrentPassword,6*i,6);
											for(j=0;j<6;j++){
												if(CurrentPassword[j]==TempPassword[j]){flag_d=1;flag_c=i;}
												else{flag_d=0;break;}
											}
											if(flag_d==1)break;
										}
										if(flag_d==0){Show_string2(Er_try);delay_s(1);Show_string2(start_line);flag=0;k=0;}else {
											for(i=flag_c;i<num;i++){
												if(i<num-1){Read(CurrentPassword,6*(i+1),6);Write(CurrentPassword,6*i,6);}										
											}
											num-=1;
											flag=3;mode=0;Show_string2(delete_s);delay_s(1);
										}
									}
								}	
							}else{Show_string2(delete_s2);delay_s(1);flag=0;mode=0;}							
						}
					}
					flag_chg=0;
					flag=0;
					k=0;
				}				
			}
		}
	}
}


