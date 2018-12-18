#include "calendar.h"

const uint32_t OldCalendar_Define[150]=
{
	0x04bd8,0x04ae0,0x0a570,0x054d5,0x0d260,0x0d950,0x16554,0x056a0,0x09ad0,0x055d2,
	0x04ae0,0x0a5b6,0x0a4d0,0x0d250,0x1d255,0x0b540,0x0d6a0,0x0ada2,0x095b0,0x14977,
	0x04970,0x0a4b0,0x0b4b5,0x06a50,0x06d40,0x1ab54,0x02b60,0x09570,0x052f2,0x04970,
	0x06566,0x0d4a0,0x0ea50,0x06e95,0x05ad0,0x02b60,0x186e3,0x092e0,0x1c8d7,0x0c950,
	0x0d4a0,0x1d8a6,0x0b550,0x056a0,0x1a5b4,0x025d0,0x092d0,0x0d2b2,0x0a950,0x0b557,
	0x06ca0,0x0b550,0x15355,0x04da0,0x0a5d0,0x14573,0x052d0,0x0a9a8,0x0e950,0x06aa0,
	0x0aea6,0x0ab50,0x04b60,0x0aae4,0x0a570,0x05260,0x0f263,0x0d950,0x05b57,0x056a0,
	0x096d0,0x04dd5,0x04ad0,0x0a4d0,0x0d4d4,0x0d250,0x0d558,0x0b540,0x0b5a0,0x195a6,
	0x095b0,0x049b0,0x0a974,0x0a4b0,0x0b27a,0x06a50,0x06d40,0x0af46,0x0ab60,0x09570,
	0x04af5,0x04970,0x064b0,0x074a3,0x0ea50,0x06b58,0x055c0,0x0ab60,0x096d5,0x092e0,
	
	0x0c960,0x0d954,0x0d4a0,0x0da50,0x07552,0x056a0,0x0abb7,0x025d0,0x092d0,0x0cab5,
	0x0a950,0x0b4a0,0x0baa4,0x0ad50,0x055d9,0x04ba0,0x0a5b0,0x15176,0x052b0,0x0a930,
	0x07954,0x06aa0,0x0ad50,0x05b52,0x04b60,0x0a6e6,0x0a4e0,0x0d260,0x0ea65,0x0d530,
	0x05aa0,0x076a3,0x096d0,0x04bd7,0x04ad0,0x0a4d0,0x1d0b6,0x0d250,0x0d520,0x0dd45,
	0x0b5a0,0x056d0,0x055b2,0x049b0,0x0a577,0x0a4b0,0x0aa50,0x1b255,0x06d20,0x0ada0,
};

void ClrNewCalendar(NewCalendar*nc)
{
	nc->baseSec=0;
	nc->sec=0;
	nc->min=0;
	nc->hour=0;
	nc->mday=0;
	nc->mon=0;
	nc->year=0;
	nc->wday=0;
	nc->yday=0;
}

void ClrOldCalendar(OldCalendar*oc)
{
	oc->baseSec=0;
	oc->mday=0;
	oc->mon=0;
	oc->year=0;
}

void CpyNewCalendar(NewCalendar*nc,NewCalendar*ncfrom)
{
	if(!nc)return;
	if(!ncfrom)return;
	nc->baseSec=ncfrom->baseSec;
	nc->sec=ncfrom->sec;
	nc->min=ncfrom->min;
	nc->hour=ncfrom->hour;
	nc->mday=ncfrom->mday;
	nc->mon=ncfrom->mon;
	nc->year=ncfrom->year;
	nc->wday=ncfrom->wday;
	nc->yday=ncfrom->yday;
}

void CpyOldCalendar(OldCalendar*oc,OldCalendar*ocfrom)
{
	if(!ocfrom)return;
	if(!oc)return;
	oc->baseSec=ocfrom->baseSec;
	oc->mday=ocfrom->mday;
	oc->mon=ocfrom->mon;
	oc->year=ocfrom->year;
}

uint16_t GetDayNumofYearOldCalendar(uint32_t def)
{
	const char NumArr[16]=
	{
		29*4+30*0,29*3+30*1,29*3+30*1,29*2+30*2,
		29*3+30*1,29*2+30*2,29*2+30*2,29*1+30*3,
		29*3+30*1,29*2+30*2,29*2+30*2,29*1+30*3,
		29*2+30*2,29*1+30*3,29*1+30*3,29*0+30*4,
	};
	return ((def&0x0000f)?((def&0x10000)>>16)?30:29:0)+NumArr[(def>>12)&0xf]+NumArr[(def>>8)&0xf]+NumArr[(def>>4)&0xf];
}

uint8_t GetDayNumofMonOldCalendar(uint32_t def,uint8_t mon)
{
	if(mon<=12)
	{
		if((def>>(15-mon))&1)
			return 30;
		else
			return 29;
	}
	if(mon==(def&0xf))
	{
		if((def>>16)&1)
			return 30;
		else
			return 29;
	}
	return 0;
}

