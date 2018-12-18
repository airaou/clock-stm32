#include <stdio.h>
#include "calendar.h"
#include "rtc.h"
#include "delay.h"

#include "music.h"
#include "tune.h"
#include "alarm.h"
#include "controls.h"

#include "music_player.h"
#include "menu_options.h"

void Options_CloseScr()
{
	OLED_IIC_Off(Controls_OLED);
	while(1)
	{
		int i;
		delay_ms(20);
		if(Controls_Alr_Had[0]>0){
			OLED_IIC_On(Controls_OLED);
			Alarm_Try();
			return;
		}
		for(i=0;i<9;i++)
		{
			if(Controls_Keys_Had[i]>0)
			{
				Controls_Keys_Had[i]--;
				OLED_IIC_On(Controls_OLED);
				return;
			}
		}
	}
}

void Options_Time()
{
	char hourstr[3],minstr[3];
	Button hour[1]={29,23,17,16,Button_Status_Hide};
	Button min[1]={64,23,17,16,Button_Status_Hide};
	Button cancel[1]={0,47,43,16,Button_Status_Hide};
	Button ok[1]={84,47,43,16,Button_Status_Hide};
	NewCalendar nc[1];
	OLED_TypeDef*oled=Controls_OLED;
	Controls_OLED=0;
	
	Newcal_Get(nc,RTC_GetCnt());
	sprintf(hourstr,"%d",nc->hour);
	sprintf(minstr,"%d",nc->min);
	
	Button_SetCaption(hour,hourstr);
	Button_SetCaption(min,minstr);
	Button_SetCaption(cancel,"Cancel");
	Button_SetCaption(ok,"OK");
	
	OLED_Draw_Scrfull(Controls_OLED,Controls_Buff,0x00);
	
	OLED_Draw_putsCN(Controls_OLED,Controls_Buff,0,0,"现在是",0);
	OLED_Draw_putsCN(Controls_OLED,Controls_Buff,48,22,"时",0);
	OLED_Draw_putsCN(Controls_OLED,Controls_Buff,83,22,"分",0);
	
	Button_SetStatus(hour,Button_Status_Selected);
	Button_SetStatus(min,Button_Status_Ready);
	Button_SetStatus(cancel,Button_Status_Ready);
	Button_SetStatus(ok,Button_Status_Ready);
	
	Controls_OLED=oled;
	OLED_Draw_Refresh(Controls_OLED,Controls_Buff);
	while(1)
	{
		int i;
		for(i=0;i<9;i++)
		{
			if(Controls_Keys_Had[i]>0)
			{
				Controls_Keys_Had[i]--;
				switch(i)
				{
					case 1:
						if(min->status==Button_Status_Selected)
						{
							Button_SetStatus(min,Button_Status_Ready);
							Button_SetStatus(hour,Button_Status_Selected);
						}
						break;
					case 7:
						if(hour->status==Button_Status_Selected)
						{
							Button_SetStatus(hour,Button_Status_Ready);
							Button_SetStatus(min,Button_Status_Selected);
						}
						break;
					case 3:
						if(hour->status==Button_Status_Selected)
						{
							if(nc->hour<23)nc->hour++;else nc->hour=0;
							sprintf(hourstr,"%d",nc->hour);
							Button_SetCaption(hour,hourstr);
						}
						if(min->status==Button_Status_Selected)
						{
							if(nc->min<59)nc->min++;else nc->min=0;
							sprintf(minstr,"%d",nc->min);
							Button_SetCaption(min,minstr);
						}
						break;
					case 5:
						if(hour->status==Button_Status_Selected)
						{
							if(nc->hour>0)nc->hour--;else nc->hour=23;
							sprintf(hourstr,"%d",nc->hour);
							Button_SetCaption(hour,hourstr);
						}
						if(min->status==Button_Status_Selected)
						{
							if(nc->min>0)nc->min--;else nc->min=59;
							sprintf(minstr,"%d",nc->min);
							Button_SetCaption(min,minstr);
						}
						break;
					case 6:
						nc->sec=0;
						if(Newcal_Judge(nc)==0)
						{
							RTC_SetCnt(Newcal_GetBaseSecond(nc));
							Button_SetCaption(ok,"Succecc");
							delay_ms(1000);
							return;
						}
						Button_SetCaption(ok,"Error");
						break;
					case 0:
						Alarm_Refresh();
						return;
					default:
						break;
				}
			}
		}
	}
}

