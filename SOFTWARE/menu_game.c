#include <stdio.h>
#include "controls.h"
#include "game_snaker.h"
#include "game_mine.h"
#include "menu_game.h"

const char*gamename[]=
{
	"Snaker",
	"Mine",
};

void Game_view()
{
	int i;
	List list[1];
	OLED_TypeDef*oled=Controls_OLED;
	List_Init(list);
	List_SetPosition(list,0,0);
	List_SetSize(list,50,9);
	List_SetQuan(list,6);
	for(i=0;i<2;i++)
	{
		List_SetString(list,i,gamename[i]);
	}
	List_SetStrQuan(list,2);
	
	Controls_OLED=0;
	List_SetStatus(list,List_Status_Invalid);
	List_SetStatus(list,List_Status_Selected);
	Controls_OLED=oled;
	OLED_Draw_Refresh(Controls_OLED,Controls_Buff);
	
	while(1)
	{
		for(i=0;i<9;i++)
		{
			if(Controls_Keys_Had[i]>0)
			{
				int score=0;
				char scorestr[12];
				Controls_Keys_Had[i]--;
				switch(i)
				{
					case 0:
						return;
					case 3:
						List_MovSelected(list,0);
						break;
					case 5:
						List_MovSelected(list,1);
						break;
					case 4:
						switch(List_GetNum(list))
						{
							case 0:
								score=Game_Snaker(10,Map_Besieged);
								break;
							case 1:
								score=Game_Mine(10,6,15);
								break;
							default:
								break;
						}
						Controls_OLED=0;
						OLED_Draw_Scrfull(Controls_OLED,Controls_Buff,0x00);
						List_SetStatus(list,List_Status_Selected);
						sprintf(scorestr,"SCORE:%d   ",score);
						OLED_Draw_puts(Controls_OLED,Controls_Buff,64,8,scorestr,0);
						Controls_OLED=oled;
						OLED_Draw_Refresh(Controls_OLED,Controls_Buff);
						break;
					default:
						break;
				}
			}
		}
	}
}
