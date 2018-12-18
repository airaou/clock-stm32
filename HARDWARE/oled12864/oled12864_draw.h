#ifndef __OLED12864_DRAW_H__
#define __OLED12864_DRAW_H__

#include "oled12864.h"

typedef struct
{
	uint8_t buff[8][128];
}OLED_Buff_TypeDef;

void OLED_Draw_Pic12864	(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff,const char*dat);
void OLED_Draw_Scrfull	(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff,uint8_t dat);
void OLED_Draw_Dot		(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff,uint8_t x,uint8_t y,uint8_t color);
void OLED_Draw_Refresh	(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff);
void OLED_Draw_RanByte	(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff,uint8_t x,uint8_t y,uint8_t byte,uint8_t trans);
void OLED_Draw_RanBytes	(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff,uint8_t weight,uint8_t x,uint8_t y,const char*arr,uint8_t trans);
void OLED_Draw_Line		(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff,uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t color);
void OLED_Draw_Square	(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff,uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t color);
void OLED_Draw_puts		(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff,uint8_t x,uint8_t y,const char*string,uint8_t trans);
void OLED_Draw_putsCN	(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff,uint8_t x,uint8_t y,const char*string,uint8_t trans);
void OLED_Draw_Picture16(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff,uint8_t x,uint8_t y,const char*pic,uint8_t trans);
void OLED_Draw_Picture32(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff,uint8_t x,uint8_t y,const char*pic,uint8_t trans);
void OLED_Draw_Number	(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff,uint8_t x,uint8_t y,int32_t num,uint8_t trans);
void OLED_Draw_Areafull (OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff,uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t color);

#endif
