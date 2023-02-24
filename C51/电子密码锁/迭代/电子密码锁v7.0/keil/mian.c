/**************************************************************************************
*		              ����������												  *
Ӳ�����ӣ�
           P1 --> �������
					 P34--> EEPROMģ��SCL
					 P35--> EEPROMģ��SDA
					 P21--> ������
					 P36--> LED_GREEN
					 
					 ������̣�
					 7      8      9       $
					 
					 4      5      6       $
					 
					 1      2      3       $
					 
					ȷ��    0  ���/����  ģʽ
					
������ã�
					main.c --> ������
					key.c  --> ��������ģ��
					lcd.c  --> LCD702ģ��
					save.c --> EEPROMģ�飨AT24C04A��

ģʽ���ã�
					ģʽ0����¼����
					ģʽ1���޸��������
						- �޸ĳ�ʼ����
						- �޸��û��������
					ģʽ2��������û��������
					ģʽ3��ɾ���û��������
					  - ɾ�������û��������
						- ɾ��һλ�û��������
ע�����
        ������ֻ������Ϊ6λ
				�����û����ֻ�����32λ
				���������3���Զ�������ֻ��ͨ����ʼ�������
				
�������⣺
        �����������ȷ������һ���ȴ�ʱ�䣨�д��Ľ���
        ��ת��ģʽ����һ�����ӳ٣����٣��д��Ľ���
***************************************************************************************/
#include <stdio.h>
#include <reg52.h>
#include <string.h>
#include <lcd.h> //  ����LCD702��
#include <key.h> //  ������󰴼���
#include <save.h> //  ����24C04��

/*---ȫ�ֱ�������---*/
//  lcd
u8 code start_line[]	= {"password:       "}; 
u8 code name[] 	 	= {"Coded Lock( /3) "};	//��ʾ����
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
u8 CurrentPassword[6]; 			//��ȡEEPROM�����ݴ�����
u8 TempPassword[6];
u8 InitPassword[6]={0,0,0,0,0,0}; //  ��ʼ����
u8 num=1; //  ������������
u8 t=0; //  �Ƿ����ó�ʼ����
u8 flag_d=0; //  �ж������Ƿ���ȷ
u8 flag_c; //  �޸���������λ��
u8 flag_chg=0; // ɾ��ѡ��
//u8 temp=1; // �޸��������ƣ�ֻ���޸�����������1�Σ�
u8 n_alert = 0; // �������

//  ����
u8 flag=0; //  �жϰ����Ƿ���
u8 mode=0; //  ģʽ����
u8 k=0; //  ��������λ��



