#ifndef __OLED12864_H__
#define __OLED12864_H__

#include "stm32f10x_gpio.h"

//需要外部自行delay_init()

#define OLED_TypeDef OLED_IIC_TypeDef
#define OLED_Cmd OLED_IIC_Cmd
#define OLED_Dat OLED_IIC_Dat
#define OLED_Init OLED_IIC_Init

typedef struct
{
	GPIO_TypeDef*SCL_GPIO;
	uint16_t SCL_Pin;
	GPIO_TypeDef*SDA_GPIO;
	uint16_t SDA_Pin;
}OLED_IIC_TypeDef;

void OLED_IIC_Init		(OLED_IIC_TypeDef*OLED_IIC);
void OLED_IIC_Cmd		(OLED_IIC_TypeDef*OLED_IIC,uint8_t cmd);
void OLED_IIC_Dat		(OLED_IIC_TypeDef*OLED_IIC,uint8_t dat);

void OLED_IIC_On		(OLED_IIC_TypeDef*OLED_IIC);
void OLED_IIC_Off		(OLED_IIC_TypeDef*OLED_IIC);

#endif
