#ifndef __TABLE_H__
#define __TABLE_H__

#include <stdint.h>
#include "controls.h"

typedef enum
{
	Table_Dir_Up,
	Table_Dir_Left,
	Table_Dir_Down,
	Table_Dir_Right,
}Table_Dir;

typedef enum
{
	Table_Status_Invalid,
	Table_Status_Ready,
	Table_Status_Selected,
	Table_Status_Hide,
}Table_Status;

#define TABLE_CONTENT_COLSMAX 10
#define TABLE_CONTENT_ROWSMAX 6

typedef struct
{
	uint8_t x;
	uint8_t y;
	uint8_t unitx;
	uint8_t unity;
	uint8_t quanx;
	uint8_t quany;
	const char*content[TABLE_CONTENT_ROWSMAX][TABLE_CONTENT_COLSMAX];
	uint8_t selectedx;
	uint8_t selectedy;
	Table_Status status;
}Table;

void Table_Init			(Table*tbl);
void Table_SetPosition	(Table*tbl,uint8_t x,uint8_t y);
void Table_SetUnit		(Table*tbl,uint8_t unitx,uint8_t unity);
void Table_SetQuan		(Table*tbl,uint8_t quanx,uint8_t quany);
void Table_SetContent	(Table*tbl,uint8_t x,uint8_t y,const char*str);
void Table_SetSelected	(Table*tbl,uint8_t x,uint8_t y);
void Table_SetStatus	(Table*tbl,Table_Status status);
void Table_ClrContent	(Table*tbl);
void Table_Close		(Table*tbl,uint8_t refresh);
void Table_Show			(Table*tbl);
int  Table_MovSelected	(Table*tbl,Table_Dir dir);

#endif
