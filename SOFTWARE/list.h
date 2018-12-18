#ifndef __LIST_H__
#define __LIST_H__

#include <stdint.h>
#include "controls.h"

typedef enum
{
	List_Status_Invalid,
	List_Status_Ready,
	List_Status_Selected,
	List_Status_Hide,
}List_Status;

/*
	x 左上角横坐标(像素)
	y 左上角纵坐标(像素)
	sizex 水平宽度(像素)
	unity 每个条目的高度(像素)
	quan 条目数量(个)
	selected 在显示出来的条目中选中的第几个条目(个)
	string 字符串数组
	firstStr 在条目中第一个显示的字符串的下标
	strQuan 字符串数量
*/
#define LIST_STRING_TOTAL 16
typedef struct
{
	uint8_t x;
	uint8_t y;
	uint8_t sizex;
	uint8_t unity;
	uint8_t quan;
	uint8_t selected;
	const char*string[LIST_STRING_TOTAL];
	uint8_t firstStr;
	uint8_t strQuan;
	List_Status status;
}List;

void List_Init(List*list);
void List_SetPosition(List*list,uint8_t x,uint8_t y);
void List_SetSize(List*list,uint8_t sizex,uint8_t unity);
void List_SetString(List*list,uint8_t n,const char*str);
void List_SetSelected(List*list,uint8_t sel);
void List_SetFirst(List*list,uint8_t fstr);
void List_SetQuan(List*list,uint8_t quan);
void List_SetStrQuan(List*list,uint8_t strquan);
void List_SetStatus(List*list,List_Status status);
void List_Show(List*list);
void List_Close(List*list,uint8_t refresh);
void List_MovSelected(List*list,uint8_t dir);
uint8_t List_GetNum(List*list);

#endif
