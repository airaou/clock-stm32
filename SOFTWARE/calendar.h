#ifndef __CALENDAR_H__
#define __CALENDAR_H__

#include <stdint.h>

/*
	baseSec 自1900年1月1日0时0分0秒起到现在经过的秒
	sec 秒 0~59
	min 分 0~59
	hour 时 0~23
	mday 日 1~31
	mon 月 1~12
	year 年 从1900年起的年份 1900~2035
	wday 星期 0:日 1:一 ... 6:六 0~6
	yday 该年的第几天 1~366
*/
typedef struct{
	uint32_t baseSec;
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t mday;
	uint8_t mon;
	uint16_t year;
	uint8_t wday;
	uint16_t yday;
}NewCalendar;

/*
	baseSec 自1900年1月1日0时0分0秒起到现在经过的秒
	sec 秒 0~59
	min 分 0~59
	hour 时 0~23
	mday 日 1~30
	mon 月 1~12表示月 13~24表示润月
	year 年 假设1900年1月31日起始的农历年为0年
*/
typedef struct{
	uint32_t baseSec;
	uint8_t mday;
	uint8_t mon;
	uint8_t year;
}OldCalendar;

extern const uint32_t OldCalendar_Define[];

void		Newcal_Clr				(NewCalendar*nc);
void		Newcal_Cpy				(NewCalendar*nc,NewCalendar*ncfrom);
void		Newcal_Get				(NewCalendar*nc,uint32_t bsec);
uint8_t		Newcal_Getwday			(uint16_t year,uint16_t yday);
uint16_t	Newcal_Getyday			(uint16_t year,uint8_t mon,uint8_t mday);
uint8_t		Newcal_GetDayNumofMon	(uint16_t year,uint8_t mon);
int			Newcal_Cmp				(NewCalendar*nc1,NewCalendar*nc2);
uint32_t	Newcal_GetBaseSecond	(NewCalendar*nc);
int			Newcal_Judge			(NewCalendar*nc);

void		Oldcal_Clr				(OldCalendar*oc);
void		Oldcal_Cpy				(OldCalendar*oc,OldCalendar*ocfrom);
void		Oldcal_Get				(OldCalendar*oc,uint32_t bsec);

//兼容
#define ClrNewCalendar Newcal_Clr
#define ClrOldCalendar Oldcal_Clr
#define CpyNewCalendar Newcal_Cpy
#define CpyOldCalendar Oldcal_Cpy
#define GetNewCalendar Newcal_Get
#define GetOldCalendar Oldcal_Get
#define Getwday Newcal_Getwday
#define Getyday Newcal_Getyday
#define GetDayNumofMonNewCalendar Newcal_GetDayNumofMon
#define JudgeNewCalendar Newcal_Judge
#define CmpNewCalendar Newcal_Cmp
#define GetBaseSecond Newcal_GetBaseSecond
/*
void ClrNewCalendar(NewCalendar*nc);
void ClrOldCalendar(OldCalendar*oc);
void CpyNewCalendar(NewCalendar*nc,NewCalendar*ncfrom);
void CpyOldCalendar(OldCalendar*oc,OldCalendar*ocfrom);
void GetNewCalendar(NewCalendar*nc,uint32_t bsec);
void GetOldCalendar(OldCalendar*oc,uint32_t bsec);
uint8_t Getwday(uint16_t year,uint16_t yday);
uint16_t Getyday(uint16_t year,uint8_t mon,uint8_t mday);
uint8_t GetDayNumofMonNewCalendar(uint16_t year,uint8_t mon);
int JudgeNewCalendar(NewCalendar*nc);
int CmpNewCalendar(NewCalendar*nc1,NewCalendar*nc2);
uint32_t GetBaseSecond(NewCalendar*nc);
*/
#endif
