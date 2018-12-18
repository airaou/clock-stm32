#include <stdio.h>
#include "rtc.h"
#include "controls.h"
#include "alarm.h"
#include "menu_alr.h"
#include "music.h"
#include "tune.h"

const char*musicname[]=
{
	"Girl",
	"Same",
	"Two",
	"Heart",
	"Birth",
	0
};

const char*namestr[]=
{
	"OFF",
	"Once",
	"EveDay",
	"EveWeek",
	
	"EveNewM",
	"EveNewY",
	"EveLnaM",
	"EveLnaY",
	
	"Alarm",
	"min=",
	"hour=",
	"day=",
	"mon=",
	"week=",
	0
};

//映射alr到List中
void Alarm_set_1(List*list,Alarm*alr,char str[6][8])
{
	int i=0;
	sprintf(str[i],"%s %d",namestr[8],alr-alarm);
	List_SetString(list,i,str[i]);
	i++;
	sprintf(str[i],"%s",namestr[alr->mode]);
	List_SetString(list,i,str[i]);
	i++;
	if(alr->music==Music_Girl)
		sprintf(str[i],"%s",musicname[0]);
	else if(alr->music==Music_Same)
		sprintf(str[i],"%s",musicname[1]);
	else if(alr->music==Music_Two)
		sprintf(str[i],"%s",musicname[2]);
	else if(alr->music==Music_Heart)
		sprintf(str[i],"%s",musicname[3]);
	else if(alr->music==Music_Birth)
		sprintf(str[i],"%s",musicname[4]);
	List_SetString(list,i,str[i]);
	i++;
	sprintf(str[i],"%s%d",namestr[9],alr->nc->min);
	List_SetString(list,i,str[i]);
	i++;
	sprintf(str[i],"%s%d",namestr[10],alr->nc->hour);
	List_SetString(list,i,str[i]);
	i++;
	switch(alr->mode)
	{
		case Alarm_Mode_Invalid:
		case Alarm_Mode_Once:
		case Alarm_Mode_EveryDay:
		default:
			break;
		case Alarm_Mode_NewEveryWeek:
			sprintf(str[i],"%s%d",namestr[13],alr->nc->wday);
			List_SetString(list,i,str[i]);
			i++;
			break;
		case Alarm_Mode_NewEveryYear:
			sprintf(str[i],"%s%d",namestr[12],alr->nc->mon);
			List_SetString(list,i,str[i]);
			i++;
		case Alarm_Mode_NewEveryMon:
			sprintf(str[i],"%s%d",namestr[11],alr->nc->mday);
			List_SetString(list,i,str[i]);
			i++;
			break;
		case Alarm_Mode_OldEveryYear:
			sprintf(str[i],"%s%d",namestr[12],alr->oc->mon);
			List_SetString(list,i,str[i]);
			i++;
		case Alarm_Mode_OldEveryMon:
			sprintf(str[i],"%s%d",namestr[11],alr->oc->mday);
			List_SetString(list,i,str[i]);
			i++;
			break;
	}
	List_SetStrQuan(list,i);
	List_SetQuan(list,i>6?6:i);
}
void Alarm_set(Alarm*alr)
{
	int i;
	unsigned char const**j=Musics;
	OLED_TypeDef*oled=Controls_OLED;
	char str[6][8];
	List list[1];
	List_Init(list);
	List_SetPosition(list,16,2);
	List_SetSize(list,90,9);
	List_SetQuan(list,1);
	List_SetStrQuan(list,1);
	List_SetStatus(list,List_Status_Invalid);
	
	Controls_OLED=0;
	
	Alarm_set_1(list,alr,str);
	
	List_SetStatus(list,List_Status_Selected);
	Controls_OLED=oled;
	OLED_Draw_Refresh(Controls_OLED,Controls_Buff);
	while(1)
	{
		for(i=0;i<9;i++)
		{
			if(Controls_Keys_Had[i]>0)
			{
				Controls_Keys_Had[i]--;
				switch(i)
				{
				case 0:
					return;
				case 3:
					List_MovSelected(list,0);
					break;
				case 5:
					List_MovSelected(list,1);
					break;
				case 7:
					switch(List_GetNum(list))
					{
					case 1:
						if(alr->mode<Alarm_Mode_OldEveryYear)
							alr->mode++;
						else
							alr->mode=(Alarm_Mode)0;
						List_SetString(list,1,namestr[alr->mode]);
						Alarm_set_1(list,alr,str);
						break;
					case 2:
						for(j=Musics;*j;j++)
						{
							if(*j==alr->music)
							{
								if(j[1])
								{
									alr->music=j[1];
								}else{
									alr->music=Musics[0];
								}
								break;
							}
						}
						Alarm_set_1(list,alr,str);
						break;
					case 3:
						if(alr->nc->min<59)
							alr->nc->min++;
						else
							alr->nc->min=0;
						Alarm_set_1(list,alr,str);
						break;
					case 4:
						if(alr->nc->hour<23)
							alr->nc->hour++;
						else
							alr->nc->hour=0;
						Alarm_set_1(list,alr,str);
						break;
					default:
						break;
					}
					break;
				case 1:
					switch(List_GetNum(list))
					{
					case 1:
						if(alr->mode>0)
							alr->mode--;
						else
							alr->mode=Alarm_Mode_OldEveryYear;
						List_SetString(list,1,namestr[alr->mode]);
						Alarm_set_1(list,alr,str);
						break;
					case 2:
						for(j=Musics;*j;j++)
						{
							if(*j==alr->music)
							{
								if(j==Musics)
								{
									alr->music=Music_Birth;
								}else{
									alr->music=j[-1];
								}
								break;
							}
						}
						Alarm_set_1(list,alr,str);
						break;
					case 3:
						if(alr->nc->min>0)
							alr->nc->min--;
						else
							alr->nc->min=59;
						Alarm_set_1(list,alr,str);
						break;
					case 4:
						if(alr->nc->hour>0)
							alr->nc->hour--;
						else
							alr->nc->hour=23;
						Alarm_set_1(list,alr,str);
						break;
					default:
						break;
					}
					break;
				default:
					break;
				}
			}
		}
	}
}

