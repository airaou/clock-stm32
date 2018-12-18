#include <stdio.h>
#include "calendar.h"
#include "rtc.h"
#include "delay.h"

#include "controls.h"

#include "festival.h"
#include "menu_calendar.h"

//日历表下每一天的情况
void Calendar_day(NewCalendar*nc)
{
	int i;
	char str[65]={0},*pstr=str;
	OldCalendar oc[1];
	nc->baseSec=Newcal_GetBaseSecond(nc);
	Oldcal_Get(oc,nc->baseSec);
	OLED_Draw_Scrfull(Controls_OLED,Controls_Buff,0x00);
	OLED_Draw_puts(Controls_OLED,Controls_Buff,0,0,"Festival",0);
	Festival_PrintOldCalendar(pstr,oc);
	while(*pstr)pstr++;
	Festival_PrintFestival(pstr,nc,oc);
	OLED_Draw_putsCN(Controls_OLED,Controls_Buff,0,0,str,0);
	while(1)
	{
		for(i=0;i<9;i++)
		{
			if(Controls_Keys_Had[i]>0)
			{
				Controls_Keys_Had[i]--;
				return;
			}
		}
	}
}
//考察year mon,懒得起名了,功能是将nc中当月的月历映射到tbl中
void Calendar_view_1(Table*tbl,NewCalendar*nc)
{
	int i,j,k=GetDayNumofMonNewCalendar(nc->year,nc->mon);
	const char*letter[]=
	{
		" 1"," 2"," 3"," 4"," 5"," 6"," 7"," 8"," 9","10",
		"11","12","13","14","15","16","17","18","19","20",
		"21","22","23","24","25","26","27","28","29","30",
		"31",
	};
	Table_ClrContent(tbl);
	for(i=Getwday(nc->year,Getyday(nc->year,nc->mon,1)),j=0;j<k;i++,j++)
	{
		Table_SetContent(tbl,i%35%7,i%35/7,letter[j]);
	}
}
//将表格中选中的数字提取
uint8_t Calendar_view_2(Table*tbl)
{
	const char*str=tbl->content[tbl->selectedy][tbl->selectedx];
	char c1,c2;
	if(str)//本来想重新算一遍,但发现直接提取数字还要快 [doge]
	{
		c1=str[0];
		c2=str[1];
		if(c1>='0'&&c1<='9')
			return (c1-'0')*10+(c2-'0');
		else
			return c2-'0';
	}else{
		return 0;
	}
}
/*
	向左上个月,向右下个月
	向上上一年,向下下一年
*/
void Calendar_view()
{
	int k;
	const char*weeknamestr[]={"Su","Mo","Tu","We","Th","Fr","Sa"};
	NewCalendar nc[1];
	char str[10];
	Table tbl[1],weekname[1];
	Text txtyear[1],txtmon[1];
	OLED_TypeDef*oled=Controls_OLED;
	
	Controls_OLED=0;
	OLED_Draw_Scrfull(Controls_OLED,Controls_Buff,0x00);
	
	GetNewCalendar(nc,RTC_GetCnt());
	
	Table_Init(tbl);
	Table_SetUnit(tbl,13,9);
	Table_SetQuan(tbl,7,5);
	Table_SetPosition(tbl,0,13);
	Calendar_view_1(tbl,nc);
	
	Table_Init(weekname);
	Table_SetUnit(weekname,13,12);
	Table_SetQuan(weekname,7,1);
	Table_SetPosition(weekname,0,0);
	for(k=0;k<7;k++)
	{
		Table_SetContent(weekname,k,0,weeknamestr[k]);
	}
	
	Text_Init(txtyear);
	Text_SetPosition(txtyear,100,13);
	Text_Init(txtmon);
	Text_SetPosition(txtmon,100,21);
	
	Table_SetStatus(tbl,Table_Status_Selected);
	Table_SetStatus(weekname,Table_Status_Ready);
	Text_SetStatus(txtyear,Text_Status_Ready);
	Text_SetStatus(txtmon,Text_Status_Ready);
	Controls_OLED=oled;
	OLED_Draw_Refresh(Controls_OLED,Controls_Buff);
	while(1)
	{
		uint8_t i,j=1;
		sprintf(str,"%d",nc->year);
		sprintf(str+5,"%d",nc->mon);
		Text_SetCaption(txtyear,str);
		Text_SetCaption(txtmon,str+5);
		while(j)
		{
			for(i=0;i<9;i++)
			{
				if(Controls_Keys_Had[i]>0)
				{
					Controls_Keys_Had[i]--;
					switch(i)
					{
						case 1:
							if(0==Table_MovSelected(tbl,Table_Dir_Left)&&nc->mon>1)
							{
								nc->mon--;
								Calendar_view_1(tbl,nc);
							}
							break;
						case 7:
							if(0==Table_MovSelected(tbl,Table_Dir_Right)&&nc->mon<12)
							{
								nc->mon++;
								Calendar_view_1(tbl,nc);
							}
							break;
						case 3:
							if(0==Table_MovSelected(tbl,Table_Dir_Up)&&nc->year<2036)
							{
								nc->year++;
								Calendar_view_1(tbl,nc);
							}
							break;
						case 5:
							if(0==Table_MovSelected(tbl,Table_Dir_Down)&&nc->year>1900)
							{
								nc->year--;
								Calendar_view_1(tbl,nc);
							}
							break;
						case 0:
							return;
						case 4:
							nc->mday=Calendar_view_2(tbl);
							if(nc->mday==0)break;
							nc->yday=Newcal_Getyday(nc->year,nc->mon,nc->mday);
							nc->wday=Newcal_Getwday(nc->year,nc->yday);
							if(Newcal_Judge(nc)==0)
							{
								Calendar_day(nc);
								Controls_OLED=0;
								OLED_Draw_Scrfull(Controls_OLED,Controls_Buff,0x00);
								Table_Show(tbl);
								Table_Show(weekname);
								Text_Show(txtyear);
								Text_Show(txtmon);
								Controls_OLED=oled;
								OLED_Draw_Refresh(Controls_OLED,Controls_Buff);
							}
							break;
						default:
							break;
					}
					j=0;
				}
			}
		}
	}
}
