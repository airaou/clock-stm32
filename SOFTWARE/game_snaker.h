#ifndef __GAME_SNAKER_H__
#define __GAME_SNAKER_H__

#include <stdint.h>

#define ROWS 32
#define COLS 64
#define MAXLENGTH 1024

typedef enum{
	Map_None,
	Map_Besieged,
	Map_Tube,
}Map;

typedef enum{
	Dir_Up,
	Dir_Right,
	Dir_Down,
	Dir_Left,
}Dir;

typedef struct{
	uint8_t x;
	uint8_t y;
}Body;

typedef struct{
	Body*head;
	Body*tail;
	int length;
	Body*bodys;
	int maxlength;
	Dir dir;
	Map map;
	Body food[1];
}Snaker;

//·µ»Ø·ÖÊý
int Game_Snaker(uint8_t level,Map map);

#endif
