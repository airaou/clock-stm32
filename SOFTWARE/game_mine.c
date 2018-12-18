#include <stdio.h>
#include <stdlib.h>
#include "delay.h"
#include "rtc.h"
#include "controls.h"
#include "game_mine.h"

const char*Mine_Pic[]={"M"," ","1","2","3","4","5","6","7","8","\x80","?","*","\x81","*","x"};

void Mine_Close(Mine*this,uint8_t refresh){
	Table_Close(this->table,0);
	Text_Close(this->time,0);
	Text_Close(this->rest,0);
	Button_Close(this->rstbtn,0);
	if(refresh)OLED_Draw_Refresh(Controls_OLED,Controls_Buff);
}

void Mine_Show(Mine*this){
	Table_Show(this->table);
	Text_Show(this->time);
	Text_Show(this->rest);
	Button_Show(this->rstbtn);
}

void Mine_SetTime(Mine*this,int time){
	char str[4];
	sprintf(str,"%03d",time%1000);
	str[3]=0;
	Text_SetCaption(this->time,str);
}

void Mine_SetRest(Mine*this,int rest){
	char str[4];
	sprintf(str,"%03d",rest%1000);
	str[3]=0;
	Text_SetCaption(this->rest,str);
}

void Mine_Sec_Handle(int arg,void*obj){
	Mine*this=(Mine*)obj;
	this->timen++;
	if(this->timen>=1000)this->timen=0;
	Mine_SetTime(this,this->timen%1000);
}

void Mine_SetStatus(Mine*this,Mine_Status status){
	this->status=status;
	switch(status){
	case Mine_Invalid:
		setHandle(Controls_Sec_Handle[0],0,0,0);
		Table_SetStatus(this->table,Table_Status_Invalid);
		Text_SetStatus(this->time,Text_Status_Invalid);
		Text_SetStatus(this->rest,Text_Status_Invalid);
		Button_SetStatus(this->rstbtn,Button_Status_Invalid);
		break;
	case Mine_Ready:
		setHandle(Controls_Sec_Handle[0],Mine_Sec_Handle,0,this);
		Table_SetStatus(this->table,Table_Status_Ready);
		Text_SetStatus(this->time,Text_Status_Ready);
		Text_SetStatus(this->rest,Text_Status_Ready);
		Button_SetStatus(this->rstbtn,Button_Status_Ready);
		break;
	case Mine_Selected:
		setHandle(Controls_Sec_Handle[0],Mine_Sec_Handle,0,this);
		Table_SetStatus(this->table,Table_Status_Selected);
		Text_SetStatus(this->time,Text_Status_Ready);
		Text_SetStatus(this->rest,Text_Status_Ready);
		Button_SetStatus(this->rstbtn,Button_Status_Ready);
		break;
	case Mine_Hide:
		Mine_Close(this,1);
		break;
	case Mine_End:
		setHandle(Controls_Alr_Handle[0],0,0,0);
		break;
	default:
		break;
	}
}

