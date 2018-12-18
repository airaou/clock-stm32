#ifndef __CONTROLS_H__
#define __CONTROLS_H__

#include "oled12864.h"
#include "oled12864_draw.h"


typedef struct
{
	void(*func)(int arg,void*obj);
	int arg;
	void*obj;
}Handle;

#define runHandle(__han) ((__han.func)?((__han).func((__han).arg,(__han).obj)):(void)0)
//#define runHandle(__han)
#define setHandle(__han,__func,__arg,__obj) ((__han).func=(__func),(__han).arg=(__arg),(__han).obj=(__obj))
#define cpyHandle(__hanfrom,__hanto) ((__hanfrom).func=(__hanto).func,(__hanfrom).arg=(__hanto).arg,(__hanfrom).obj=(__hanto).obj)

#include "button.h"
#include "list.h"
#include "text.h"
#include "progress.h"
#include "table.h"

extern OLED_IIC_TypeDef*volatile Controls_OLED;
extern OLED_Buff_TypeDef*volatile Controls_Buff;
extern Handle volatile Controls_Keys_Handle[];
extern int volatile Controls_Keys_Had[];
extern Handle volatile Controls_Alr_Handle[];
extern int volatile Controls_Alr_Had[];
extern Handle volatile Controls_Sec_Handle[];

int ControlsMain(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff);

#endif
