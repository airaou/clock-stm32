#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "delay.h"
#include "DHT11.h"

#include "usart.h"
//ÐèÊÂÏÈdelay_init()

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

void DHT11_Reset(DHT11*dht11)
{
	GPIO_ResetBits(dht11->DATA_GPIO,dht11->DATA_Pin);
	delay_ms(20);
	GPIO_SetBits(dht11->DATA_GPIO,dht11->DATA_Pin);
	delay_us(30);
}

void DHT11_Init(DHT11*dht11)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(GPIO2RCCAPB2Periph_GPIO(dht11->DATA_GPIO),ENABLE);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=dht11->DATA_Pin;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(dht11->DATA_GPIO,&GPIO_InitStruct);
	GPIO_SetBits(dht11->DATA_GPIO,dht11->DATA_Pin);
	DHT11_Reset(dht11);
	delay_ms(1000);
}

static int Read_Bit(DHT11*dht11)
{
	uint32_t t=0;
	while(GPIO_ReadInputDataBit(dht11->DATA_GPIO,dht11->DATA_Pin)&&t<100)
	{
		delay_us(1);
		t++;
	}
	if(t>=100)return -1;
	t=0;
	while((!GPIO_ReadInputDataBit(dht11->DATA_GPIO,dht11->DATA_Pin))&&t<100)
	{
		delay_us(1);
		t++;
	}
	if(t>=100)return -1;
	delay_us(40);
	return GPIO_ReadInputDataBit(dht11->DATA_GPIO,dht11->DATA_Pin);
	/*
	while(GPIO_ReadInputDataBit(dht11->DATA_GPIO,dht11->DATA_Pin))
	{
		delay_us(1);
		u++;
	}
	return u>t;*/
}
void DHT11_Get(DHT11*dht11,DHT11_Data*data)
{
	int i;
	uint8_t*p=(uint8_t*)data;
	GPIO_InitTypeDef GPIO_InitStruct;
	DHT11_Reset(dht11);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=dht11->DATA_Pin;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(dht11->DATA_GPIO,&GPIO_InitStruct);
	GPIO_SetBits(dht11->DATA_GPIO,dht11->DATA_Pin);
	while(GPIO_ReadInputDataBit(dht11->DATA_GPIO,dht11->DATA_Pin));

	*p=0;
	for(i=0;i<40;i++)
	{
		*p|=Read_Bit(dht11);
		*p<<=1;
		if((i+1)%8==0)*++p=0;
	}
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(dht11->DATA_GPIO,&GPIO_InitStruct);
	GPIO_SetBits(dht11->DATA_GPIO,dht11->DATA_Pin);
}
