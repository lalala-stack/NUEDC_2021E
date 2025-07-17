
#include "TLV56XX_V1.h"

/****************************参数定义**********************************/
//1.控制寄存器
union REG_CFG			 						//DAC控制寄存器					
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
	@note： DAC端口初始化
***********************************************************************/
void TLV_DAC_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct={0};

	//1.时钟配置
	__HAL_RCC_GPIOE_CLK_ENABLE();	//使能GPIOE时钟
	
	// 2.配置GPIO引脚
    GPIO_InitStruct.Pin = DAC_CS1_Pin | DAC_CS2_Pin | DAC_SCK_Pin | DAC_SDI_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;     // 推挽输出
    GPIO_InitStruct.Pull = GPIO_PULLUP;             // 上拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;   // 高速
	 HAL_GPIO_Init(DAC_GPIO_Port, &GPIO_InitStruct);	
	
	// 3.设置初始电平
    DAC_CS1_HIGH();
    DAC_CS2_HIGH();
    DAC_SCK_HIGH();
    DAC_SDI_HIGH();
}
/**********************************************************************
* 名称 : DAC_Write_Byte(unsigned char val)
* 功能 : 写单字节数据函数
* 输入 : val -- 单字节数据
* 输出 ：无
* 说明 : 无
***********************************************************************/
void DAC_Write_Byte(unsigned char val)
{
	uint8_t i = 0;

	DAC_SDI_HIGH();  								//SDI = 1
	for(i = 0;i < 8;i ++)
	{						
		if((val & 0x80) == 0x80)				//高字节在前
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
* 名称 : DAC_Output(unsigned char channel,float Votage)
* 功能 : DAC 电压输出函数
* 输入 : channel -- 通道，Votage -- 电压值
* 输出 ：无
* 说明 : 校正系数用于校准输出！！！
         计算方法：假设设定输出为1.25V,实际只有1.248V
		 那么DAC_Coeff =1.25 ÷ 1.248 = 1.001602f
		 不使用时直接设置为1即可！！！！
***********************************************************************/
void TLV_DAC_Output(uint8_t channel,float Votage)
{
	 uint16_t REG_Value = 0;	
	CFG.BYTE.H = CFG.BYTE.L = 0x00;						//复位寄存器数据		
	
	//1.设置寄存器数据
 	if(channel == 0 || channel ==2)		//通道1，即OUTA
	{
		CFG.BIT.R1 = 1;
		CFG.BIT.R0 = 0;
	}
	else if(channel == 1 || channel ==3)	//通道2，即OUTB
	{
		CFG.BIT.R1 = 0;
		CFG.BIT.R0 = 0;	
	}
	CFG.BIT.SPD = Fast_Mode;							//选择快速模式			
	CFG.BIT.POWER = n_Op;						

	//2.根据电压计算DAC内码
	if(Votage <= DAC_VREF * DAC_GAIN)
	{
		REG_Value = (unsigned int)((float)Votage * CODE_MAX * DAC_Coeff / (DAC_VREF * DAC_GAIN));		//测试程序使用的VREF = 2.5V,GAIN = 2
	}
	CFG.BIT.MSB = REG_Value >> 8;
	CFG.BIT.LSB = REG_Value & 0x00FF;					//设置DAC内码
	
	//3.将寄存器数据写入到DAC，更新输出
	if(channel == 0 || channel == 1)		//打开芯片1片选
		DAC_CS1_LOW();
	else if(channel == 2 || channel == 3)	//打开芯片2片选
		DAC_CS2_LOW();
	
	DAC_Write_Byte(CFG.BYTE.H);	 						//写入寄存器高8位
	DAC_Write_Byte(CFG.BYTE.L);	   						//写入寄存器低8位

	if(channel == 0 || channel == 1)		//关闭芯片1片选
		DAC_CS1_HIGH();
	else if(channel == 2 || channel == 3)	//关闭芯片2片选
		DAC_CS2_HIGH();				
}