//将alarm映射到list中
void Alarm_view_1(List*list,char(*str)[2+1+2+1+2+1+7+1])
{
	int i;
	List_SetStrQuan(list,LIST_STRING_TOTAL);
	for(i=0;i<ALARM_TOTAL&&i<LIST_STRING_TOTAL;i++)
	{
		sprintf(str[i],"%d %02d:%02d %s",i,alarm[i].nc->hour,alarm[i].nc->min,namestr[alarm[i].mode]);
		List_SetString(list,i,str[i]);
	}
}

void Alarm_view()
{
	int i;
	List list[1];
	char str[ALARM_TOTAL][2+1+2+1+2+1+7+1];
	OLED_TypeDef*oled=Controls_OLED;
	
	Controls_OLED=0;
	OLED_Draw_Scrfull(Controls_OLED,Controls_Buff,0x00);
	
	List_Init(list);
	List_SetPosition(list,0,0);
	List_SetSize(list,90,9);
	List_SetQuan(list,6);
	Alarm_view_1(list,str);
	
	List_SetStatus(list,List_Status_Selected);
	Controls_OLED=oled;
	OLED_Draw_Refresh(Controls_OLED,Controls_Buff);
	while(1)
	{
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
					case 0:
						Alarm_Refresh();
						return;
					case 4:
						Alarm_Init(alarm+List_GetNum(list),Music_Two);
						Alarm_set(alarm+List_GetNum(list));
						Alarm_view_1(list,str);
						break;
					default:
						break;
				}
				OLED_Draw_Scrfull(0,Controls_Buff,0x00);
				List_Show(list);
			}
		}
	}
}
