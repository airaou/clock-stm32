#ifndef __TEXT_H__
#define __TEXT_H__

#include <stdint.h>
#include "controls.h"

typedef enum
{
	Text_Status_Invalid,
	Text_Status_Ready,
	Text_Status_Hide,
}Text_Status;

typedef struct
{
	uint8_t x;
	uint8_t y;
	Text_Status status;
	char caption[32];
	uint8_t len;
}Text;

void Text_Init        (Text*txt);
void Text_Show        (Text*txt);
void Text_SetPosition (Text*txt,uint8_t x,uint8_t y);
void Text_SetCaption  (Text*txt,const char*str);
void Text_Close       (Text*txt,uint8_t refresh);
void Text_SetStatus   (Text*txt,Text_Status status);

#endif
