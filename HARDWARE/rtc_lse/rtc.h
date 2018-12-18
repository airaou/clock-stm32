#ifndef __RTC_H__
#define __RTC_H__

#include <stdint.h>

//使用LSE外部晶振
//需自行delay_init()

int RTC_Init(uint32_t initCounter);

uint32_t RTC_GetCnt(void);
void RTC_SetCnt(uint32_t CounterValue);
void RTC_SetAlr(uint32_t AlarmValue);
void RTC_CancelAlr(void);

//请自行实现
void RTC_Sec_Handler(void);
void RTC_Alr_Handler(void);

#endif
