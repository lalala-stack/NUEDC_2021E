#ifndef __TM1637_H
#define __TM1637_H	
#include "main.h"
#define TM1637_RCC_Cmd    RCC_APB2PeriphClockCmd

#define TM_CLK_CLK_GPIO_Port GPIOF
#define TM_DIO_CLK_GPIO_Port GPIOF
#define TM_RCLK_GPIO_Port	 GPIOF
#define TM_CLK_CLK_Pin		 GPIO_PIN_0
#define TM_DIO_CLK_Pin		 GPIO_PIN_1
#define TM_RCLK_Pin		 	 GPIO_PIN_2

#define      SCLK_1         HAL_GPIO_WritePin(TM_CLK_CLK_GPIO_Port,TM_CLK_CLK_Pin,GPIO_PIN_SET)
#define      SCLK_0         HAL_GPIO_WritePin(TM_CLK_CLK_GPIO_Port,TM_CLK_CLK_Pin,GPIO_PIN_RESET)
#define      DIO_1         HAL_GPIO_WritePin(TM_DIO_CLK_GPIO_Port,TM_DIO_CLK_Pin,GPIO_PIN_SET)
#define      DIO_0         HAL_GPIO_WritePin(TM_DIO_CLK_GPIO_Port,TM_DIO_CLK_Pin,GPIO_PIN_RESET)
#define      RCLK_1         HAL_GPIO_WritePin(TM_RCLK_GPIO_Port,TM_RCLK_Pin,GPIO_PIN_SET)
#define      RCLK_0         HAL_GPIO_WritePin(TM_RCLK_GPIO_Port,TM_RCLK_Pin,GPIO_PIN_RESET)



void GPIO_Config(void);
void TM1637_WriteBit(unsigned char mBit);
void TM1637_WriteByte(unsigned char Byte);
void TM1637_start(void);
void TM1637_ack(void);
void delay_us(int i);
void TM1637_stop(void);
void TM1637_Write(unsigned char DATA);
void TM1637_display(uint8_t a,uint8_t b,uint8_t c,uint8_t d,uint8_t h);

void LED4_Display (void);			// LED显示
void LED_OUT(char X);				// LED单字节串行移位函数 

extern unsigned char LED[8];
 
#endif 
