#include "stm32f10x_rcc.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_bkp.h"
#include "delay.h"
#include "rtc.h"

int RTC_Init(uint32_t initCounter)
{
	int tmp;
	int isInit=0;
	NVIC_InitTypeDef NVIC_InitStruct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	if(BKP_ReadBackupRegister(BKP_DR1)!=0x2333)
	{
		BKP_DeInit();
		RCC_LSEConfig(RCC_LSE_ON);
		for(tmp=0;RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET&&tmp<250;tmp++)delay_ms(10);
		if(tmp>=250)return -1;
		
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		
		RCC_RTCCLKCmd(ENABLE);
		RTC_WaitForLastTask();
		RTC_WaitForSynchro();
		RTC_ITConfig(RTC_IT_SEC,ENABLE);
		RTC_WaitForLastTask();
		
		RTC_EnterConfigMode();
		RTC_SetPrescaler(32768-1);
		RTC_WaitForLastTask();
		RTC_SetCounter(initCounter);
		RTC_WaitForLastTask();
		RTC_ExitConfigMode();
		
		BKP_WriteBackupRegister(BKP_DR1,0x2333);
		isInit=1;
	}else{
		RTC_WaitForSynchro();
		RTC_ITConfig(RTC_IT_SEC,ENABLE);
		RTC_WaitForLastTask();
		isInit=0;
	}
	NVIC_InitStruct.NVIC_IRQChannel=RTC_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStruct);
	return isInit;
}

uint32_t RTC_GetCnt(void)
{
	return RTC_GetCounter();
}
void RTC_SetCnt(uint32_t CounterValue)
{
	RTC_SetCounter(CounterValue);
}
void RTC_SetAlr(uint32_t AlarmValue)
{
	RTC_ITConfig(RTC_IT_ALR,ENABLE);
	RTC_SetAlarm(AlarmValue);
}
void RTC_CancelAlr(void)
{
	RTC_ITConfig(RTC_IT_ALR,DISABLE);
}

void RTC_IRQHandler()
{
	if(RTC_GetITStatus(RTC_IT_SEC)!=RESET)
	{
		RTC_ClearITPendingBit(RTC_IT_SEC);
		RTC_Sec_Handler();
	}
	if(RTC_GetITStatus(RTC_IT_ALR)!=RESET)
	{
		RTC_CancelAlr();
		RTC_ClearITPendingBit(RTC_IT_ALR);
		RTC_Alr_Handler();
	}
	RTC_ClearITPendingBit(RTC_IT_OW);
	RTC_WaitForLastTask();
}
