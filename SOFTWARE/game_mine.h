#ifndef __GAME_MINE_H__
#define __GAME_MINE_H__

#include <stdint.h>
#include "table.h"
#include "text.h"
#include "button.h"

typedef enum{
	Mine_Invalid,
	Mine_Ready,
	Mine_Selected,
	Mine_Hide,
	Mine_End,
}Mine_Status;

typedef enum{
	Pic_None,
	Pic_N0,
	Pic_N1,
	Pic_N2,
	Pic_N3,
	Pic_N4,
	Pic_N5,
	Pic_N6,
	Pic_N7,
	Pic_N8,
	Pic_Flag,
	Pic_Ques,
	Pic_Bomb,
	Pic_Bomb_Flag,
	Pic_Bomb_Ques,
	Pic_NoBomb,
}Pic_t;

typedef enum{
	MineUnit_Bomb=1<<0,
	MineUnit_Flag=1<<1,
	MineUnit_Ques=1<<2,
	MineUnit_Open=1<<3,
}MineUnit_t;

typedef uint8_t MineUnit;

typedef struct{
	uint8_t x;
	uint8_t y;
	uint8_t quanx;
	uint8_t quany;
	Mine_Status status;
	MineUnit memory[TABLE_CONTENT_ROWSMAX][TABLE_CONTENT_COLSMAX];
	Table table[1];
	uint16_t timen;
	Text time[1];
	uint16_t restn;
	Text rest[1];
	Button rstbtn[1];
}Mine;

int Game_Mine(uint8_t quanx,uint8_t quany,uint8_t quanmine);

#endif
