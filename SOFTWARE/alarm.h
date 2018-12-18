#ifndef __ALARM_H__
#define __ALARM_H__

#include <stdint.h>
#include "calendar.h"

typedef enum
{
	Alarm_Mode_Invalid,
	Alarm_Mode_Once,//使用hour min sec
	Alarm_Mode_EveryDay,//使用hour min sec
	//NewCalendar
	Alarm_Mode_NewEveryWeek,//使用wday hour min sec
	Alarm_Mode_NewEveryMon,//使用mday hour min sec
	Alarm_Mode_NewEveryYear,//使用mon mday hour min sec
	//OldCalenadr
	Alarm_Mode_OldEveryMon,//使用mday hour min sec
	Alarm_Mode_OldEveryYear,//使用mon mday hour min sec
}Alarm_Mode;

typedef struct
{
	uint32_t recentBaseSec;
	const unsigned char*music;
	Alarm_Mode mode;
	NewCalendar nc[1];
	OldCalendar oc[1];
	
}Alarm;

#define ALARM_TOTAL 16
extern Alarm alarm[ALARM_TOTAL];

void Alarm_Init(Alarm*alr,const unsigned char*music);
void Alarm_SetNewCalendar(Alarm*alr,NewCalendar*nc);
void Alarm_SetOldCalendar(Alarm*alr,OldCalendar*oc);
void Alarm_Refresh(void);
void Alarm_Try(void);

#endif
