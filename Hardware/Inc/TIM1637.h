#ifndef __TM1637_H
#define __TM1637_H	
#include "main.h"

#define TM_CLK_CLK_GPIO_Port GPIOF
#define TM_DIO_CLK_GPIO_Port GPIOF

#define TM_CLK_CLK_Pin		 GPIO_PIN_0
#define TM_DIO_CLK_Pin		 GPIO_PIN_1



#define      CLK_1         HAL_GPIO_WritePin(TM_CLK_CLK_GPIO_Port,TM_CLK_CLK_Pin,GPIO_PIN_SET)
#define      CLK_0         HAL_GPIO_WritePin(TM_CLK_CLK_GPIO_Port,TM_CLK_CLK_Pin,GPIO_PIN_RESET)
#define      DIO_1         HAL_GPIO_WritePin(TM_DIO_CLK_GPIO_Port,TM_DIO_CLK_Pin,GPIO_PIN_SET)
#define      DIO_0         HAL_GPIO_WritePin(TM_DIO_CLK_GPIO_Port,TM_DIO_CLK_Pin,GPIO_PIN_RESET)

void GPIO_Config(void);
void TM1637_WriteBit(unsigned char mBit);
void TM1637_WriteByte(unsigned char Byte);
void TM1637_start(void);
void TM1637_ack(void);
void delay_us(int i);
void TM1637_stop(void);
void TM1637_Write(unsigned char DATA);
void TM1637_display(uint8_t a,uint8_t b,uint8_t c,uint8_t d,uint8_t h);

 
#endif 
