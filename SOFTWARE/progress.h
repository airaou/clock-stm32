#ifndef __PROGRESS_H__
#define __PROGRESS_H__

#include <stdint.h>
#include "controls.h"

typedef enum
{
	Progress_Status_Invalid,
	Progress_Status_Ready,
	Progress_Status_Hide,
}Progress_Status;

typedef enum
{
	Progress_Dir_Horizontal,
	Progress_Dir_Vertical,
}Progress_Dir;

typedef struct
{
	uint8_t x;
	uint8_t y;
	Progress_Dir dir;
	uint8_t len;
	uint8_t wid;
	unsigned num;
	Progress_Status status;
}Progress;
	
void Progress_Init			(Progress*pgs);
void Progress_Show			(Progress*pgs,uint8_t opt);
void Progress_SetPosition	(Progress*pgs,uint8_t x,uint8_t y);
void Progress_SetDir		(Progress*pgs,Progress_Dir dir);
void Progress_SetSize		(Progress*pgs,uint8_t len,uint8_t wid);
void Progress_SetNumber		(Progress*pgs,uint8_t num);
void Progress_SetStatus		(Progress*pgs,Progress_Status status);
void Progress_Close			(Progress*pgs);

#endif
