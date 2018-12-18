#include "delay.h"
#include "oled12864.h"
#include "font.h"
#include "button.h"
#include "controls.h"
#include "menu_options.h"
#include "menu_calendar.h"
#include "menu_alr.h"
#include "menu_game.h"
#include "menu_other.h"
#include "calendar.h"
#include "alarm.h"
#include "music.h"
#include "tune.h"

void mainMenu()
{
    int i;
	OLED_TypeDef*OLED=Controls_OLED;
	OLED_Buff_TypeDef*buff=Controls_Buff;
    Button btns[5];
    Button text[1];
    const char*const btnname[]={"Cal","Alr","Gam","Opt","Oth"};
    const char*const optname[]={"\xff日历","\xff闹钟","\xff游戏","\xff设置","\xff其他"};
    Controls_OLED=0;
	for(i=0;i<9;i++)Controls_Keys_Had[i]=0;
    for(i=0;i<5;i++)
    {
        Button_Init(btns+i);
        Button_SetSize(btns+i,16,16);
        Button_SetPosition(btns+i,24*i,47);
        Button_SetCaption(btns+i,btnname[i]);
        Button_SetStatus(btns+i,Button_Status_Ready);
    }
    Button_Init(text);
    Button_SetSize(text,56,24);
    Button_SetPosition(text,64,12);
    Button_SetCaption(text,optname[0]);
    Button_SetStatus(text,Button_Status_Invalid);

    Button_SetStatus(btns,Button_Status_Selected);

    OLED_Draw_Picture32(Controls_OLED,Controls_Buff,8,8,icon32[0],0);
	Controls_OLED=OLED;
	OLED_Draw_Refresh(Controls_OLED,Controls_Buff);
    while(1)
    {
        int keynum,j;
        delay_ms(10);
		Alarm_Try();
		
        for(keynum=0;keynum<9;keynum++)
        {
            if(Controls_Keys_Had[keynum]>0)
            {
                Controls_Keys_Had[keynum]=0;
                switch(keynum)
                {
                case 1:
                    for(j=0;j<5;j++)
                    {
                        if(btns[j].status==Button_Status_Selected)
                        {
                            Button_SetStatus(btns+j,Button_Status_Ready);
							if(j==0)j=5-1;else j--;
                            Button_SetStatus(btns+j, Button_Status_Selected);
                            OLED_Draw_Picture32(Controls_OLED,Controls_Buff,8,8,icon32[j*8],0);
                            Button_SetCaption(text,optname[j]);
                            break;
                        }
                    }
					break;
                case 4:
                    for(j=0;j<5;j++)
                    {
                        if(btns[j].status==Button_Status_Selected)
                        {
							Button_SetStatus(btns+j,Button_Status_Click);
							switch(j)
							{
							case 0:
								Calendar_view();
								break;
							case 1:
								Alarm_view();
								break;
							case 2:
								Game_view();
								break;
							case 3:
								Options_view();
								break;
							case 4:
								Other_view();
								break;
							default:
								break;
							}
							Controls_OLED=0;
							Controls_Buff=buff;
							OLED_Draw_Scrfull(Controls_OLED,Controls_Buff,0x00);
							Button_Show(text);
							for(i=0;i<5;i++)
							{
								Button_Show(btns+i);
							}
							OLED_Draw_Picture32(Controls_OLED,Controls_Buff,8,8,icon32[j*8],0);
							Controls_OLED=OLED;
							OLED_Draw_Refresh(Controls_OLED,Controls_Buff);
							Button_SetStatus(btns+j,Button_Status_Selected);
                        }
                    }
                    break;
				case 7:
                    for(j=0;j<5;j++)
                    {
                        if(btns[j].status==Button_Status_Selected)
                        {
                            Button_SetStatus(btns+j,Button_Status_Ready);
							if(j>=4)j=0;else j++;
                            Button_SetStatus(btns+j,Button_Status_Selected);
                            OLED_Draw_Picture32(Controls_OLED,Controls_Buff,8,8,icon32[j*8],0);
                            Button_SetCaption(text, optname[j]);
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
}
