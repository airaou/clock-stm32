#include "progress.h"

void Progress_Init(Progress*pgs)
{
	pgs->x=0;
	pgs->y=0;
	pgs->dir=Progress_Dir_Horizontal;
	pgs->len=10;
	pgs->wid=4;
	pgs->num=0;
	pgs->status=Progress_Status_Hide;
	Progress_Show(pgs,0);
}

void Progress_Close(Progress*pgs)
{
	if(pgs->dir)
	{
		OLED_Draw_Areafull(0,Controls_Buff,pgs->x,pgs->y,pgs->x+pgs->wid+1,pgs->y+pgs->len+1,0);
	}else{
		OLED_Draw_Areafull(0,Controls_Buff,pgs->x,pgs->y,pgs->x+pgs->len+1,pgs->y+pgs->wid+1,0);
	}
	OLED_Draw_Refresh(Controls_OLED,Controls_Buff);
}

void Progress_Show(Progress*pgs,uint8_t opt)
{
	switch(pgs->status)
	{
		case Progress_Status_Invalid:
			if(pgs->dir!=Progress_Dir_Horizontal)
				OLED_Draw_Square(0,Controls_Buff,pgs->x,pgs->y,pgs->x+pgs->wid+1,pgs->y+pgs->len+1,2);
			else
				OLED_Draw_Square(0,Controls_Buff,pgs->x,pgs->y,pgs->x+pgs->len+1,pgs->y+pgs->wid+1,2);
			break;
		case Progress_Status_Ready:
			if(pgs->dir!=Progress_Dir_Horizontal)
			{
				if(!opt)OLED_Draw_Square(0,Controls_Buff,pgs->x,pgs->y,pgs->x+pgs->wid+1,pgs->y+pgs->len+1,1);
				if(pgs->num>0)			OLED_Draw_Areafull(0,Controls_Buff,pgs->x+1,pgs->y+1,pgs->x+pgs->wid,pgs->y+pgs->num,1);
				if(pgs->num<pgs->len)	OLED_Draw_Areafull(0,Controls_Buff,pgs->x+1,pgs->y+pgs->num+1,pgs->x+pgs->wid,pgs->y+pgs->len,0);
			}else{
				if(!opt)OLED_Draw_Square(0,Controls_Buff,pgs->x,pgs->y,pgs->x+pgs->len+1,pgs->y+pgs->wid+1,1);
				if(pgs->num>0)			OLED_Draw_Areafull(0,Controls_Buff,pgs->x+1,pgs->y+1,pgs->x+pgs->num,pgs->y+pgs->wid,1);
				if(pgs->num<pgs->len)	OLED_Draw_Areafull(0,Controls_Buff,pgs->x+pgs->num+1,pgs->y+1,pgs->x+pgs->len,pgs->y+pgs->wid,0);
			}
			break;
		default:
		case Progress_Status_Hide:
			break;
	}
	OLED_Draw_Refresh(Controls_OLED,Controls_Buff);
}
		
void Progress_SetPosition(Progress*pgs,uint8_t x,uint8_t y)
{
	Progress_Close(pgs);
	pgs->x=x;
	pgs->y=y;
	Progress_Show(pgs,0);
}

void Progress_SetDir(Progress*pgs,Progress_Dir dir)
{
	Progress_Close(pgs);
	pgs->dir=dir;
	Progress_Show(pgs,0);
}

void Progress_SetSize(Progress*pgs,uint8_t len,uint8_t wid)
{
	Progress_Close(pgs);
	pgs->len=len;
	pgs->wid=wid;
	Progress_Show(pgs,0);
}

void Progress_SetNumber(Progress*pgs,uint8_t num)
{
	if(pgs->status==Progress_Status_Ready)
	{
		pgs->num=num;
		Progress_Show(pgs,1);
	}
	else
	{
		pgs->num=num;
	}
}

void Progress_SetStatus(Progress*pgs,Progress_Status status)
{
	Progress_Close(pgs);
	pgs->status=status;
	Progress_Show(pgs,0);
}
