#include "stm32f10x_tim.h"
#include "music.h"
#include "tune.h"

/*
 音高由三位数字组成：
 		个位是表示 1~7 这七个音符 
 		十位是表示音符所在的音区:1-低音，2-中音，3-高音;
 		百位表示这个音符是否要升半音: 0-不升，1-升半音。
 
 音长最多由三位数字组成： 
 		个位表示音符的时值，其对应关系是： 
 			|数值(n):  |0 |1 |2 |3 | 4 | 5 | 6 
 			|几分音符: |1 |2 |4 |8 |16 |32 |64 		音符=2^n
 		十位表示音符的演奏效果(0-2):  0-普通，1-连音，2-顿音
 		百位是符点位: 0-无符点，1-有符点
*/

#define SOUND_SPACE 	4/5 		//定义普通音符演奏的长度分率,//每4分音符间隔

//标准音为440Hz,其他音可由标准音乘 (2^(1/12))^n得到
const float FreTab[12]={261.63,277.18,293.66,311.13,329.63,349.23,369.99,392.00,415.30,440.00,466.16,493.88}; //原始频率表
const uint8_t SignTab[7]={0,2,4,5,7,9,11};							  //1~7在频率表中的位置
const uint8_t LengthTab[7]={1,2,4,8,16,32,64};

Music_t Music_Playing={0};

void Music_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=0;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=500-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler=72-1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM2;
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC2Init(TIM3,&TIM_OCInitStruct);
	
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=0;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=100-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler=7200-1;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	NVIC_InitStruct.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIM2,DISABLE);
	TIM_Cmd(TIM3,DISABLE);
	TIM_ForcedOC2Config(TIM3,TIM_ForcedAction_InActive);
}

void Music_Play(Music_Tune_t music,int Signature,int Octachord,int Speed)
{
	int i,j;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_SelectOCxM(TIM3,TIM_Channel_2,TIM_OCMode_PWM2); 
	TIM_CCxCmd(TIM3,TIM_Channel_2,TIM_CCx_Enable);
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_Cmd(TIM3,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
	Music_Playing.tune=music;
	Music_Playing.progress=music;
	for(i=0;i<12;i++)
	{
		j=i+Signature;
		if(j>11)
		{
			j-=12;
			Music_Playing.NewFreTab[i]=FreTab[j]*2;
		}else{
			Music_Playing.NewFreTab[i]=FreTab[j];
		}
		switch(Octachord)
		{
			case 1:
				Music_Playing.NewFreTab[i]/=4;
				break;
			default:
			case 2:
				break;
			case 3:
				Music_Playing.NewFreTab[i]*=4;
		}
	}
	if(*Music_Playing.progress==0)return;
	Music_Playing.LDiv0=12000/Speed;
	Music_Playing.LDiv4=Music_Playing.LDiv0/4;
	Music_Playing.LDiv4=Music_Playing.LDiv4-Music_Playing.LDiv4*SOUND_SPACE;
	Music_Playing.delay=0;
}

void TIM2_IRQHandler()
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==RESET)return;
	{
		if(Music_Playing.delay!=0)
		{
			Music_Playing.delay--;
		}else{
			if(*Music_Playing.progress==0)
			{
				TIM_Cmd(TIM2,DISABLE);
				TIM_Cmd(TIM3,DISABLE);
				TIM_ForcedOC2Config(TIM3,TIM_ForcedAction_InActive);
			}else{
				uint8_t Tone=Music_Playing.progress[0];
				uint8_t Length=Music_Playing.progress[1];
				uint8_t SL=Tone%10;
				uint8_t SM=Tone/10%10;
				uint8_t SH=Tone/100;
				
				uint8_t SLen=LengthTab[Length%10];
				uint8_t XG=Length/10%10;
				uint8_t FD=Length/100;
				uint8_t LDiv=Music_Playing.LDiv0/SLen;
				uint8_t LDiv1,LDiv2;
				float CurrentFre=Music_Playing.NewFreTab[SignTab[SL-1]+SH];
				if(SL!=0)
				{
					uint16_t tmp;
					switch(SM)
					{
						case 1:
							CurrentFre/=4;
							break;
						default:
						case 2:
							break;
						case 3:
							CurrentFre*=4;
							break;
					}
					tmp=72e6/72/CurrentFre;
					TIM_SetCounter(TIM3,0);
					TIM_SetCompare2(TIM3,tmp/2-1);
					TIM_SetAutoreload(TIM3,tmp-1);
				}
				if(FD==1)
					LDiv=LDiv+LDiv/2;
				if(XG!=1)
					if(XG==0) 				//算出普通音符的演奏长度
						if (SLen<=4)
							LDiv1=LDiv-Music_Playing.LDiv4;
						else
							LDiv1=LDiv*SOUND_SPACE;
					else
						LDiv1=LDiv/2; 		//算出顿音的演奏长度
				else
					LDiv1=LDiv;
				if(SL==0)LDiv1=0;
				LDiv2=LDiv-LDiv1;
				Music_Playing.progress+=2;
				if(SL!=0)
				{
					Music_Playing.delay+=LDiv1;
				}
				if(LDiv2!=0)
				{
					Music_Playing.delay+=LDiv2;
				}
			}
		}
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}


void Music_Stop()
{
	TIM_Cmd(TIM2,DISABLE);
	TIM_Cmd(TIM3,DISABLE);
	TIM_ForcedOC2Config(TIM3,TIM_ForcedAction_InActive);
}

void Music_Continue()
{
	TIM_SelectOCxM(TIM3,TIM_Channel_2,TIM_OCMode_PWM2); 
	TIM_CCxCmd(TIM3,TIM_Channel_2,TIM_CCx_Enable);
	TIM_Cmd(TIM2,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}
