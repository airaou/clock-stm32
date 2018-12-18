#include <stdint.h>
#include <stdlib.h>
#include "rtc.h"
#include "delay.h"
#include "oled12864_draw.h"
#include "controls.h"
#include "game_snaker.h"

typedef enum{
	Floor_None,
	Floor_Body,
	Floor_Food,
	Floor_Wall,
}Floor;


static void Dot(int x,int y,uint8_t color){
	OLED_Draw_Dot(0,Controls_Buff,x*2+0,y*2+0,color);
	OLED_Draw_Dot(0,Controls_Buff,x*2+1,y*2+0,color);
	OLED_Draw_Dot(0,Controls_Buff,x*2+0,y*2+1,color);
	OLED_Draw_Dot(0,Controls_Buff,x*2+1,y*2+1,color);
}

uint8_t Snaker_Map(Map map,uint8_t x,uint8_t y){
	switch(map){
	default:
	case Map_None:
		return 0;
	case Map_Besieged:
		if(x==0)return 1;
		if(y==0)return 1;
		if(x==COLS-1)return 1;
		if(y==ROWS-1)return 1;
		return 0;
	case Map_Tube:
		if(y==8&&x>16&&x<48)return 1;
		if(y==24&&x>16&&x<48)return 1;
		return 0;
	}
}

int Snaker_Body(Snaker*this,uint8_t x,uint8_t y){
	Body*pb=this->tail;
	while(pb!=this->head){
		if(pb->x==x&&pb->y==y)
			return 1;
		pb++;
		if(pb>=this->bodys+this->maxlength)
			pb=this->bodys;
	}
	if(this->head->x==x&&this->head->y==y)return 2;
	return 0;
}

void Snaker_newfood(Snaker*this){
	uint8_t x,y;
	do{
		x=rand()%COLS;
		y=rand()%ROWS;
	}while(Snaker_Body(this,x,y)!=0||Snaker_Map(this->map,x,y));
	this->food->x=x;
	this->food->y=y;
}

int Snaker_Add(Snaker*this){
	Body*old;
	if(this->length<this->maxlength){
		this->length++;
		old=this->head;
		if(this->head-this->bodys<this->maxlength-1)
			this->head++;
		else
			this->head=this->bodys;
		switch(this->dir){
		case Dir_Up:
			this->head->x=old->x;
			this->head->y=(old->y==0)?ROWS-1:old->y-1;
			break;
		case Dir_Down:
			this->head->x=old->x;
			this->head->y=(old->y==ROWS-1)?0:old->y+1;
			break;
		case Dir_Left:
			this->head->x=(old->x==0)?COLS-1:old->x-1;
			this->head->y=old->y;
			break;
		case Dir_Right:
			this->head->x=(old->x==COLS-1)?0:old->x+1;
			this->head->y=old->y;
			break;
		default:
			break;
		}
		if(Snaker_Body(this,this->head->x,this->head->y)==1)
			return -1;
		if(Snaker_Map(this->map,this->head->x,this->head->y))
			return -1;
		if(Snaker_Body(this,this->food->x,this->food->y))
			return 1;
		return 0;
	}
	return 2;
}

void Snaker_Sub(Snaker*this){
	if(this->tail!=this->head){
		this->length--;
		if(this->tail<this->bodys+this->maxlength-1)
			this->tail++;
		else
			this->tail=this->bodys;
	}
}

void Snaker_SetDir(Snaker*snaker,Dir dir){
	if(snaker->dir==Dir_Up&&dir==Dir_Down)return;
	if(snaker->dir==Dir_Down&&dir==Dir_Up)return;
	if(snaker->dir==Dir_Left&&dir==Dir_Right)return;
	if(snaker->dir==Dir_Right&&dir==Dir_Left)return;
	snaker->dir=dir;
}

void Snaker_Init(Snaker*this,Body*bodys,int maxlength,Map map){
	int i;
	srand(RTC_GetCnt());
	this->food->x=0;
	this->food->y=0;
	this->head=this->tail=this->bodys=bodys;
	this->head->x=15;
	this->head->y=5;
	this->maxlength=maxlength;
	this->map=map;
	this->dir=Dir_Down;
	this->length=1;
	for(i=0;i<5;i++)Snaker_Add(this);
	Snaker_newfood(this);
}

void Snaker_Show(Snaker*this){
	Body*body=this->tail;
	int i,j;
	for(i=0;i<COLS;i++){
		for(j=0;j<ROWS;j++){
			Dot(i,j,Snaker_Map(this->map,i,j));
		}
	}
	while(body!=this->head){
		Dot(body->x,body->y,1);
		body++;
		if(body-this->bodys>=this->maxlength){
			body=this->bodys;
		}
	}
	Dot(this->head->x,this->head->y,1);
	Dot(this->food->x,this->food->y,1);
	OLED_Draw_Refresh(Controls_OLED,Controls_Buff);
}

int Game_Snaker(uint8_t level,Map map){
	int times=1,score=0,speed=100/level+1;
	Snaker snaker[1];
	Body body[MAXLENGTH];
	Snaker_Init(snaker,body,MAXLENGTH,map);
	Snaker_Show(snaker);
	while(1){
		int i;
		times++;
		delay_ms(10);
		if(times%speed==0){
			switch(Snaker_Add(snaker)){
				case 2://too long
					score+=level;
				case 0://normal
					Snaker_Sub(snaker);
					break;
				case 1://long
					score+=level;
					Snaker_newfood(snaker);
					break;
				case -1://die
					return score;
				default:
					break;
			}
			Snaker_Show(snaker);
			times=1;
		}
		for(i=0;i<9;i++){
			if(Controls_Keys_Had[i]>0){
				Controls_Keys_Had[i]--;
				switch(i){
					case 0:
						return score;
					case 1:
						Snaker_SetDir(snaker,Dir_Left);
						break;
					case 3:
						Snaker_SetDir(snaker,Dir_Up);
						break;
					case 5:
						Snaker_SetDir(snaker,Dir_Down);
						break;
					case 7:
						Snaker_SetDir(snaker,Dir_Right);
						break;
					default:
						break;
				}
			}
		}
	}
}
