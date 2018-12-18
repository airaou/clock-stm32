#include "controls.h"
#include "alarm.h"
#include "rtc.h"
#include "tune.h"
#include "font.h"
#include "delay.h"
#include "music.h"

Alarm alarm[ALARM_TOTAL]={0};

void Alarm_Init(Alarm*alr,const unsigned char*music)
{
	alr->recentBaseSec=0;
	alr->music=music;
}

void Alarm_SetNewCalendar(Alarm*alr,NewCalendar*nc)
{
	CpyNewCalendar(alr->nc,nc);
}

void Alarm_SetOldCalendar(Alarm*alr,OldCalendar*oc)
{
	CpyOldCalendar(alr->oc,oc);
}

uint32_t Alarm_GetRecentBaseSec(Alarm*alr)
{
	NewCalendar nc[1],tnc[1];
	OldCalendar oc[1];//,toc[1];
	Newcal_Get(nc,RTC_GetCnt());
	Oldcal_Get(oc,RTC_GetCnt());
	switch(alr->mode)
	{
		case Alarm_Mode_Invalid:
		default:
			return 0;
		case Alarm_Mode_EveryDay:
		case Alarm_Mode_Once:
			Newcal_Cpy(tnc,nc);
			tnc->hour=alr->nc->hour;
			tnc->min=alr->nc->min;
			tnc->sec=alr->nc->sec;
			tnc->baseSec=Newcal_GetBaseSecond(tnc);
			if(Newcal_Cmp(tnc,nc)==-1)
				Newcal_Get(tnc,tnc->baseSec+86400);
			return tnc->baseSec;
		case Alarm_Mode_NewEveryWeek:
			return 0;
		case Alarm_Mode_NewEveryMon:
			return 0;
		case Alarm_Mode_NewEveryYear:
			return 0;
		case Alarm_Mode_OldEveryMon:
			return 0;
		case Alarm_Mode_OldEveryYear:
			return 0;
	}
}

//#include "usart.h"
void Alarm_Try()
{
	Alarm*alr;
	if(!Controls_Alr_Had[0])return;
	alr=alarm+Controls_Alr_Had[0]-1;
	Controls_Alr_Had[0]=0;
	OLED_Draw_Pic12864(Controls_OLED,0,pic[0]);
	delay_ms(1000);
	Music_Play(alr->music,0,2,60);
	while(1)
	{
		int i,exit=0;
		delay_ms(100);
	//	printf("debug " __FILE__ ":%d\n",__LINE__);
		for(i=0;i<9;i++)
		{
			if(Controls_Keys_Had[i]>0)
			{
			//	printf("debug " __FILE__ ":%d\n",__LINE__);
				exit=1;
				Controls_Keys_Had[i]--;
			}
		}
		if(exit)break;
	}
	//Music_Play(Music_Girl,0,2,60);//bug
	Music_Stop();
	OLED_Draw_Refresh(Controls_OLED,Controls_Buff);
}

void Alarm_Handle(int arg,void*obj)
{
	alarm[arg].recentBaseSec=0;
	if(alarm[arg].mode==Alarm_Mode_Once)
		alarm[arg].mode=Alarm_Mode_Invalid;
	Alarm_Refresh();
}

void Alarm_Refresh()
{
	int i;
	uint32_t min=UINT32_MAX,minalr=0;
	NewCalendar nc[1];
	Newcal_Get(nc,RTC_GetCnt());
	for(i=0;i<ALARM_TOTAL;i++)
	{
		uint32_t t=alarm[i].recentBaseSec=Alarm_GetRecentBaseSec(alarm+i);
		if(t>nc->baseSec&&t<min)
		{
			min=t;
			minalr=i;
		}
	}
	if(min>nc->baseSec)
	{
		RTC_SetAlr(min);
		setHandle(*Controls_Alr_Handle,Alarm_Handle,minalr,0);
	}
}

