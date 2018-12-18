/*
	STM32F103自带I2C通信,但是不知如何与该OLED通信
	2017年12月2日15:51:00
*/

#include "stdarg.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "oled12864.h"
#include "delay.h"

#define USING_REGISTER 1

#define B8S ((((((((
#define  X +0)*2+1
#define  _ +0)*2+0
#define B8P

void OLED_IIC_Start(OLED_IIC_TypeDef*OLED_IIC)
{
	if(!OLED_IIC)return;
#if (USING_REGISTER==1)
	OLED_IIC->SCL_GPIO->BSRR|=OLED_IIC->SCL_Pin;//BUG:不能直接赋值[吐血]
	OLED_IIC->SDA_GPIO->BSRR|=OLED_IIC->SDA_Pin;
	OLED_IIC->SDA_GPIO->BRR|=OLED_IIC->SDA_Pin;
	OLED_IIC->SCL_GPIO->BRR|=OLED_IIC->SCL_Pin;
#else
	GPIO_SetBits	(OLED_IIC->SCL_GPIO,OLED_IIC->SCL_Pin);
	GPIO_SetBits	(OLED_IIC->SDA_GPIO,OLED_IIC->SDA_Pin);
	GPIO_ResetBits	(OLED_IIC->SDA_GPIO,OLED_IIC->SDA_Pin);
	GPIO_ResetBits	(OLED_IIC->SCL_GPIO,OLED_IIC->SCL_Pin);
#endif
}

void OLED_IIC_Stop(OLED_IIC_TypeDef*OLED_IIC)
{
	if(!OLED_IIC)return;
#if (USING_REGISTER==1)
	OLED_IIC->SCL_GPIO->BSRR|=OLED_IIC->SCL_Pin;
//	OLED_IIC->SCL_GPIO->BRR|=OLED_IIC->SCL_Pin;
	OLED_IIC->SDA_GPIO->BRR|=OLED_IIC->SDA_Pin;
	OLED_IIC->SDA_GPIO->BSRR|=OLED_IIC->SDA_Pin;	
#else
	GPIO_SetBits	(OLED_IIC->SCL_GPIO,OLED_IIC->SCL_Pin);
//	GPIO_ResetBits	(OLED_IIC->SCL_GPIO,OLED_IIC->SCL_Pin);
	GPIO_ResetBits	(OLED_IIC->SDA_GPIO,OLED_IIC->SDA_Pin);
	GPIO_SetBits	(OLED_IIC->SDA_GPIO,OLED_IIC->SDA_Pin);
#endif
}

void OLED_IIC_Byte(OLED_IIC_TypeDef*OLED_IIC,uint8_t byte)
{
	int i;
	if(!OLED_IIC)return;
#if (USING_REGISTER==1)
	OLED_IIC->SCL_GPIO->BRR|=OLED_IIC->SCL_Pin;
	for(i=0;i<8;i++)
	{
		if(byte>>7)
		{
			OLED_IIC->SDA_GPIO->BSRR|=OLED_IIC->SDA_Pin;
		}
		else
		{
			OLED_IIC->SDA_GPIO->BRR|=OLED_IIC->SDA_Pin;
		}
		OLED_IIC->SCL_GPIO->BSRR|=OLED_IIC->SCL_Pin;
		OLED_IIC->SCL_GPIO->BRR|=OLED_IIC->SCL_Pin;
		byte<<=1;//bug
	}
	OLED_IIC->SCL_GPIO->BSRR|=OLED_IIC->SCL_Pin;
	OLED_IIC->SCL_GPIO->BRR|=OLED_IIC->SCL_Pin;
#else
	GPIO_ResetBits(OLED_IIC->SCL_GPIO,OLED_IIC->SCL_Pin);
	for(i=0;i<8;i++)
	{
		if(byte>>7)
		{
			GPIO_SetBits(OLED_IIC->SDA_GPIO,OLED_IIC->SDA_Pin);
		}
		else
		{
			GPIO_ResetBits(OLED_IIC->SDA_GPIO,OLED_IIC->SDA_Pin);
		}
		byte<<=1;
		GPIO_SetBits(OLED_IIC->SCL_GPIO,OLED_IIC->SCL_Pin);
		GPIO_ResetBits(OLED_IIC->SCL_GPIO,OLED_IIC->SCL_Pin);
	}
	GPIO_SetBits(OLED_IIC->SCL_GPIO,OLED_IIC->SCL_Pin);
	GPIO_ResetBits(OLED_IIC->SCL_GPIO,OLED_IIC->SCL_Pin);
#endif
}

void OLED_IIC_Cmd(OLED_IIC_TypeDef*OLED_IIC,uint8_t cmd)
{
	if(!OLED_IIC)return;
	OLED_IIC_Start	(OLED_IIC);
	OLED_IIC_Byte	(OLED_IIC,0x78);
	OLED_IIC_Byte	(OLED_IIC,0x00);
	OLED_IIC_Byte	(OLED_IIC,cmd);
	OLED_IIC_Stop	(OLED_IIC);
}

void OLED_IIC_Dat(OLED_IIC_TypeDef*OLED_IIC,uint8_t dat)
{
	if(!OLED_IIC)return;
	OLED_IIC_Start	(OLED_IIC);
	OLED_IIC_Byte	(OLED_IIC,0x78);
	OLED_IIC_Byte	(OLED_IIC,0x40);
	OLED_IIC_Byte	(OLED_IIC,dat);
	OLED_IIC_Stop	(OLED_IIC);
}

