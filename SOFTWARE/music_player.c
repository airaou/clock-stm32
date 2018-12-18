#include "tune.h"
#include "music.h"
#include "controls.h"
#include "music_player.h"

#define MUSICPOSISMAX 11
const uint8_t musicposis[][2]={
	{0,0},
	{4,4},
	{8,0},
	{12,4},
	{16,0},
	{20,4},
	{24,0},
	{28,4},
	{32,0},
	{36,4},
	{40,0},
};

volatile uint8_t posi1=0;
volatile uint8_t posi2=1;
Text musicpic[2];

const char*musicname2[]=
{
	"Girl",
	"Same",
	"Two",
	"Heart",
	"Birth",
	0
};

const char*GetMusicString(const uint8_t*tune){
	int i;
	for(i=0;Musics[i];i++){
		if(Musics[i]==tune){
			return musicname2[i];
		}
	}
	return 0;
}

void MusicPlayer_Music_Sec_Handle(int arg,void*obj)
{
	if(posi1<MUSICPOSISMAX-1){
		posi1++;
	}else{
		posi1=0;
	}
	Text_SetPosition(musicpic+0,musicposis[posi1][0],musicposis[posi1][1]);
	if(posi2<MUSICPOSISMAX-1){
		posi2++;
	}else{
		posi2=0;
	}
	Text_SetPosition(musicpic+1,musicposis[posi2][0],musicposis[posi2][1]);
}

void SetMusic(uint8_t musicnum,const uint8_t**tune,const uint8_t**process,const uint8_t**end){
	const uint8_t*p;
	Music_Stop();
	posi1=0;
	posi2=4;
	setHandle(Controls_Sec_Handle[0],0,0,0);
	p=*process=*tune=Musics[musicnum];
	while(*p){
		p++;
	}
	*end=p;
	Music_Play(*tune,0,2,60);
	Music_Stop();
}

void MusicPlayer()
{
	int i,j;
	OLED_TypeDef*oled=Controls_OLED;
	uint8_t tunenum=0;
	const uint8_t*tune;
	const uint8_t*process;
	const uint8_t*end;
	uint8_t playing=0;
	Button ctrl[1];
	Button next[1];
	Button prev[1];
	Button*btns[3];
	Progress pgs[1];
	Text name[1];
	Controls_OLED=0;
	
	tunenum=0;
	SetMusic(tunenum,&tune,&process,&end);
	
	posi1=0;
	posi2=4;
	
	btns[0]=prev;
	btns[1]=ctrl;
	btns[2]=next;
	
	Progress_Init(pgs);
	Progress_SetPosition(pgs,3,32);
	Progress_SetSize(pgs,70,2);
	Progress_SetNumber(pgs,0);
	
	Text_Init(musicpic+0);
	Text_SetCaption(musicpic+0,"\x0d");
	
	Text_Init(musicpic+1);
	Text_SetCaption(musicpic+1,"\x0e");
	
	Text_Init(name);
	Text_SetPosition(name,64,16);
	Text_SetCaption(name,GetMusicString(tune));
	
	Button_Init(ctrl);
	Button_SetPosition(ctrl,51,39);
	Button_SetSize(ctrl,24,24);
	Button_SetCaption(ctrl,"\xff\x11\x03");
	
	Button_Init(next);
	Button_SetPosition(next,103,39);
	Button_SetSize(next,24,24);
	Button_SetCaption(next,"\xff\x11\x04");
	
	Button_Init(prev);
	Button_SetPosition(prev,0,39);
	Button_SetSize(prev,24,24);
	Button_SetCaption(prev,"\xff\x11\x05");
	
	OLED_Draw_Scrfull(Controls_OLED,Controls_Buff,0x00);
	Button_SetStatus(ctrl,Button_Status_Selected);
	Button_SetStatus(next,Button_Status_Ready);
	Button_SetStatus(prev,Button_Status_Ready);
	Progress_SetStatus(pgs,Progress_Status_Ready);
	Text_SetStatus(musicpic+0,Text_Status_Ready);
	Text_SetStatus(musicpic+1,Text_Status_Ready);
	Text_SetStatus(name,Text_Status_Ready);
	Controls_OLED=oled;
	OLED_Draw_Refresh(Controls_OLED,Controls_Buff);
	setHandle(Controls_Sec_Handle[0],MusicPlayer_Music_Sec_Handle,0,0);
	while(1)
	{
		for(i=0;i<9;i++)
		{
			if(Controls_Keys_Had[i]>0)
			{
				Controls_Keys_Had[i]--;
				switch(i)
				{
					case 4:
						for(j=0;j<3;j++){
							if(btns[j]->status==Button_Status_Selected){
								switch(j){
									case 0:
										if(tunenum>0){
											tunenum--;
										}else{
											tunenum=5-1;
										}
										SetMusic(tunenum,&tune,&process,&end);
										Button_SetCaption(ctrl,"\xff\x11\x03");
										Text_SetCaption(name,GetMusicString(tune));
										break;
									case 1://ctrl
										if(playing){
											setHandle(Controls_Sec_Handle[0],0,0,0);
											Music_Stop();
											playing=0;
											Button_SetCaption(ctrl,"\xff\x11\x03");
										}else{
											setHandle(Controls_Sec_Handle[0],MusicPlayer_Music_Sec_Handle,0,0);
											Music_Continue();
											playing=1;
											Button_SetCaption(ctrl,"\xff\x11\x02");
										}
										break;
									case 2:
										if(tunenum<5-1){
											tunenum++;
										}else{
											tunenum=0;
										}
										SetMusic(tunenum,&tune,&process,&end);
										Button_SetCaption(ctrl,"\xff\x11\x03");
										Text_SetCaption(name,GetMusicString(tune));
										break;
									default:
										break;
								}
							}
						}
						break;
					case 1:
						for(j=0;j<3;j++){
							if(btns[j]->status==Button_Status_Selected){
								Button_SetStatus(btns[j],Button_Status_Ready);
								if(j==0){
									Button_SetStatus(btns[3-1],Button_Status_Selected);
								}else{
									Button_SetStatus(btns[j-1],Button_Status_Selected);
								}
								break;
							}
						}
						break;
					case 7:
						for(j=0;j<3;j++){
							if(btns[j]->status==Button_Status_Selected){
								Button_SetStatus(btns[j],Button_Status_Ready);
								if(j==3-1){
									Button_SetStatus(btns[0],Button_Status_Selected);
								}else{
									Button_SetStatus(btns[j+1],Button_Status_Selected);
								}
								break;
							}
						}
						break;
					case 0:
						setHandle(Controls_Sec_Handle[0],0,0,0);
						return;
					default:
						break;
				}
			}
		}
	}
}