void GetOldCalendar(OldCalendar*oc,uint32_t bsec)
{
	uint32_t bd=bsec/86400-30;//base day
	uint16_t y=0;//year
	uint32_t def;//define
	int i;
	oc->baseSec=bsec;
	while(bd>=GetDayNumofYearOldCalendar(OldCalendar_Define[y]))bd-=GetDayNumofYearOldCalendar(OldCalendar_Define[y]),y++;
	def=OldCalendar_Define[y];
	oc->year=y;
	for(i=0;i<12;i++)
	{
		int dn=((def>>(15-i))&1)?30:29;//day number
		if(bd>=dn)
			bd-=dn;
		else{
			oc->mon=i+1;
			break;
		}
		if((def&0xf)==i+1)
		{
			dn=((def>>16)&1)?30:29;
			if(bd>=dn)
				bd-=dn;
			else{
				oc->mon=i+1+12;
				break;
			}
		}
	}
	oc->mday=bd+1;
}

//不考虑闰秒
void GetNewCalendar(NewCalendar*nc,uint32_t bsec)//new canlendar  base second
{
	uint32_t d=bsec/86400,y=1900;
	uint8_t mond[12]={31,28,31,30,31,30, 31,31,30,31,30,31},m;
	nc->baseSec=bsec;
	nc->sec=bsec%60;
	nc->min=bsec/60%60;
	nc->hour=bsec/3600%24;
	while(((!(y%(y%100?4:400)))?366:365)<=d)d-=((!(y%(y%100?4:400)))?366:365),y++;
	nc->year=y;
	nc->yday=d+1;
	
	if(!(nc->year%(nc->year%100?4:400)))mond[2-1]++;
	d=nc->yday-1;
	m=0;
	while(d>=mond[m])d-=mond[m],m++;
	nc->mday=d+1;
	nc->mon=m+1;
	nc->wday=Getwday(nc->year,nc->yday);
}

uint8_t Getwday(uint16_t year,uint16_t yday)
{
	year--;
	return (year+year/4-year/100+year/400+yday)%7;
}

uint16_t Getyday(uint16_t year,uint8_t mon,uint8_t mday)
{
	uint8_t mond[12]={31,28,31,30,31,30, 31,31,30,31,30,31};
	int16_t yday=mday;
	mond[1]+=!(year%(year%100?4:400));
	mon-=1;
	while(mon--)yday+=mond[mon];
	return yday;
}

uint8_t GetDayNumofMonNewCalendar(uint16_t year,uint8_t mon)
{
	uint8_t mond[12]={31,28,31,30,31,30, 31,31,30,31,30,31};
	mond[1]+=!(year%(year%100?4:400));
	return mond[mon-1];
}

//返回0 表示可处理 -1表示错误或无法处理
int JudgeNewCalendar(NewCalendar*nc)
{
	uint8_t mond[12]={31,28,31,30,31,30, 31,31,30,31,30,31};
	mond[1]+=!(nc->year%(nc->year%100?4:400));
	if(nc->sec>=60)return -1;
	if(nc->min>=60)return -2;
	if(nc->hour>=24)return -3;
	if(nc->year<1901||nc->year>2035)return -4;
	if(nc->mon-1>=12)return -5;
	if(nc->wday>=7)return -6;
	if(nc->yday-1>=((nc->year%(nc->year%100?4:400))?365:366))return -7;
	if(nc->mday-1>=mond[nc->mon-1])return -8;
	if(nc->yday!=Getyday(nc->year,nc->mon,nc->mday))return -9;
	if((nc->year-1+(nc->year-1)/4-(nc->year-1)/100+(nc->year-1)/400+nc->yday)%7!=nc->wday)return -10;
	return 0;
}

//nc1晚于nc2则返回1,相等返回0,早于返回-1
int CmpNewCalendar(NewCalendar*nc1,NewCalendar*nc2)
{
	uint32_t bs1,bs2;//base second
	if(nc1->year>nc2->year)return 1;
	if(nc1->year<nc2->year)return -1;
	bs1=nc1->sec+nc1->min*60+nc1->hour*3600+(nc1->yday-1)*86400;
	bs2=nc2->sec+nc2->min*60+nc2->hour*3600+(nc2->yday-1)*86400;
	if(bs1>bs2)return 1;
	if(bs1<bs2)return -1;
	return 0;
}

//不考虑闰秒 只考察sec,min,hour,yday,year 正确则返回0
uint32_t GetBaseSecond(NewCalendar*nc)
{
	uint32_t bs=nc->sec+nc->min*60+nc->hour*3600+(nc->yday-1)*86400;
	uint16_t y=nc->year;
	//a-->b 称为a趋向于b [doge]
	while(y-->1900)bs+=((!(y%(y%100?4:400)))?366*86400:365*86400);
	return bs;
}