void OLED_IIC_On(OLED_IIC_TypeDef*OLED_IIC)
{
	if(!OLED_IIC)return;
	OLED_IIC_Cmd(OLED_IIC,0x8D);
	OLED_IIC_Cmd(OLED_IIC,0x14);
	OLED_IIC_Cmd(OLED_IIC,0xAF);
}

void OLED_IIC_Off(OLED_IIC_TypeDef*OLED_IIC)
{
	if(!OLED_IIC)return;
	OLED_IIC_Cmd(OLED_IIC,0x8D);
	OLED_IIC_Cmd(OLED_IIC,0x10);
	OLED_IIC_Cmd(OLED_IIC,0xAE);
}

#define GPIO2RCCAPB2Periph_GPIO(_GPIO)\
(\
	((_GPIO)==GPIOA?RCC_APB2Periph_GPIOA:0)|\
	((_GPIO)==GPIOB?RCC_APB2Periph_GPIOB:0)|\
	((_GPIO)==GPIOC?RCC_APB2Periph_GPIOC:0)|\
	((_GPIO)==GPIOD?RCC_APB2Periph_GPIOD:0)|\
	((_GPIO)==GPIOE?RCC_APB2Periph_GPIOE:0)|\
	((_GPIO)==GPIOF?RCC_APB2Periph_GPIOF:0)|\
	((_GPIO)==GPIOG?RCC_APB2Periph_GPIOG:0)\
)

/*
static uint32_t GPIO2RCCAPB2Periph_GPIO(GPIO_TypeDef*GPIO)
{
	switch((uint32_t)GPIO)
	{
		case (uint32_t)GPIOA:
			return RCC_APB2Periph_GPIOA;
		case (uint32_t)GPIOB:
			return RCC_APB2Periph_GPIOB;
		case (uint32_t)GPIOC:
			return RCC_APB2Periph_GPIOC;
		case (uint32_t)GPIOD:
			return RCC_APB2Periph_GPIOD;
		case (uint32_t)GPIOE:
			return RCC_APB2Periph_GPIOE;
		case (uint32_t)GPIOF:
			return RCC_APB2Periph_GPIOF;
		case (uint32_t)GPIOG:
			return RCC_APB2Periph_GPIOG;
		default:
			return 0;
	}
}*/

void OLED_IIC_Init(OLED_IIC_TypeDef*OLED)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	if(!OLED)return;
	RCC_APB2PeriphClockCmd(GPIO2RCCAPB2Periph_GPIO(OLED->SCL_GPIO),ENABLE);
	RCC_APB2PeriphClockCmd(GPIO2RCCAPB2Periph_GPIO(OLED->SDA_GPIO),ENABLE);

	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=OLED->SCL_Pin;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(OLED->SCL_GPIO,&GPIO_InitStruct);
	GPIO_SetBits(OLED->SCL_GPIO,OLED->SCL_Pin);

	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=OLED->SDA_Pin;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(OLED->SDA_GPIO,&GPIO_InitStruct);
	GPIO_SetBits(OLED->SDA_GPIO,OLED->SDA_Pin);
	
	delay_ms(200);
	
	OLED_IIC_Cmd(OLED,0x2E);//关闭滚动
	
	OLED_IIC_Cmd(OLED,0xAE);//关显示
	OLED_IIC_Cmd(OLED,0x00);//设列地址低4位为0x0
	OLED_IIC_Cmd(OLED,0x10);//设列地址高4位为0x0
	OLED_IIC_Cmd(OLED,0x40);//设开始显示的行
	OLED_IIC_Cmd(OLED,0xB0);//设当前页
	
	OLED_IIC_Cmd(OLED,0x81);//准备设置对比度,下接1条指令
	OLED_IIC_Cmd(OLED,0xFF);//设置对比度
	
	OLED_IIC_Cmd(OLED,0xA1);//重映射列地址
	OLED_IIC_Cmd(OLED,0xA6);//反色
	
	OLED_IIC_Cmd(OLED,0xA8);//准备设置垂直屏蔽,下接1条指令
	OLED_IIC_Cmd(OLED,0x3F);//疲敝后剩下像素点
	
	OLED_IIC_Cmd(OLED,0xC8);//垂直翻转
	
	OLED_IIC_Cmd(OLED,0xD3);//准备设置向上偏移,下接1条指令
	OLED_IIC_Cmd(OLED,0x00);//向上偏移像素点
	
	OLED_IIC_Cmd(OLED,0xD5);//准备设置刷新率和振荡器频率,下接1条指令
	OLED_IIC_Cmd(OLED,0xF0);//高4位为振荡器频率 低4位为分频值
	
	OLED_IIC_Cmd(OLED,0xD8);//set area color mode off
	OLED_IIC_Cmd(OLED,0x05);//
	
	OLED_IIC_Cmd(OLED,0xD9);//准备设置预充电,下接1条指令
	OLED_IIC_Cmd(OLED,0xF1);
	
	OLED_IIC_Cmd(OLED,0xDA);//准备设置端口奇奇怪怪的重映射,下接1条指令
	OLED_IIC_Cmd(OLED,0x12);//
	
	OLED_IIC_Cmd(OLED,0xDB);
	OLED_IIC_Cmd(OLED,0x30);
	
	OLED_IIC_Cmd(OLED,0x8D);//set charge pump disable ??? SSD1306数据手册没这一条指令
	OLED_IIC_Cmd(OLED,0x14);
	
	OLED_IIC_Cmd(OLED,0xAF);//开显示
}
