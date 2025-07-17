#ifndef  __TLV56XX_V1_H__
#define  __TLV56XX_V1_H__

#include "main.h"

/*****************************�˿ڶ���*********************************/
#define DAC_GPIO_Port   GPIOE

// GPIO���Ŷ���
#define DAC_CS1_Pin     GPIO_PIN_2      // PE0 -- CS1
#define DAC_CS2_Pin     GPIO_PIN_3      // PE1 -- CS2
#define DAC_SCK_Pin     GPIO_PIN_5      // PE2 -- SCK
#define DAC_SDI_Pin     GPIO_PIN_6      // PE3 -- SDI

// GPIO���ƺ궨�壨HAL�ⷽʽ��
#define DAC_CS1_HIGH()  HAL_GPIO_WritePin(DAC_GPIO_Port, DAC_CS1_Pin, GPIO_PIN_SET)
#define DAC_CS1_LOW()   HAL_GPIO_WritePin(DAC_GPIO_Port, DAC_CS1_Pin, GPIO_PIN_RESET)
#define DAC_CS2_HIGH()  HAL_GPIO_WritePin(DAC_GPIO_Port, DAC_CS2_Pin, GPIO_PIN_SET)
#define DAC_CS2_LOW()   HAL_GPIO_WritePin(DAC_GPIO_Port, DAC_CS2_Pin, GPIO_PIN_RESET)
#define DAC_SCK_HIGH()  HAL_GPIO_WritePin(DAC_GPIO_Port, DAC_SCK_Pin, GPIO_PIN_SET)
#define DAC_SCK_LOW()   HAL_GPIO_WritePin(DAC_GPIO_Port, DAC_SCK_Pin, GPIO_PIN_RESET)
#define DAC_SDI_HIGH()  HAL_GPIO_WritePin(DAC_GPIO_Port, DAC_SDI_Pin, GPIO_PIN_SET)
#define DAC_SDI_LOW()   HAL_GPIO_WritePin(DAC_GPIO_Port, DAC_SDI_Pin, GPIO_PIN_RESET)

/*****************************��������*********************************/
// 1.��׼��ѹ
#define DAC_VREF        2.5f                // 2.5V ref voltage
#define DAC_GAIN        2                   // 2 gain

// 2.�������
#define CODE_MAX        0x0FFF              // 12λDAC���ֵ
#define DAC_Coeff       1.0f                // У��ϵ��

// 3.DACͨ������
typedef enum
{
    DAC_CH1 = 0,
    DAC_CH2,
    DAC_CH3,
    DAC_CH4,
} nCH;

// 4.DAC���Ʋ���
#define Fast_Mode       1                   // ����ģʽ
#define Slow_Mode       0                   // ����ģʽ
#define p_Down          1                   // ����ģʽ
#define n_Op            0                   // ����ģʽ

/*****************************�ӿں���*********************************/
void TLV_DAC_Init(void);                               // ��ʼ��
void TLV_DAC_Output(uint8_t channel, float voltage);   // DAC�������
void DAC_Write_Byte(uint8_t val);                              // д���ֽ�����

#endif