int Mine_GetNumber(Mine*this,int8_t x,int8_t y){
	const int8_t offset[8][2]={{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
	int i,cnt=0;
	for(i=0;i<8;i++){
		int8_t a=x+offset[i][0];
		int8_t b=y+offset[i][1];
		if(a>=0&&b>=0&&a<this->quanx&&b<this->quany)
			if(this->memory[b][a]&MineUnit_Bomb)cnt++;
	}
	return cnt;
}

void Mine_SetMines(Mine*this,uint8_t quan){
	int i;
	if(quan>=this->quanx*this->quany)quan=this->quanx*this->quany-1;
	for(i=0;i<quan;i++){
		uint8_t x,y;
		do{
			x=rand()%this->quanx;
			y=rand()/this->quanx%this->quany;
		}while(this->memory[y][x]&MineUnit_Bomb);
		this->memory[y][x]=MineUnit_Bomb;
	}
	this->restn=quan;
	Mine_SetRest(this,this->restn);
}

int Mine_FinfishCheck(Mine*this){
	int i;
	for(i=0;i<this->quanx*this->quany;i++){
		if((this->memory[i/this->quanx][i%this->quanx]
			&(MineUnit_Open|MineUnit_Bomb))==0){
			return 0;
		}
	}
	return 1;
}

void Mine_printMines(Mine*this){
	int i;
	for(i=0;i<this->quanx*this->quany;i++){
		uint8_t b=i/this->quanx;
		uint8_t a=i%this->quanx;
		if(this->memory[b][a]&MineUnit_Bomb){
			Table_SetContent(this->table,a,b,
				Mine_Pic[(this->memory[b][a]&MineUnit_Flag)?
				Pic_Bomb_Flag:Pic_Bomb]);
		}else{
			if(this->memory[b][a]&MineUnit_Flag){
				Table_SetContent(this->table,a,b,Mine_Pic[Pic_NoBomb]);
			}
		}
	}
}

void Mine_Click(Mine*this){
	int x,y;
	if(this->status!=Mine_Selected)return;
	x=this->table->selectedx;
	y=this->table->selectedy;
	if(this->memory[y][x]&MineUnit_Bomb){
		Button_SetCaption(this->rstbtn,"\xff\x10\x03");
		Mine_printMines(this);
		Mine_SetStatus(this,Mine_End);
	}else{
		if((this->memory[y][x]&MineUnit_Flag)==0){
			Table_SetContent(this->table,x,y,Mine_Pic[Pic_N0+Mine_GetNumber(this,x,y)]);
			this->memory[y][x]|=MineUnit_Open;
			if(Mine_FinfishCheck(this)){
				Button_SetCaption(this->rstbtn,"\xff\x10\x04");
				Mine_printMines(this);
				Mine_SetStatus(this,Mine_End);
			}
		}
	}
}

void Mine_RightClick(Mine*this){
	uint8_t x=this->table->selectedx;
	uint8_t y=this->table->selectedy;
	if(!(this->memory[y][x]&MineUnit_Open)){
		if(this->memory[y][x]&MineUnit_Flag){
			this->memory[y][x]&=~MineUnit_Flag;
			Table_SetContent(this->table,x,y,Mine_Pic[Pic_None]);
			this->restn++;
		}else{
			this->memory[y][x]|=MineUnit_Flag;
			Table_SetContent(this->table,x,y,Mine_Pic[Pic_Flag]);
			this->restn--;
		}
	}
	Mine_SetRest(this,this->restn);
}

void Mine_Open(Mine*this){
	int i,j,change;
	const int8_t offset[8][2]={{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
	int8_t a,b;
	do{
		change=0;
		for(i=0;i<this->quanx*this->quany;i++){
			if((this->memory[i/this->quanx][i%this->quanx]&MineUnit_Open)==0){
				for(j=0;j<8;j++){
					a=i%this->quanx+offset[j][0];
					b=i/this->quanx+offset[j][1];
					if(a>=0&&b>=0&&a<this->quanx&&b<this->quany){
						if(this->table->content[b][a][0]==Mine_Pic[Pic_N0][0]){
							change=1;
							Table_SetSelected(this->table,i%this->quanx,i/this->quanx);
							Mine_Click(this);
						}
					}
				}
			}
		}
	}while(change);
}

void Mine_Init(Mine*this,uint8_t quanx,uint8_t quany){
	int i;
	
	this->quanx=quanx;
	this->quany=quany;
	this->x=0;
	this->y=0;
	this->restn=0;
	this->timen=0;
	this->status=Mine_Hide;
	
//	srand(RTC_GetCnt());
	setHandle(Controls_Alr_Handle[0],0,0,0);
	
	Table_Init(this->table);
	Table_SetPosition(this->table,this->x+25,this->y+0);
	Table_SetUnit(this->table,9,9);
	Table_SetQuan(this->table,quanx,quany);
	for(i=0;i<quanx*quany;i++){
		Table_SetContent(this->table,i%quanx,i/quanx,Mine_Pic[Pic_None]);
		this->memory[i/quanx][i%quanx]=0;
	}
	
	Text_Init(this->time);
	Mine_SetTime(this,this->timen);
	Text_SetPosition(this->time,this->x+1,this->y+7);
	
	Text_Init(this->rest);
	Text_SetPosition(this->rest,this->x+1,this->y+49);
	Mine_SetRest(this,0);
	
	Button_Init(this->rstbtn);
	Button_SetCaption(this->rstbtn,"\xff\x10\x01");
	Button_SetPosition(this->rstbtn,this->x+0,this->y+20);
	Button_SetSize(this->rstbtn,23,23);
}

void Mine_DeInit(Mine*this){
	int i;
	for(i=0;i<this->quanx*this->quany;i++){
		Table_SetContent(this->table,i%this->quanx,i/this->quanx,Mine_Pic[Pic_None]);
		this->memory[i/this->quanx][i%this->quanx]=0;
	}
	this->restn=0;
	this->timen=0;
	this->status=Mine_Ready;
	Mine_SetRest(this,0);
	Mine_SetTime(this,this->timen);
	Button_SetCaption(this->rstbtn,"\xff\x10\x02");
}

int Game_Mine(uint8_t quanx,uint8_t quany,uint8_t quanmine){
	int score=0;
	Mine mine[1];
	OLED_Draw_Scrfull(Controls_OLED,Controls_Buff,0x00);
	Mine_Init(mine,quanx,quany);
	Mine_SetMines(mine,quanmine);
	
	Mine_SetStatus(mine,Mine_Selected);
	while(1){
		int i;
		for(i=0;i<9;i++){
			if(Controls_Keys_Had[i]>0){
				Controls_Keys_Had[i]--;
				switch(i){
					case 6:
						if(mine->rstbtn->status!=Button_Status_Selected){
							Mine_RightClick(mine);
						}
						break;
					case 4:
						if(mine->rstbtn->status==Button_Status_Selected){
							Button_SetStatus(mine->rstbtn,Button_Status_Click);
							Mine_DeInit(mine);
							Mine_SetStatus(mine,Mine_Selected);
							Mine_SetMines(mine,quanmine);
						}else{
							Button_SetCaption(mine->rstbtn,"\xff\x10\x02");
							Mine_Click(mine);
							Mine_Open(mine);
							if(mine->status!=Mine_End){
								delay_ms(200);
								Button_SetCaption(mine->rstbtn,"\xff\x10\x01");
							}
						}
						break;
					case 1:
						if(0==Table_MovSelected(mine->table,Table_Dir_Left)){
							Table_SetStatus(mine->table,Table_Status_Ready);
							Button_SetStatus(mine->rstbtn,Button_Status_Selected);							
						}
						break;
					case 3:
						Table_MovSelected(mine->table,Table_Dir_Up);
						break;
					case 5:
						Table_MovSelected(mine->table,Table_Dir_Down);
						break;
					case 7:
						if(mine->rstbtn->status==Button_Status_Selected){
							Button_SetStatus(mine->rstbtn,Button_Status_Ready);
							Table_SetStatus(mine->table,Table_Status_Selected);
						}else{
							Table_MovSelected(mine->table,Table_Dir_Right);
						}
						break;
					case 0:
						setHandle(Controls_Sec_Handle[0],0,0,0);
						return score;
					default:
						break;
				}					
			}
		}
	}
}
