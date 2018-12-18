#include <string.h>
#include "table.h"

void Table_Init(Table*tbl)
{
	int i,j;
	tbl->x=0;
	tbl->y=0;
	tbl->unitx=6;
	tbl->unity=10;
	tbl->quanx=3;
	tbl->quany=2;
	for(i=0;i<7;i++)for(j=0;j<5;j++)
	{
		tbl->content[j][i]=0;
	}
	tbl->selectedx=0;
	tbl->selectedy=0;
	tbl->status=Table_Status_Hide;
	Table_Show(tbl);
}

void Table_SetPosition(Table*tbl,uint8_t x,uint8_t y)
{
	Table_Close(tbl,0);
	tbl->x=x;
	tbl->y=y;
	Table_Show(tbl);
}

void Table_SetUnit(Table*tbl,uint8_t unitx,uint8_t unity)
{
	Table_Close(tbl,0);
	tbl->unitx=unitx;
	tbl->unity=unity;
	Table_Show(tbl);
}

void Table_SetQuan(Table*tbl,uint8_t quanx,uint8_t quany)
{
	Table_Close(tbl,0);
	tbl->quanx=quanx;
	tbl->quany=quany;
	Table_Show(tbl);
}

void Table_SetContent(Table*tbl,uint8_t x,uint8_t y,const char*str)
{
	tbl->content[y][x]=str;
	switch(tbl->status)
	{
		case Table_Status_Ready:
		case Table_Status_Invalid:
		case Table_Status_Selected:
			OLED_Draw_puts(Controls_OLED,Controls_Buff,tbl->x+2+(tbl->unitx+1)*x,tbl->y+2+(tbl->unity+1)*y,str,0);
			break;
		default:
			break;
	}
}

void Table_SetSelected(Table*tbl,uint8_t x,uint8_t y)
{
	Table_Close(tbl,0);
	tbl->selectedx=x;
	tbl->selectedy=y;
	Table_Show(tbl);
}

void Table_SetStatus(Table*tbl,Table_Status status)
{
	Table_Close(tbl,0);
	tbl->status=status;
	Table_Show(tbl);
}

void Table_Close(Table*tbl,uint8_t refresh)
{
	OLED_Draw_Areafull(0,Controls_Buff,tbl->x,tbl->y,tbl->x+(tbl->unitx+1)*tbl->quanx,tbl->y+(tbl->unity+1)*tbl->quany,0);
	if(refresh)OLED_Draw_Refresh(Controls_OLED,Controls_Buff);
}

void Table_printContent(Table*tbl)
{
	int i,j,bx=tbl->x+2,by=tbl->y+2,sx=tbl->unitx+1,sy=tbl->unity+1,x,y;
	for(i=0,x=bx;i<tbl->quanx;i++,x+=sx)
	{
		for(j=0,y=by;j<tbl->quany;j++,y+=sy)
		{
			OLED_Draw_puts(0,Controls_Buff,x,y,tbl->content[j][i],0);
		}
	}
	OLED_Draw_Refresh(0,Controls_Buff);
}

void Table_printRound(Table*tbl,uint8_t color)
{
	OLED_Draw_Square(0,Controls_Buff,tbl->x,tbl->y,tbl->x+(tbl->unitx+1)*tbl->quanx,tbl->y+(tbl->unity+1)*tbl->quany,color);
}

void Table_printSelected(Table*tbl)
{
	uint8_t x=tbl->x+(tbl->unitx+1)*tbl->selectedx,y=tbl->y+(tbl->unity+1)*tbl->selectedy;
	OLED_Draw_Square(0,Controls_Buff,x,y,x+tbl->unitx+1,y+tbl->unity+1,1);
}

void Table_printNet(Table*tbl)
{
	int i;
	int bx=tbl->x;
	int by=tbl->y;
	int sx=tbl->unitx+1;
	int sy=tbl->unity+1;
	int ex=tbl->x+(tbl->unitx+1)*tbl->quanx-1;
	int ey=tbl->y+(tbl->unity+1)*tbl->quany-1;
	int x,y;
	for(i=1,x=bx+sx;i<tbl->quanx;i++,x+=sx)
	{
		OLED_Draw_Line(0,Controls_Buff,x,tbl->y+1,x,ey,3);
	}
	for(i=1,y=by+sy;i<tbl->quany;i++,y+=sy)
	{
		OLED_Draw_Line(0,Controls_Buff,tbl->x+1,y,ex,y,3);
	}
}
	
void Table_Show(Table*tbl)
{
	switch(tbl->status)
	{
		case Table_Status_Hide:
		default:
			break;
		case Table_Status_Invalid:
			Table_printRound(tbl,2);
			Table_printNet(tbl);
			Table_printContent(tbl);
			break;
		case Table_Status_Ready:
			Table_printRound(tbl,1);
			Table_printNet(tbl);
			Table_printContent(tbl);
			break;
		case Table_Status_Selected:
			Table_printRound(tbl,1);
			Table_printNet(tbl);
			Table_printContent(tbl);
			Table_printSelected(tbl);
			break;
	}
	OLED_Draw_Refresh(Controls_OLED,Controls_Buff);
}

void Table_ClrContent(Table*tbl)
{
	int i,j;
	Table_Close(tbl,0);
	for(i=0;i<tbl->quanx;i++)
	{
		for(j=0;j<tbl->quany;j++)
		{
			tbl->content[j][i]=0;
		}
	}
	Table_Show(tbl);
}

//移动成功返回1 已经到边缘返回0
int Table_MovSelected(Table*tbl,Table_Dir dir)
{
	switch(dir)
	{
		case Table_Dir_Up:
			if(tbl->selectedy>0)
			{
				Table_SetSelected(tbl,tbl->selectedx,tbl->selectedy-1);
				return 1;
			}
			return 0;
		case Table_Dir_Left:
			if(tbl->selectedx>0)
			{
				Table_SetSelected(tbl,tbl->selectedx-1,tbl->selectedy);
				return 1;
			}
			return 0;
		case Table_Dir_Right:
			if(tbl->selectedx+1<tbl->quanx)
			{
				Table_SetSelected(tbl,tbl->selectedx+1,tbl->selectedy);
				return 1;
			}
			return 0;
		case Table_Dir_Down:
			if(tbl->selectedy+1<tbl->quany)
			{
				Table_SetSelected(tbl,tbl->selectedx,tbl->selectedy+1);
				return 1;
			}
			return 0;
		default:
			return -1;
	}
}
