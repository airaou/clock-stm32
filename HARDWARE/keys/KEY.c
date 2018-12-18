#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "key.h"
#include "delay.h"
#include "usart.h"

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

void Keys_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	RCC_APB2PeriphClockCmd(GPIO2RCCAPB2Periph_GPIO(KEYS_GET1_GPIO),ENABLE);
	RCC_APB2PeriphClockCmd(GPIO2RCCAPB2Periph_GPIO(KEYS_GET2_GPIO),ENABLE);
	RCC_APB2PeriphClockCmd(GPIO2RCCAPB2Periph_GPIO(KEYS_GET3_GPIO),ENABLE);
	RCC_APB2PeriphClockCmd(GPIO2RCCAPB2Periph_GPIO(KEYS_SET1_GPIO),ENABLE);
	RCC_APB2PeriphClockCmd(GPIO2RCCAPB2Periph_GPIO(KEYS_SET2_GPIO),ENABLE);
	RCC_APB2PeriphClockCmd(GPIO2RCCAPB2Periph_GPIO(KEYS_SET3_GPIO),ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_InitStruct.GPIO_Pin=KEYS_SET1_Pin;
	GPIO_Init(KEYS_SET1_GPIO,&GPIO_InitStruct);
	GPIO_ResetBits(KEYS_SET1_GPIO,KEYS_SET1_Pin);
	GPIO_InitStruct.GPIO_Pin=KEYS_SET2_Pin;
	GPIO_Init(KEYS_SET1_GPIO,&GPIO_InitStruct);
	GPIO_ResetBits(KEYS_SET2_GPIO,KEYS_SET2_Pin);
	GPIO_InitStruct.GPIO_Pin=KEYS_SET3_Pin;
	GPIO_Init(KEYS_SET1_GPIO,&GPIO_InitStruct);
	GPIO_ResetBits(KEYS_SET3_GPIO,KEYS_SET3_Pin);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_InitStruct.GPIO_Pin=KEYS_GET1_Pin;
	GPIO_Init(KEYS_GET1_GPIO,&GPIO_InitStruct);
	GPIO_SetBits(KEYS_GET1_GPIO,KEYS_GET1_Pin);
	GPIO_InitStruct.GPIO_Pin=KEYS_GET2_Pin;
	GPIO_Init(KEYS_GET1_GPIO,&GPIO_InitStruct);
	GPIO_SetBits(KEYS_GET2_GPIO,KEYS_GET2_Pin);
	GPIO_InitStruct.GPIO_Pin=KEYS_GET3_Pin;
	GPIO_Init(KEYS_GET1_GPIO,&GPIO_InitStruct);
	GPIO_SetBits(KEYS_GET3_GPIO,KEYS_GET3_Pin);
	
	//PA1
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);
	EXTI_InitStruct.EXTI_Line=EXTI_Line1;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	NVIC_InitStruct.NVIC_IRQChannel=EXTI1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&NVIC_InitStruct);
	
	//PA2
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource2);
	EXTI_InitStruct.EXTI_Line=EXTI_Line2;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	NVIC_InitStruct.NVIC_IRQChannel=EXTI2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&NVIC_InitStruct);
	
	//PA3
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource3);
	EXTI_InitStruct.EXTI_Line=EXTI_Line3;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	NVIC_InitStruct.NVIC_IRQChannel=EXTI3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&NVIC_InitStruct);
}

uint8_t Keys_Read()
{
	GPIO_SetBits(KEYS_SET1_GPIO,KEYS_SET1_Pin);
	GPIO_SetBits(KEYS_SET2_GPIO,KEYS_SET2_Pin);
	GPIO_SetBits(KEYS_SET3_GPIO,KEYS_SET3_Pin);
	
	GPIO_ResetBits(KEYS_SET1_GPIO,KEYS_SET1_Pin);
	if(GPIO_ReadInputDataBit(KEYS_GET1_GPIO,KEYS_GET1_Pin)==0)return 1;
	if(GPIO_ReadInputDataBit(KEYS_GET2_GPIO,KEYS_GET2_Pin)==0)return 2;
	if(GPIO_ReadInputDataBit(KEYS_GET3_GPIO,KEYS_GET3_Pin)==0)return 3;
	GPIO_SetBits(KEYS_SET1_GPIO,KEYS_SET1_Pin);
	GPIO_ResetBits(KEYS_SET2_GPIO,KEYS_SET2_Pin);
	if(GPIO_ReadInputDataBit(KEYS_GET1_GPIO,KEYS_GET1_Pin)==0)return 4;
	if(GPIO_ReadInputDataBit(KEYS_GET2_GPIO,KEYS_GET2_Pin)==0)return 5;
	if(GPIO_ReadInputDataBit(KEYS_GET3_GPIO,KEYS_GET3_Pin)==0)return 6;
	GPIO_SetBits(KEYS_SET2_GPIO,KEYS_SET2_Pin);
	GPIO_ResetBits(KEYS_SET3_GPIO,KEYS_SET3_Pin);
	if(GPIO_ReadInputDataBit(KEYS_GET1_GPIO,KEYS_GET1_Pin)==0)return 7;
	if(GPIO_ReadInputDataBit(KEYS_GET2_GPIO,KEYS_GET2_Pin)==0)return 8;
	if(GPIO_ReadInputDataBit(KEYS_GET3_GPIO,KEYS_GET3_Pin)==0)return 9;
	GPIO_SetBits(KEYS_SET3_GPIO,KEYS_SET3_Pin);
	
	GPIO_ResetBits(KEYS_SET1_GPIO,KEYS_SET1_Pin);
	GPIO_ResetBits(KEYS_SET2_GPIO,KEYS_SET2_Pin);
	GPIO_ResetBits(KEYS_SET3_GPIO,KEYS_SET3_Pin);
	return 0;
}

