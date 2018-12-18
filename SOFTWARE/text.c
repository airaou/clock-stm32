#include <string.h>
#include "oled12864_draw.h"
#include "controls.h"
#include "text.h"

void Text_Init(Text*txt)
{
	txt->status=Text_Status_Hide;
	txt->x=0;
	txt->y=0;
	strcpy(txt->caption,"T");
	txt->len=1;
	Text_Show(txt);
}

void Text_Show(Text*txt)
{
	if(txt->status==Text_Status_Hide)return;
	if(!txt->caption)return;
	OLED_Draw_puts(0,Controls_Buff,txt->x,txt->y,txt->caption,0);
	OLED_Draw_Refresh(Controls_OLED,Controls_Buff);
}

void Text_Close(Text*txt,uint8_t refresh)
{
	if(!txt->caption)return;
	if(txt->caption[0]!='\xff')
	{
		OLED_Draw_Areafull(0,Controls_Buff,txt->x,txt->y,txt->x+txt->len*5-1,txt->y+8-1,0);
	}else{
		OLED_Draw_Areafull(0,Controls_Buff,txt->x,txt->y,txt->x+txt->len*(16+1)-1,txt->y+16-1,0);
	}
	if(refresh)OLED_Draw_Refresh(Controls_OLED,Controls_Buff);
}

void Text_SetPosition(Text*txt,uint8_t x,uint8_t y)
{
	Text_Close(txt,0);
	txt->x=x;
	txt->y=y;
	Text_Show(txt);
}

void Text_SetCaption(Text*txt,const char*str)
{
	if(!str)return;
	Text_Close(txt,0);
	strcpy(txt->caption,str);
	txt->len=strlen(txt->caption);
	Text_Show(txt);
}

void Text_SetStatus(Text*txt,Text_Status status)
{
	Text_Close(txt,0);
	txt->status=status;
	switch(status)
	{
		case Text_Status_Hide:
			break;
		default:
			Text_Show(txt);
			break;
	}
}
