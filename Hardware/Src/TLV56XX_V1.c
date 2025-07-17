
#include "TLV56XX_V1.h"

/****************************��������**********************************/
//1.���ƼĴ���
union REG_CFG			 						//DAC���ƼĴ���					
{												
	struct 										
	{											//Byte Access	
		unsigned char L;					
		unsigned char H;					
	}	BYTE;																	
	struct 										//Bit  Access
	{	
		unsigned char LSB  :8;					//D7 - D0
		unsigned char MSB  :4;					//D11- D8										
		unsigned char R0   :1;
		unsigned char POWER:1; 					//Power control bit
		unsigned char SPD  :1;					//Speed control bit
		unsigned char R1   :1;					
	}	BIT;
}CFG;

/**
	@note�� DAC�˿ڳ�ʼ��
***********************************************************************/
void TLV_DAC_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct={0};

	//1.ʱ������
	__HAL_RCC_GPIOE_CLK_ENABLE();	//ʹ��GPIOEʱ��
	
	// 2.����GPIO����
    GPIO_InitStruct.Pin = DAC_CS1_Pin | DAC_CS2_Pin | DAC_SCK_Pin | DAC_SDI_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;     // �������
    GPIO_InitStruct.Pull = GPIO_PULLUP;             // ����
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;   // ����
	 HAL_GPIO_Init(DAC_GPIO_Port, &GPIO_InitStruct);	
	
	// 3.���ó�ʼ��ƽ
    DAC_CS1_HIGH();
    DAC_CS2_HIGH();
    DAC_SCK_HIGH();
    DAC_SDI_HIGH();
}
/**********************************************************************
* ���� : DAC_Write_Byte(unsigned char val)
* ���� : д���ֽ����ݺ���
* ���� : val -- ���ֽ�����
* ��� ����
* ˵�� : ��
***********************************************************************/
void DAC_Write_Byte(unsigned char val)
{
	uint8_t i = 0;

	DAC_SDI_HIGH();  								//SDI = 1
	for(i = 0;i < 8;i ++)
	{						
		if((val & 0x80) == 0x80)				//���ֽ���ǰ
			DAC_SDI_HIGH();
		else 
			DAC_SDI_LOW();
	
		DAC_SCK_LOW();	
		for( int i = 0 ; i < 320 ; i++ ) ;			//SCK = 0	
		DAC_SCK_HIGH();
		for( int i = 0 ; i < 320 ; i++ ) ;				//SCK = 1
		val <<= 1;		
	}	
}
/**********************************************************************
* ���� : DAC_Output(unsigned char channel,float Votage)
* ���� : DAC ��ѹ�������
* ���� : channel -- ͨ����Votage -- ��ѹֵ
* ��� ����
* ˵�� : У��ϵ������У׼���������
         ���㷽���������趨���Ϊ1.25V,ʵ��ֻ��1.248V
		 ��ôDAC_Coeff =1.25 �� 1.248 = 1.001602f
		 ��ʹ��ʱֱ������Ϊ1���ɣ�������
***********************************************************************/
void TLV_DAC_Output(uint8_t channel,float Votage)
{
	 uint16_t REG_Value = 0;	
	CFG.BYTE.H = CFG.BYTE.L = 0x00;						//��λ�Ĵ�������		
	
	//1.���üĴ�������
 	if(channel == 0 || channel ==2)		//ͨ��1����OUTA
	{
		CFG.BIT.R1 = 1;
		CFG.BIT.R0 = 0;
	}
	else if(channel == 1 || channel ==3)	//ͨ��2����OUTB
	{
		CFG.BIT.R1 = 0;
		CFG.BIT.R0 = 0;	
	}
	CFG.BIT.SPD = Fast_Mode;							//ѡ�����ģʽ			
	CFG.BIT.POWER = n_Op;						

	//2.���ݵ�ѹ����DAC����
	if(Votage <= DAC_VREF * DAC_GAIN)
	{
		REG_Value = (unsigned int)((float)Votage * CODE_MAX * DAC_Coeff / (DAC_VREF * DAC_GAIN));		//���Գ���ʹ�õ�VREF = 2.5V,GAIN = 2
	}
	CFG.BIT.MSB = REG_Value >> 8;
	CFG.BIT.LSB = REG_Value & 0x00FF;					//����DAC����
	
	//3.���Ĵ�������д�뵽DAC���������
	if(channel == 0 || channel == 1)		//��оƬ1Ƭѡ
		DAC_CS1_LOW();
	else if(channel == 2 || channel == 3)	//��оƬ2Ƭѡ
		DAC_CS2_LOW();
	
	DAC_Write_Byte(CFG.BYTE.H);	 						//д��Ĵ�����8λ
	DAC_Write_Byte(CFG.BYTE.L);	   						//д��Ĵ�����8λ

	if(channel == 0 || channel == 1)		//�ر�оƬ1Ƭѡ
		DAC_CS1_HIGH();
	else if(channel == 2 || channel == 3)	//�ر�оƬ2Ƭѡ
		DAC_CS2_HIGH();				
}



