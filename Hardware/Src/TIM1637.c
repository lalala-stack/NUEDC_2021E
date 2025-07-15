#include "TIM1637.h"
 
 unsigned char fseg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
 unsigned char segbit[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
 unsigned char  disbuf[4]={0,0,0,0};


 //-----------------------------------------------------------------------------
// 函数原形定义
#define uchar unsigned char




unsigned char LED_0F[];		// LED字模表


//-----------------------------------------------------------------------------
// 全局变量
unsigned char LED[8];	//用于LED的8位显示缓存


//*****************************************************************************
// 主程序
//


void LED4_Display (void)
{
	unsigned char *led_table;          // 查表指针
	unsigned char i;
	//显示第1位
	led_table = LED_0F + LED[0];
	i = *led_table;
	//i=LED_0F[LED[0]];
	LED_OUT(i);			
	LED_OUT(0x01);		

	RCLK_0;
	
	RCLK_1;
	//显示第2位
	led_table = LED_0F + LED[1];
	i = *led_table;

	LED_OUT(i);
	LED_OUT(0x02);		

	RCLK_0;
	
	RCLK_1;
	//显示第3位
	led_table = LED_0F + LED[2];
	i = *led_table;

	LED_OUT(i);			
	LED_OUT(0x04);	

	RCLK_0;
	
	RCLK_1;
	//显示第4位
	led_table = LED_0F + LED[3];
	i = *led_table;

	LED_OUT(i);			
	LED_OUT(0x08);		

	RCLK_0;
	
	RCLK_1;
}

void LED_OUT(char X)
{
	char i;
	for(i=8;i>=1;i--)
	{
		if (X&0x80) DIO_1; else DIO_0;
		X<<=1;
		SCLK_0;
		SCLK_1;
	}
}

unsigned char LED_0F[] = 
{// 0	 1	  2	   3	4	 5	  6	   7	8	 9	  A	   b	C    d	  E    F    -
	0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x8C,0xBF,0xC6,0xA1,0x86,0xFF,0xbf
};



