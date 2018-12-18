#include "list.h"

static void List_printScroll(List*list,uint8_t color)
{
	uint8_t x=list->x+list->sizex+3;
	uint8_t space,len,offset;
	if(list->quan>=list->strQuan)return;
	space=(list->unity+1)*list->quan-4;
	len=list->quan*space/list->strQuan;
	offset=list->firstStr*(space-len)/(list->strQuan-list->quan);
	OLED_Draw_Dot(0,Controls_Buff,x,list->y,1);
	OLED_Draw_Dot(0,Controls_Buff,x,list->y+space+4,1);
	if(list->firstStr+list->quan<=list->strQuan)
	{
		OLED_Draw_Line(0,Controls_Buff,x,list->y+2+offset,x,list->y+2+offset+len,color);
	}
}

static void List_printString(List*list)
{
	int i,y;
	for(i=0,y=list->y+2;i<list->quan&&i<list->strQuan;i++,y+=list->unity+1)
	{
		OLED_Draw_puts(0,Controls_Buff,list->x+7,y,list->string[(list->firstStr+i)%list->strQuan],0);
	}
}

static void List_printNet(List*list)
{
	int i,len=list->x+list->sizex;
	for(i=1;i<list->quan;i++)
	{
		int y=list->y+(list->unity+1)*i;
		OLED_Draw_Line(0,Controls_Buff,list->x+1,y,len,y,2);
	}
}

static void List_printRound(List*list,uint8_t color)
{
	OLED_Draw_Square(0,Controls_Buff,list->x,list->y,list->x+list->sizex+1,list->y+list->quan*(list->unity+1),color);
}

void List_Init(List*list)
{
	int i;
	list->x=0;
	list->y=0;
	list->sizex=20;
	list->unity=10;
	list->quan=4;
	list->selected=0;
	for(i=0;i<LIST_STRING_TOTAL;i++)list->string[i]=0;
	list->firstStr=0;
	list->strQuan=4;
	list->status=List_Status_Hide;
	List_Show(list);
}

void List_SetPosition(List*list,uint8_t x,uint8_t y)
{
	List_Close(list,0);
	list->x=x;
	list->y=y;
	List_Show(list);
}

void List_SetSize(List*list,uint8_t sizex,uint8_t unity)
{
	List_Close(list,0);
	list->sizex=sizex;
	list->unity=unity;
	List_Show(list);
}

void List_SetString(List*list,uint8_t n,const char*str)
{
	List_Close(list,0);
	list->string[n]=str;
	List_Show(list);
}

void List_SetSelected(List*list,uint8_t sel)
{
	List_Close(list,0);
	list->selected=sel;
	List_Show(list);
}

void List_SetFirst(List*list,uint8_t fstr)
{
	List_Close(list,0);
	list->firstStr=fstr;
	List_Show(list);
}

void List_SetQuan(List*list,uint8_t quan)
{
	List_Close(list,0);
	list->quan=quan;
	List_Show(list);
}

void List_SetStrQuan(List*list,uint8_t strquan)
{
	List_Close(list,0);
	list->strQuan=strquan;
	List_Show(list);
}

void List_SetStatus(List*list,List_Status status)
{
	List_Close(list,0);
	list->status=status;
	List_Show(list);
}

void List_Show(List*list)
{
	switch(list->status)
	{
		case List_Status_Hide:
		default:
			return;
		case List_Status_Invalid:
			List_printRound(list,2);
			List_printNet(list);
			List_printString(list);
			List_printScroll(list,2);
			break;
		case List_Status_Ready:
			List_printRound(list,1);
			List_printNet(list);
			List_printString(list);
			List_printScroll(list,1);
			break;
		case List_Status_Selected:
			List_printRound(list,1);
			List_printNet(list);
			List_printString(list);
			List_printScroll(list,1);
			OLED_Draw_puts(0,Controls_Buff,list->x+2,list->y+2+(list->unity+1)*list->selected,"\x10",0);
			break;
	}
	OLED_Draw_Refresh(Controls_OLED,Controls_Buff);
}
void List_Close(List*list,uint8_t refresh)
{
	uint8_t i,j;
	if(list->status==List_Status_Hide)return;
	i=list->x+list->sizex+1,j=list->y+(list->unity+1)*list->quan;
	if(list->quan<list->strQuan)i+=2;
	OLED_Draw_Areafull(0,Controls_Buff,list->x,list->y,i,j,0);
	if(refresh)OLED_Draw_Refresh(Controls_OLED,Controls_Buff);
}

void List_MovSelected(List*list,uint8_t dir)
{
	switch(dir)
	{
		case 0://up
			if(list->selected>0)
			{
				List_Close(list,0);
				list->selected--;
				List_Show(list);
			}else{
				if(list->firstStr>0)
				{
					List_SetFirst(list,list->firstStr-1);
				}
			}
			break;
		case 1://down
			if(list->selected+1<list->quan)
			{
				List_Close(list,0);
				list->selected++;
				List_Show(list);
			}else{
				if(list->firstStr+list->quan<list->strQuan)
				{
					List_SetFirst(list,list->firstStr+1);
				}
			}
			break;
		default:
			break;
	}
}

uint8_t List_GetNum(List*list)
{
	return list->firstStr+list->selected;
}
