#include <stdio.h>
#include "calendar.h"

#include "delay.h"
#include "usart.h"

#include "rtc.h"
#include "oled12864.h"
#include "oled12864_draw.h"
#include "font.h"

#include "progress.h"
#include "table.h"
#include "text.h"
#include "list.h"
#include "button.h"

#include "menu.h"
#include "controls.h"

OLED_IIC_TypeDef*volatile Controls_OLED=0;
OLED_Buff_TypeDef*volatile Controls_Buff=0;

Handle volatile Controls_Keys_Handle[9]={0};
Handle volatile Controls_Alr_Handle[1]={0};
Handle volatile Controls_Sec_Handle[1]={0};
int volatile Controls_Keys_Had[9];
int volatile Controls_Alr_Had[1];

int ControlsMain(OLED_TypeDef*OLED, OLED_Buff_TypeDef*buff)
{
    Controls_OLED=OLED;
    Controls_Buff=buff;
    OLED_Draw_Scrfull(Controls_OLED,Controls_Buff,0x00);
    mainMenu();
    return 0;
}
