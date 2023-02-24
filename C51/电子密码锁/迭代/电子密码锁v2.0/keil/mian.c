#include <stdio.h>
#include <reg52.h>
#include <string.h>
#include <lcd.h> //  ����LCD1602��
#include <key.h> //  ������󰴼���
#include <save.h> //  ����24C04��

/*---ȫ�ֱ�������---*/
//  lcd
u8 code start_line[]	= {"password:       "};
u8 code name[] 	 	= {"===Coded Lock==="};	//��ʾ����
u8 code LockOpen[]	= {"      open      "};		//OPEN
u8 code ResetOK[] 	= {"ResetPasswordOK "};
u8 code initword[]	= {"Init password..."};
u8 code mode_line[] = {"==Mode Setting=="};
u8 code mode1[] = {"-Change Password"};
u8 code mode2[] = {"-Add New User   "};
u8 code Er_try[]		= {"error,try again!"};
u8 code try_again[] = {"Warn:A Lenth of "};
u8 code try_again2[] = {"password is six"};
u8 code again[]		= {"input again     "};
u8 code username[] = {"=Administrator  "};
u8 code old_p[] = {"==Old Password=="};
u8 code new_p[] = {"==New Password=="};
u8 code warn_a[] = {"-Password exists"};
u8 code right_a[] = {"Adding succeeded"};
u8 code Locked[] = {"====UI locked==="};

//  AT24C02A
u8 CurrentPassword[6]; 			//��ȡEEPROM�����ݴ�����
u8 TempPassword[6];
u8 InitPassword[6];
u8 num=0; //  ������������
u8 t=0; //  �Ƿ����ó�ʼ����
u8 flag_d=0; //  �ж������Ƿ���ȷ
u8 flag_c; //  �޸���������λ��
u8 n_alert = 0; // �������

//  ����
u8 flag=0; //  �жϰ����Ƿ���
u8 mode=0; //  ģʽ����
u8 k=0; //  ��������λ��



void main()
{
	u8 i,j; 
	LcdInit(); //  LCD1602��ʼ������ 
	Show_string1(initword);
	Show_string2(start_line);
	while(1){
		//  ��ʼ��������
		while(mode==0&t==0) {
			KeyDown(&t, &mode, &k, TempPassword, &flag);
			InPut(&k, &flag, start_line, TempPassword);
			if(flag==3&k<5){flag=0;k=0;Show_string1(try_again);Show_string2(try_again2);delay_s(1);Show_string1(initword);Show_string2(start_line);}
			if(flag==3&k>=5){t=1;k=0;num+=1;flag=0;memcpy(InitPassword,TempPassword,sizeof(InitPassword));mode=0;WriteI2C(TempPassword,0x0000,6);}			
		}
		
		//  ģʽ0����¼����
		if(mode==0) {		
			Show_string1(name);
			Show_string2(start_line);
			while(mode==0&t==1){	
				KeyDown(&t, &mode, &k, TempPassword, &flag);
				InPut(&k, &flag, start_line, TempPassword);
				if(flag==3&k<5){flag=0;k=0;Show_string1(try_again);Show_string2(try_again2);delay_s(1);Show_string1(name);Show_string2(start_line);}
				//  �ж���������
				if(flag==3&k>=5){
					for(i=0;i<num;i++){
						ReadI2C(CurrentPassword,0X0000+6*i,6);delay_ms(100);
						for(j=0;j<6;j++){
							if(CurrentPassword[j]==TempPassword[j]){flag_d=1;}
							else{flag_d=0;}
							show_number(0x49+j,CurrentPassword[j]);
						}
						if(flag_d==1)break;
					}
					delay_s(2);
					if(flag_d==0){
						ALERT=0;delay_ms(200);ALERT=1;Show_string2(Er_try);n_alert+=1;delay_s(1);Show_string2(start_line);
						//  ����
						while(n_alert==3){
							flag=0;
							k=0;
							Show_string1(Locked);
							KeyDown(&t, &mode, &k, TempPassword, &flag);
							InPut(&k, &flag, start_line, TempPassword);
							if(flag==3&k<5){flag=0;k=0;Show_string1(try_again);Show_string2(try_again2);delay_s(1);Show_string1(name);Show_string2(start_line);}
							while(flag==3&k>=5){
								if(flag==3&k>=5){
									ReadI2C(CurrentPassword,0,6);
									for(j=0;j<6;j++){
										if(CurrentPassword[j]==InitPassword[j]){flag_d=1;}
										else{flag_d=0;break;}
									}
									if(flag_d==1)break;
								}
								if(flag_d==0){Show_string2(Er_try);delay_s(1);Show_string2(start_line);flag=0;flag_c==0;}
								else n_alert=0;
							}
						}
					}
					else {Show_string2(LockOpen),LED=0;delay_s(1);Show_string2(start_line);LED=1;n_alert=0;}
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
				
				//  ҳ������
				if(flag==3){
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
							for(i=0;i<num;i++){
								ReadI2C(CurrentPassword,0+6*i,6);
								for(j=0;j<6;j++){
									if(CurrentPassword[j]==TempPassword[j]){flag_d=1;flag_c=0+6*i;}
									else{flag_d=0;break;}
								}
								if(flag_d==1)break;
							}
							if(flag_d==0){Show_string2(Er_try);delay_s(1);Show_string2(start_line);flag=0;k=0;}else {
								//  ������
								flag=0 ;
								flag_d=0;
								Show_string1(new_p);
								Show_string2(start_line);
								while(flag!=3){							
									KeyDown(&t, &mode, &k, TempPassword, &flag);
									if(flag==2&k==0){flag=3;mode=1;k='a';}  // ����
									InPut(&k, &flag, &start_line, TempPassword);
									if(flag==3&k<5&k!='a'){flag=0;k=0;Show_string1(try_again);Show_string2(try_again2);delay_s(1);Show_string1(new_p);Show_string2(start_line);}
									if(flag==3&k>=5&k!='a'){t=1;k=0;flag=0;flag=0;mode=0;WriteI2C(TempPassword,flag_c,6);Show_string2(ResetOK);}
								}
							}
						}
					}
					flag=0;
					k=0;
			  }
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
					show_number(0X0E,num);show_string(0X0F,'=');
					Show_string2(start_line);
					while(flag!=3){		
						KeyDown(&t, &mode, &k, TempPassword, &flag);
						if(flag==2&k==0){flag=3;mode=2;k='a';}  // ����
						InPut(&k, &flag, start_line, TempPassword);
						if(flag==3&k<5&k!='a'){flag=0;k=0;Show_string1(try_again);Show_string2(try_again2);delay_s(1);Show_string1(username);show_number(0X0E,num);show_string(0X0F,'=');Show_string2(start_line);} //  ����λ������
						if(flag==3&k>=5&k!='a'){
							for(i=0;i<num;i++){
								ReadI2C(CurrentPassword,0+6*i,6);
								for(j=0;j<6;j++){
									if(CurrentPassword[j]==TempPassword[j]){flag_d=1;}
									else{flag_d=0;break;}
								}
								if(flag_d==1)break;
							}
							if(flag_d==1){Show_string2(warn_a);delay_s(1);Show_string2(start_line);}
							else {mode=0;k=0;num+=1;WriteI2C(TempPassword,0X00+6*num,6);Show_string2(right_a);delay_s(1);}
							}
						}
					flag=0;
					k=0;
				}				
			}
		}
	}
}


