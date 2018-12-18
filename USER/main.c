#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "stm32f10x_rcc.h"
#include "delay.h"
#include "usart.h"

#include "dht11.h"
#include "key.h"
#include "music.h"
#include "font.h"
#include "oled12864.h"
#include "oled12864_draw.h"
#include "rtc.h"
#include "tune.h"

#include "calendar.h"
#include "controls.h"

DHT11 dht11;
DHT11_Data dht11_data;
OLED_IIC_TypeDef oled;
OLED_Buff_TypeDef buff;

typedef struct{
	int a;
	char b;
}Node;

int a=offsetof(Node,a);

int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(72);
//	uart_init(72,115200);
//	printf("Hello!\r\n");
	
	oled.SCL_GPIO=GPIOC;
	oled.SCL_Pin=GPIO_Pin_13;
	oled.SDA_GPIO=GPIOB;
	oled.SDA_Pin=GPIO_Pin_10 ;
	OLED_IIC_Init(&oled);
	while(1);
	OLED_Draw_Scrfull(&oled,&buff,0x00);
	OLED_Draw_puts(&oled,&buff,0,0,"starting...  ",0);
	
	OLED_Draw_puts(&oled,&buff,0,0,"RTC Init...  ",0);
	{
		NewCalendar nc[1];
		nc->sec=56;
		nc->min=58;
		nc->hour=0;
		nc->year=2017;
		nc->yday=Getyday(nc->year,12,16);
		RTC_Init(GetBaseSecond(nc));
		srand(RTC_GetCnt());
	}
	
	OLED_Draw_puts(&oled,&buff,0,0,"Keys Init... ",0);
	Keys_Init();
	
	OLED_Draw_puts(&oled,&buff,0,0,"Music Init...",0);
	Music_Init();
	
	OLED_Draw_puts(&oled,&buff,0,0,"DHT11 Init...",0);
	dht11.DATA_GPIO=GPIOB;
	dht11.DATA_Pin=GPIO_Pin_11;
	DHT11_Init(&dht11);
	
	OLED_Draw_Scrfull(&oled,&buff,0x00);
	
	ControlsMain(&oled,&buff);
	
	OLED_Draw_puts(&oled,&buff,0,0,"Waiting      ",0);
	while(1);
}
void RTC_Sec_Handler()
{
	runHandle(Controls_Sec_Handle[0]);
}
void RTC_Alr_Handler()
{
//	printf("alr:\r\n");
	runHandle(Controls_Alr_Handle[0]);
	Controls_Alr_Had[0]=Controls_Alr_Handle[0].arg+1;
}
void Key1_Handler()
{
//	printf("key1:\r\n");
	runHandle(Controls_Keys_Handle[0]);
	Controls_Keys_Had[0]++;
}
void Key2_Handler()
{
//	printf("key2:\r\n");
	runHandle(Controls_Keys_Handle[1]);
	Controls_Keys_Had[1]++;
}
void Key3_Handler()
{
//	printf("key3:\r\n");
	runHandle(Controls_Keys_Handle[2]);
	Controls_Keys_Had[2]++;
}
void Key4_Handler()
{
//	printf("key4:\r\n");
	runHandle(Controls_Keys_Handle[3]);
	Controls_Keys_Had[3]++;
}
void Key5_Handler()
{
//	printf("key5:\r\n");
	runHandle(Controls_Keys_Handle[4]);
	Controls_Keys_Had[4]++;
}
void Key6_Handler()
{
//	printf("key6:\r\n");
	runHandle(Controls_Keys_Handle[5]);
	Controls_Keys_Had[5]++;
}
void Key7_Handler()
{
//	printf("key7:\r\n");
	runHandle(Controls_Keys_Handle[6]);
	Controls_Keys_Had[6]++;
}
void Key8_Handler()
{
//	printf("key8:\r\n");
	runHandle(Controls_Keys_Handle[7]);
	Controls_Keys_Had[7]++;
}
void Key9_Handler()
{
//	printf("key9:\r\n");
	runHandle(Controls_Keys_Handle[8]);
	Controls_Keys_Had[8]++;
}
