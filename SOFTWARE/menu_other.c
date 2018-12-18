#include <stdio.h>
#include "delay.h"
#include "dht11.h"
#include "controls.h"
#include "menu_other.h"
#include "music_player.h"

void Other_dht11_view(){
	int cnt=0;
	Text txt[1];
	DHT11 dht11[1];
	DHT11_Data dat[1];
	OLED_Draw_Scrfull(Controls_OLED,Controls_Buff,0x00);
	
	Text_Init(txt);
	Text_SetPosition(txt,10,10);
	Text_SetStatus(txt,Text_Status_Ready);
	Text_SetCaption(txt,"DHT11 Init...");
	
	dht11->DATA_GPIO=GPIOB;
	dht11->DATA_Pin=GPIO_Pin_11;
	DHT11_Init(dht11);
	
	Text_SetCaption(txt,"DHT11 OK");
	
	while(1){
		int i;
		cnt++;
		delay_ms(100);
		if(cnt>=50){
			char str[32];
			cnt=0;
			DHT11_Get(dht11,dat);
			sprintf(str,"Temp:%d,Humi:%d",dat->TemperatureInteger,dat->HumidityInteger);
			Text_SetCaption(txt,str);
		}
		for(i=0;i<9;i++){
			if(Controls_Keys_Had[i]>0){
				Controls_Keys_Had[i]--;
				switch(i){
					case 0:
						return;
					default:
						break;					
				}
			}
		}
	}
}

void Other_view()
{
	int i;
	const char*strname[]={"DHT11","music"};
	List list[1];

	OLED_Draw_Scrfull(Controls_OLED,Controls_Buff,0x00);
	
	List_Init(list);
	List_SetPosition(list,0,0);
	List_SetSize(list,50,9);
	List_SetQuan(list,6);
	for(i=0;i<2;i++)
	{
		List_SetString(list,i,strname[i]);
	}
	List_SetStrQuan(list,2);
	
	List_SetStatus(list,List_Status_Selected);
	while(1)
	{
		int i;
		for(i=0;i<9;i++){
			if(Controls_Keys_Had[i]>0){
				Controls_Keys_Had[i]--;
				switch(i){
					case 0:
						return;
					case 3:
						List_MovSelected(list,0);
						break;
					case 5:
						List_MovSelected(list,1);
						break;
					case 4:
						switch(List_GetNum(list)){
							case 0:
								Other_dht11_view();
								break;
							case 1:
								MusicPlayer();
								break;
							default:
								break;
						}
						OLED_Draw_Scrfull(Controls_OLED,Controls_Buff,0x00);
						List_Show(list);
					default:
						break;
				}
			}
		}
	}
}
