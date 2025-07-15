#include "TIM1637.h"
 
 unsigned char fseg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
 unsigned char segbit[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
 unsigned char  disbuf[4]={0,0,0,0};


 //-----------------------------------------------------------------------------
// ����ԭ�ζ���
#define uchar unsigned char




unsigned char LED_0F[];		// LED��ģ��


//-----------------------------------------------------------------------------
// ȫ�ֱ���
unsigned char LED[8];	//����LED��8λ��ʾ����


//*****************************************************************************
// ������
//


void LED4_Display (void)
{
	unsigned char *led_table;          // ���ָ��
	unsigned char i;
	//��ʾ��1λ
	led_table = LED_0F + LED[0];
	i = *led_table;
	//i=LED_0F[LED[0]];
	LED_OUT(i);			
	LED_OUT(0x01);		

	RCLK_0;
	
	RCLK_1;
	//��ʾ��2λ
	led_table = LED_0F + LED[1];
	i = *led_table;

	LED_OUT(i);
	LED_OUT(0x02);		

	RCLK_0;
	
	RCLK_1;
	//��ʾ��3λ
	led_table = LED_0F + LED[2];
	i = *led_table;

	LED_OUT(i);			
	LED_OUT(0x04);	

	RCLK_0;
	
	RCLK_1;
	//��ʾ��4λ
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



