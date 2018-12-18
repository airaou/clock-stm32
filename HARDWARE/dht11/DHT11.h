#ifndef __DHT11_H__
#define __DHT11_H__

#include "stm32f10x_gpio.h"

typedef struct
{
	GPIO_TypeDef*DATA_GPIO;
	uint16_t DATA_Pin;
}DHT11;

typedef struct
{
	uint8_t HumidityInteger;	
	uint8_t HumidityDecimal;
	uint8_t TemperatureInteger;
	uint8_t TemperatureDecimal;
	uint8_t Check;
}DHT11_Data;

void DHT11_Init(DHT11*dht11);
void DHT11_Get(DHT11*dht11,DHT11_Data*data);

#endif