void Options_Date()
{
	char yearstr[5],monstr[3],daystr[3];
	Button year[1]={49,0,28,16,Button_Status_Hide};
	Button mon[1]={0,18,28,16,Button_Status_Hide};
	Button day[1]={49,18,28,16,Button_Status_Hide};
	Button cancel[1]={0,47,43,16,Button_Status_Hide};
	Button ok[1]={84,47,43,16,Button_Status_Hide};
	NewCalendar nc[1],tnc[1];
	OLED_TypeDef*oled=Controls_OLED;
	Controls_OLED=0;
	
	Newcal_Get(nc,RTC_GetCnt());
	sprintf(yearstr,"%d",nc->year);
	sprintf(monstr,"%d",nc->mon);
	sprintf(daystr,"%d",nc->mday);
	
	Button_SetCaption(year,yearstr);
	Button_SetCaption(mon,monstr);
	Button_SetCaption(day,daystr);
	Button_SetCaption(cancel,"Cancel");
	Button_SetCaption(ok,"OK");
	
	OLED_Draw_Scrfull(Controls_OLED,Controls_Buff,0x00);
	
	OLED_Draw_puts(Controls_OLED,Controls_Buff,0,0,"\xff今天是",0);
	OLED_Draw_puts(Controls_OLED,Controls_Buff,80,0,"\xff年",0);
	OLED_Draw_puts(Controls_OLED,Controls_Buff,34,18,"\xff月",0);
	OLED_Draw_puts(Controls_OLED,Controls_Buff,80,18,"\xff日",0);

	Button_SetStatus(year,Button_Status_Ready);
	Button_SetStatus(mon,Button_Status_Ready);
	Button_SetStatus(day,Button_Status_Ready);
	Button_SetStatus(cancel,Button_Status_Ready);
	Button_SetStatus(ok,Button_Status_Ready);
	
	Button_SetStatus(year,Button_Status_Selected);
	
	Controls_OLED=oled;
	OLED_Draw_Refresh(Controls_OLED,Controls_Buff);
	while(1)
	{
		int i;
		for(i=0;i<9;i++)
		{
			if(Controls_Keys_Had[i]>0)
			{
				Controls_Keys_Had[i]--;
				switch(i)
				{
					case 1:
						if(day->status==Button_Status_Selected)
						{
							Button_SetStatus(day,Button_Status_Ready);
							Button_SetStatus(mon,Button_Status_Selected);
							break;
						}
						if(mon->status==Button_Status_Selected)
						{
							Button_SetStatus(mon,Button_Status_Ready);
							Button_SetStatus(year,Button_Status_Selected);
							break;
						}
						break;
					case 7:
						if(year->status==Button_Status_Selected)
						{
							Button_SetStatus(year,Button_Status_Ready);
							Button_SetStatus(mon,Button_Status_Selected);
							break;
						}
						if(mon->status==Button_Status_Selected)
						{
							Button_SetStatus(mon,Button_Status_Ready);
							Button_SetStatus(day,Button_Status_Selected);
							break;
						}
						break;
					case 3:
						if(mon->status==Button_Status_Selected)
						{
							if(nc->mon<12)nc->mon++;
							sprintf(monstr,"%d",nc->mon);
							Button_SetCaption(mon,monstr);
						}
						if(year->status==Button_Status_Selected)
						{
							if(nc->year<2036)nc->year++;
							sprintf(yearstr,"%d",nc->year);
							Button_SetCaption(year,yearstr);
						}
						if(day->status==Button_Status_Selected)
						{
							if(nc->mday<31)nc->mday++;
							sprintf(daystr,"%d",nc->mday);
							Button_SetCaption(day,daystr);
						}
						break;
					case 5:
						if(mon->status==Button_Status_Selected)
						{
							if(nc->mon>1)nc->mon--;
							sprintf(monstr,"%d",nc->mon);
							Button_SetCaption(mon,monstr);
						}
						if(year->status==Button_Status_Selected)
						{
							if(nc->year>1970)nc->year--;
							sprintf(yearstr,"%d",nc->year);
							Button_SetCaption(year,yearstr);
						}
						if(day->status==Button_Status_Selected)
						{
							if(nc->mday>1)nc->mday--;
							sprintf(daystr,"%d",nc->mday);
							Button_SetCaption(day,daystr);
						}
						break;
					case 6:
						Newcal_Get(tnc,RTC_GetCnt());
						tnc->mday=nc->mday;
						tnc->year=nc->year;
						tnc->mon=nc->mon;
						tnc->yday=Newcal_Getyday(tnc->year,tnc->mon,tnc->mday);
						tnc->wday=Newcal_Getwday(tnc->year,tnc->yday);
						if(Newcal_Judge(tnc)==0)
						{
							RTC_SetCnt(Newcal_GetBaseSecond(tnc));
							Button_SetCaption(ok,"Succecc");
							delay_ms(1000);
							return;
						}
						Button_SetCaption(ok,"Error");
						break;
					case 0:
						Alarm_Refresh();
						return;
					default:
						break;
				}
			}
		}
	}
}

void Options_view()
{
	int i;
	const char*optname[]={"Time","Date","CloseScr"};
	List list[1];

	OLED_Draw_Scrfull(Controls_OLED,Controls_Buff,0x00);
	
	List_Init(list);
	List_SetPosition(list,0,0);
	List_SetSize(list,50,9);
	List_SetQuan(list,6);
	for(i=0;i<4;i++)
	{
		List_SetString(list,i,optname[i]);
	}
	List_SetStrQuan(list,3);
	
	List_SetStatus(list,List_Status_Selected);
	while(1)
	{
		int i;
		for(i=0;i<9;i++)
		{
			if(Controls_Keys_Had[i]>0)
			{
				Controls_Keys_Had[i]--;
				switch(i)
				{
					case 3:
						List_MovSelected(list,0);
						break;
					case 5:
						List_MovSelected(list,1);
						break;
					case 4:
						switch(List_GetNum(list))
						{
							case 0:
								Options_Time();
								break;
							case 1:
								Options_Date();
								break;
							case 2:
								Options_CloseScr();
								break;
							default:
								break;
						}
						OLED_Draw_Scrfull(Controls_OLED,Controls_Buff,0x00);
						List_SetStatus(list,list->status);
						break;
					case 0:
						return;
					default:
						break;
				}
			}
		}
	}
}
