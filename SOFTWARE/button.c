#include <string.h>
#include "oled12864_draw.h"
#include "controls.h"
#include "button.h"

void Button_Init(Button*btn)
{
	btn->status=Button_Status_Hide;
	btn->x=0;
	btn->y=0;
	btn->sizex=22;
	btn->sizey=12;
	strcpy(btn->caption,"B");
	Button_Show(btn);
}

void Button_Show(Button*btn)
{
	switch(btn->status)
	{
		case Button_Status_Ready:
			OLED_Draw_Square(0,Controls_Buff,btn->x,btn->y,btn->x+btn->sizex,btn->y+btn->sizey,1);
			break;
		case Button_Status_Selected:
			OLED_Draw_Square(0,Controls_Buff,btn->x+2,btn->y+2,btn->x+btn->sizex-2,btn->y+btn->sizey-2,2);
			OLED_Draw_Square(0,Controls_Buff,btn->x,btn->y,btn->x+btn->sizex,btn->y+btn->sizey,1);
			break;
		case Button_Status_Click:
			OLED_Draw_Square(0,Controls_Buff,btn->x+2,btn->y+2,btn->x+btn->sizex-2,btn->y+btn->sizey-2,1);
			OLED_Draw_Square(0,Controls_Buff,btn->x,btn->y,btn->x+btn->sizex,btn->y+btn->sizey,1);
			break;
		case Button_Status_Invalid:
			OLED_Draw_Square(0,Controls_Buff,btn->x,btn->y,btn->x+btn->sizex,btn->y+btn->sizey,2);
			break;
		default:
		case Button_Status_Hide:
			return;
	}
	OLED_Draw_puts(0,Controls_Buff,btn->x+4,btn->y+4,btn->caption,1);
	OLED_Draw_Refresh(Controls_OLED,Controls_Buff);
}

void Button_Close(Button*btn,uint8_t refresh)
{
	OLED_Draw_Areafull(0,Controls_Buff,btn->x,btn->y,btn->x+btn->sizex,btn->y+btn->sizey,0);
	if(refresh)OLED_Draw_Refresh(Controls_OLED,Controls_Buff);
}

void Button_SetPosition(Button*btn,uint8_t x,uint8_t y)
{
	Button_Close(btn,0);
	btn->x=x;
	btn->y=y;
	Button_Show(btn);
}

void Button_SetCaption(Button*btn,const char*str)
{
	int len=strlen(str);
	if(len*5+7>btn->sizex)Button_SetSize(btn,len*5+7,btn->sizey);
	Button_Close(btn,0);
	strcpy(btn->caption,str);
	Button_Show(btn);
}

void Button_SetSize(Button*btn,uint8_t sizex,uint8_t sizey)
{
	Button_Close(btn,0);
	btn->sizex=sizex;
	btn->sizey=sizey;
	Button_Show(btn);
}

void Button_SetStatus(Button*btn,Button_Status status)
{
	Button_Close(btn,0);
	btn->status=status;
	switch(status)
	{
		case Button_Status_Hide:
			break;
		default:
			Button_Show(btn);
			break;
	}
}