void EXTI1_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line1)==0)return;
	delay_ms(20);
	if(GPIO_ReadInputDataBit(KEYS_GET1_GPIO,GPIO_Pin_1)==0)
	{
		GPIO_SetBits(KEYS_SET1_GPIO,KEYS_SET1_Pin);
		GPIO_SetBits(KEYS_SET2_GPIO,KEYS_SET2_Pin);
		GPIO_SetBits(KEYS_SET3_GPIO,KEYS_SET3_Pin);
		
		GPIO_ResetBits(KEYS_SET1_GPIO,KEYS_SET1_Pin);
		if(GPIO_ReadInputDataBit(KEYS_GET1_GPIO,KEYS_GET1_Pin)==0)Key1_Handler();
		GPIO_SetBits(KEYS_SET1_GPIO,KEYS_SET1_Pin);
		
		GPIO_ResetBits(KEYS_SET2_GPIO,KEYS_SET2_Pin);
		if(GPIO_ReadInputDataBit(KEYS_GET1_GPIO,KEYS_GET1_Pin)==0)Key2_Handler();
		GPIO_SetBits(KEYS_SET2_GPIO,KEYS_SET2_Pin);
		
		GPIO_ResetBits(KEYS_SET3_GPIO,KEYS_SET3_Pin);
		if(GPIO_ReadInputDataBit(KEYS_GET1_GPIO,KEYS_GET1_Pin)==0)Key3_Handler();
		GPIO_SetBits(KEYS_SET3_GPIO,KEYS_SET3_Pin);
		
		GPIO_ResetBits(KEYS_SET1_GPIO,KEYS_SET1_Pin);
		GPIO_ResetBits(KEYS_SET2_GPIO,KEYS_SET2_Pin);
		GPIO_ResetBits(KEYS_SET3_GPIO,KEYS_SET3_Pin);
	}
	EXTI_ClearITPendingBit(EXTI_Line1);
}

void EXTI2_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line2)==0)return;
	delay_ms(20);
	if(GPIO_ReadInputDataBit(KEYS_GET1_GPIO,GPIO_Pin_2)==0)
	{
		GPIO_SetBits(KEYS_SET1_GPIO,KEYS_SET1_Pin);
		GPIO_SetBits(KEYS_SET2_GPIO,KEYS_SET2_Pin);
		GPIO_SetBits(KEYS_SET3_GPIO,KEYS_SET3_Pin);
		
		GPIO_ResetBits(KEYS_SET1_GPIO,KEYS_SET1_Pin);
		if(GPIO_ReadInputDataBit(KEYS_GET2_GPIO,KEYS_GET2_Pin)==0)Key4_Handler();
		GPIO_SetBits(KEYS_SET1_GPIO,KEYS_SET1_Pin);
		
		GPIO_ResetBits(KEYS_SET2_GPIO,KEYS_SET2_Pin);
		if(GPIO_ReadInputDataBit(KEYS_GET2_GPIO,KEYS_GET2_Pin)==0)Key5_Handler();
		GPIO_SetBits(KEYS_SET2_GPIO,KEYS_SET2_Pin);
		
		GPIO_ResetBits(KEYS_SET3_GPIO,KEYS_SET3_Pin);
		if(GPIO_ReadInputDataBit(KEYS_GET2_GPIO,KEYS_GET2_Pin)==0)Key6_Handler();
		GPIO_SetBits(KEYS_SET3_GPIO,KEYS_SET3_Pin);
		
		GPIO_ResetBits(KEYS_SET1_GPIO,KEYS_SET1_Pin);
		GPIO_ResetBits(KEYS_SET2_GPIO,KEYS_SET2_Pin);
		GPIO_ResetBits(KEYS_SET3_GPIO,KEYS_SET3_Pin);
	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}

void EXTI3_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line3)==0)return;
	delay_ms(20);
	if(GPIO_ReadInputDataBit(KEYS_GET1_GPIO,GPIO_Pin_3)==0)
	{
		GPIO_SetBits(KEYS_SET1_GPIO,KEYS_SET1_Pin);
		GPIO_SetBits(KEYS_SET2_GPIO,KEYS_SET2_Pin);
		GPIO_SetBits(KEYS_SET3_GPIO,KEYS_SET3_Pin);
		
		GPIO_ResetBits(KEYS_SET1_GPIO,KEYS_SET1_Pin);
		if(GPIO_ReadInputDataBit(KEYS_GET3_GPIO,KEYS_GET3_Pin)==0)Key7_Handler();
		GPIO_SetBits(KEYS_SET1_GPIO,KEYS_SET1_Pin);
		
		GPIO_ResetBits(KEYS_SET2_GPIO,KEYS_SET2_Pin);
		if(GPIO_ReadInputDataBit(KEYS_GET3_GPIO,KEYS_GET3_Pin)==0)Key8_Handler();
		GPIO_SetBits(KEYS_SET2_GPIO,KEYS_SET2_Pin);
		
		GPIO_ResetBits(KEYS_SET3_GPIO,KEYS_SET3_Pin);
		if(GPIO_ReadInputDataBit(KEYS_GET3_GPIO,KEYS_GET3_Pin)==0)Key9_Handler();
		GPIO_SetBits(KEYS_SET3_GPIO,KEYS_SET3_Pin);
		
		GPIO_ResetBits(KEYS_SET1_GPIO,KEYS_SET1_Pin);
		GPIO_ResetBits(KEYS_SET2_GPIO,KEYS_SET2_Pin);
		GPIO_ResetBits(KEYS_SET3_GPIO,KEYS_SET3_Pin);
	}
	EXTI_ClearITPendingBit(EXTI_Line3);
}
