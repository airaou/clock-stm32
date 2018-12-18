#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <stdint.h>
#include "controls.h"

typedef enum
{
	Button_Status_Invalid,
	Button_Status_Ready,
	Button_Status_Selected,
	Button_Status_Click,
	Button_Status_Hide,
}Button_Status;

typedef struct
{
	uint8_t x;
	uint8_t y;
	uint8_t sizex;
	uint8_t sizey;
	Button_Status status;
	Handle click;
	char caption[16];
}Button;

void Button_Init        (Button*btn);
void Button_Show        (Button*btn);
void Button_SetPosition (Button*btn,uint8_t x,uint8_t y);
void Button_SetCaption  (Button*btn,const char*str);
void Button_Close       (Button*btn,uint8_t refresh);
void Button_SetSize     (Button*btn,uint8_t sizex,uint8_t sizey);
void Button_SetStatus   (Button*btn,Button_Status status);

#endif
