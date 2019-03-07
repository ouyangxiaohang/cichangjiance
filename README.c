# cichangjiance
course_2
#include<reg51.h>
#include "LCD1602.h"
#include "adc0809.c"

#define uchar unsigned char
#define uint  unsigned int
#define addo (5.0/255.0)
#define zero 0.019	 //用于校准AD的误差

sbit KEY1 = P1^3;//按键1
sbit KEY2 = P1^4;//按键2

unsigned int key_1=0,key_2=0;  //记录按键按下次数用于功能设置
unsigned char  Data[5],Data2[5];

//延时函数
void Delay_ms(uint jj)
{
	uint ii;
	while(jj--)
		for(ii=0;ii<125;ii++);
}
    
void key()		//判断按键是否按下
{
 	if(!KEY1)	
		{
			Delay_ms(100);
			if(!KEY1) 
			{
			key_1++;  //记录按键被按下次数
			}
		}
	if(!KEY2)
		{
			Delay_ms(100);
			if(!KEY2) 
			{
			key_2++;  //记录按键被按下次数
			}
		 }
}

void decodenum(float adv)	  //获得AD静态数值,unsigned char chn
{
	unsigned int temp; 
	temp = (unsigned int)(adv*1000);
	Data[0]='0'+temp/1000;     
    Data[1]='.';
	Data[2]='0'+(temp/100)%10; 
	Data[3]='0'+(temp/10)%10;  
	Data[4]='0'+temp%10;  
}

void main()
{
	LCD1602_init();//液晶初始化
	LCD1602_Disp_ZF(0x80,"    Welcome!   ",16);	  //欢迎界面
	LCD1602_Disp_ZF(0x80+0x40,"ci chang jian ce",16);
	Delay_ms(800);
	while(1)
	{	key();
		if (key_1%2==0)
			{
			if (key_2%3==0) 
				{
				if(ad_action(6)*addo-zero<0)  	
				{
				LCD1602_Disp_ZF(0x80,"    Set Zero    ",16);	  //静态显示
				LCD1602_Disp_ZF(0x80+0x40,"    Error!      ",16);	  //静态显示
				}
				else
			 	{
					decodenum((ad_action(6)*addo-zero)*0.32); //显示成磁感应强度
			 	//charact1="M Density(B):   ";
			 	//charact2="          "+Data+"T";
					LCD1602_Disp_ZF(0x80,"M Density(B):  ",16);	  //静态显示
					LCD1602_Disp_ZF(0x80+0x40,"               T",16);
					LCD1602_Disp_ZF(0x80+0x48,Data,5);
					}	   
				}
			else if(key_2%3==1)
				{
			 	decodenum((ad_action(6)*addo-zero)*0.32/12.566);//真空磁场强度
			 	//charact1="M Strength(H):  ";
			 	//charact2="       "+Data+"A/m";
				LCD1602_Disp_ZF(0x80,"M Strength(H):  ",16);	  //静态显示
				LCD1602_Disp_ZF(0x80+0x40,"       x10^-7A/m",16);
				LCD1602_Disp_ZF(0x80+0x42,Data,5);
				}
			else if(key_2%3==2)
				{
				decodenum(ad_action(7)*addo-zero);  //选择信号输入接口 最后一位显示接口号 
				//charact1="Now Voltage:    ";
				//charact2="         "+Data+"V";
				LCD1602_Disp_ZF(0x80,"Now Voltage:    ",16);	  //静态显示
				LCD1602_Disp_ZF(0x80+0x40,"               V",16);
				LCD1602_Disp_ZF(0x80+0x48,Data,5);
				}
			}
		else
			{
			decodenum(ad_action(1)*addo);  //判断调零电路是否工作			
			//charact1="Zero Voltage:   ";
			//	charact2="         "+Data+"V";
			LCD1602_Disp_ZF(0x80,"Zero Voltage:   ",16);	  //静态显示
			LCD1602_Disp_ZF(0x80+0x40,"               V",16);
			LCD1602_Disp_ZF(0x80+0x48,Data,5);
			}		 

	
	}

}