void main()
{
	u8 i,j; 
	LcdInit(); //  LCD702��ʼ������ 
//	Show_string1(initword);
//	Show_string2(start_line);
	while(1){
		//  ��ʼ��������
/*		while(mode==0&t==0) {
			KeyDown(&t, &mode, &k, TempPassword, &flag);
			InPut(&k, &flag, start_line, TempPassword);
			if(flag==3&k<5){flag=0;k=0;Show_string1(try_again);Show_string2(try_again2);delay_s(1);Show_string1(initword);Show_string2(start_line);}
			if(flag==3&k>=5){t=1;k=0;num+=1;flag=0;memcpy(InitPassword,TempPassword,sizeof(InitPassword));mode=0;Write(TempPassword,0,6);}			
		}*/
		// ��ʼ��������
		while(mode==0&t==0){Write(InitPassword,0,6);t=1;} 
		
		//  ģʽ0����¼����
		if(mode==0) {		
			Show_string1(name);
			show_number(0x0b,3-n_alert);
			Show_string2(start_line);
			while(mode==0&t==1){	
				KeyDown(&t, &mode, &k, TempPassword, &flag);
				InPut(&k, &flag, start_line, TempPassword);
				if(flag==3&k<5){flag=0;k=0;Show_string1(try_again);Show_string2(try_again2);delay_s(1);Show_string1(name);show_number(0x0b,3-n_alert);Show_string2(start_line);}
				//  �ж���������
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
						//  ����
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
		
		
		
    //  ģʽ1���޸�����
		if(mode==1){ 
			while(mode==1){
				//  ģʽѡ��ҳ��
				Show_string1(mode_line);
				Show_string2(mode1);
				KeyDown(&t, &mode, &k, TempPassword, &flag);
				if(flag==3){
					flag=0;
					mode=3;
					while(flag!=3){
						// �޸�ģʽѡ��					
						Show_string1(mode2_0);
						if(mode==3){Show_string2(mode2_1);flag_chg=1;}
						if(mode==0){Show_string2(mode2_2);mode=2;flag_chg=2;}
						KeyDown(&t, &mode, &k, TempPassword, &flag);
						if(flag==2&k==0){flag=3;mode=1;k='a';}  // ����
						//  ��ʼ�����޸�
						if(flag==3&flag_chg==1&k!='a'){
							//  ������ 
							flag=0;
							Show_string1(old_p);
							Show_string2(start_line);
							while(flag!=3){							
								KeyDown(&t, &mode, &k, TempPassword, &flag);
								if(flag==2&k==0){flag=3;mode=1;k='a';}  // ����
								InPut(&k, &flag, &start_line, TempPassword);
								if(flag==3&k<5&k!='a'){flag=0;k=0;Show_string1(try_again);Show_string2(try_again2);delay_s(1);Show_string1(old_p);Show_string2(start_line);}
								//  ��ʼ�������
								if(flag==3&k>=5&k!='a'){
									Read(CurrentPassword,0,6);
									for(j=0;j<6;j++){
										if(CurrentPassword[j]==TempPassword[j]){flag_d=1;}
										else{flag_d=0;break;}
									}
									if(flag_d==0){Show_string2(Er_try);delay_s(1);Show_string2(start_line);flag=0;k=0;}else {
										//  ������
										flag=0 ;
										flag_d=0;
										k=0;
										Show_string1(new_p);
										Show_string2(start_line);
										while(flag!=3){							
											KeyDown(&t, &mode, &k, InitPassword, &flag);
											if(flag==2&k==0){flag=3;mode=1;k='a';}  // ����
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
						//  �û����������޸�
						if(flag==3&flag_chg==2&k!='a'){
							if(num>1){
								//  ������ 
								flag=0;
								Show_string1(old_p);
								Show_string2(start_line);
								while(flag!=3){							
									KeyDown(&t, &mode, &k, TempPassword, &flag);
									if(flag==2&k==0){flag=3;mode=1;k='a';}  // ����
									InPut(&k, &flag, &start_line, TempPassword);
									if(flag==3&k<5&k!='a'){flag=0;k=0;Show_string1(try_again);Show_string2(try_again2);delay_s(1);Show_string1(old_p);Show_string2(start_line);}
									//  �ѱ����������
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
											//  ������
											flag=0 ;
											flag_d=0;
											k=0;
											Show_string1(new_p);
											Show_string2(start_line);
											while(flag!=3){							
												KeyDown(&t, &mode, &k, TempPassword, &flag);
												if(flag==2&k==0){flag=3;mode=1;k='a';}  // ����
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
		
		
    
		//  ģʽ2��������û�
		if(mode==2){ 
			while(mode==2){
				// ģʽѡ��ҳ��
				Show_string1(mode_line);
				Show_string2(mode2);
				KeyDown(&t, &mode, &k, TempPassword, &flag);
				
				// ҳ������
				if(flag==3){
					flag=0;
					Show_string1(username);
					show_number(0X0E,num);show_string(0X0F,"=");
					Show_string2(start_line);
					while(flag!=3){		
						KeyDown(&t, &mode, &k, TempPassword, &flag);
						if(flag==2&k==0){flag=3;mode=2;k='a';}  // ����
						InPut(&k, &flag, start_line, TempPassword);
						if(flag==3&k<5&k!='a'){flag=0;k=0;Show_string1(try_again);Show_string2(try_again2);delay_s(1);Show_string1(username);show_number(0X0E,num);show_string(0X0F,"=");Show_string2(start_line);} //  ����λ������
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
		
		
		//  ɾ������
		if(mode==3){
			while(mode==3){
				// ģʽѡ��ҳ��
				Show_string1(mode_line);
				Show_string2(mode3);
				KeyDown(&t, &mode, &k, TempPassword, &flag);
				if(flag==3){
					flag=0;
					while(flag!=3){
						// ɾ��ģʽѡ��					
						Show_string1(mode3_0);
						if(mode==3){Show_string2(mode3_1);flag_chg=1;}
						if(mode==0){Show_string2(mode3_2);mode=2;flag_chg=2;}
						KeyDown(&t, &mode, &k, TempPassword, &flag);
						if(flag==2&k==0){flag=3;mode=3;k='a';}  // ����						
						if(flag==3&flag_chg==1&k!='a'){	// ɾ��ȫ��(��������ʼ����)
							if(num>1){
								num=1;
								flag=3;mode=0;
								Show_string2(delete_s);delay_s(1);
							}else{Show_string2(delete_s2);delay_s(1);flag=0;mode=3;}
						}
						if(flag==3&flag_chg==2&k!='a'){ // ɾ��1��(��������ʼ����)
							if(num>1){
								flag=0;
								Show_string1(old_p);
								Show_string2(start_line);
								while(flag!=3){							
									KeyDown(&t, &mode, &k, TempPassword, &flag);
									if(flag==2&k==0){flag=3;mode=1;k='a';}  // ����
									InPut(&k, &flag, &start_line, TempPassword);
									if(flag==3&k<5&k!='a'){flag=0;k=0;Show_string1(try_again);Show_string2(try_again2);delay_s(1);Show_string1(old_p);Show_string2(start_line);}
									//  �ѱ����������
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


