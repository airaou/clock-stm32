#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f10x_gpio.h"

//请初始化:delay_init()

//请不要为Get的三个IO设置相同的Pin
//注意,此处的宏定义不能完全更改引脚设置
#define KEYS_GET1_GPIO GPIOA
#define KEYS_GET2_GPIO GPIOA
#define KEYS_GET3_GPIO GPIOA
#define KEYS_SET1_GPIO GPIOA
#define KEYS_SET2_GPIO GPIOA
#define KEYS_SET3_GPIO GPIOA

#define KEYS_GET1_Pin GPIO_Pin_1
#define KEYS_GET2_Pin GPIO_Pin_2
#define KEYS_GET3_Pin GPIO_Pin_3
#define KEYS_SET1_Pin GPIO_Pin_4
#define KEYS_SET2_Pin GPIO_Pin_5
#define KEYS_SET3_Pin GPIO_Pin_6

void	Keys_Init(void);
uint8_t Keys_Read(void);
//请自行实现以下函数
void	Key1_Handler(void);
void	Key2_Handler(void);
void	Key3_Handler(void);
void	Key4_Handler(void);
void	Key5_Handler(void);
void	Key6_Handler(void);
void	Key7_Handler(void);
void	Key8_Handler(void);
void	Key9_Handler(void);

#endif
