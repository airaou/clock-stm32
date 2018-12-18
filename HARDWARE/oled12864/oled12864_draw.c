#include "oled12864.h"
#include "oled12864_draw.h"
#include "font.h"

void OLED_Draw_Pic12864(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff,const char*dat)
{
	int i,n;
	for(i=0;i<8;i++)
	{
		OLED_Cmd(OLED,0xb0+i);
		OLED_Cmd(OLED,0x00);
		OLED_Cmd(OLED,0x10);
		for(n=0;n<128;n++)
		{
			const char d=*dat++;
			if(buff)buff->buff[i][n]=d;
			if(OLED)OLED_Dat(OLED,d);
		}
	}
}

void OLED_Draw_Scrfull(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff,uint8_t dat)
{
	int i,n;
	for(i=0;i<8;i++)
	{
		OLED_Cmd(OLED,0xb0+i);
		OLED_Cmd(OLED,0x00);
		OLED_Cmd(OLED,0x10);
		for(n=0;n<128;n++)
		{
			buff->buff[i][n]=dat;
			OLED_Dat(OLED,dat);
		}
	}
}

void OLED_Draw_Refresh(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff)
{
	int i,j;
	for(i=0;i<8;i++)
	{
		OLED_Cmd(OLED,0xB0+i);
		OLED_Cmd(OLED,0x00);
		OLED_Cmd(OLED,0x10);
		for(j=0;j<128;j++)
		{
			OLED_Dat(OLED,buff->buff[i][j]);
		}
	}
}

void OLED_Draw_RanByte(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff,uint8_t x,uint8_t y,uint8_t byte,uint8_t trans)
{
	if(y%8)
	{
		if(!trans)buff->buff[y/8][x]&=~(0xff<<(y%8));
		buff->buff[y/8][x]|=byte<<(y%8);
		OLED_Cmd(OLED,0xb0|(y/8));
		OLED_Cmd(OLED,0x00|(x&0x0f));
		OLED_Cmd(OLED,0x10|((x>>4)&0x0f));
		OLED_Dat(OLED,buff->buff[y/8][x]);
		if(y/8+1<8)
		{
			if(!trans)buff->buff[y/8+1][x]&=~(0xff>>(8-y%8));
			buff->buff[y/8+1][x]|=byte>>(8-y%8);
			OLED_Cmd(OLED,0xb0|(y/8+1));
			OLED_Cmd(OLED,0x00|(x&0x0f));
			OLED_Cmd(OLED,0x10|((x>>4)&0x0f));
			OLED_Dat(OLED,buff->buff[y/8+1][x]);
		}
	}
	else
	{
		if(trans)
			buff->buff[y/8][x]|=byte;
		else
			buff->buff[y/8][x]=byte;
		OLED_Cmd(OLED,0xb0|(y/8));
		OLED_Cmd(OLED,0x00|(x&0x0f));
		OLED_Cmd(OLED,0x10|((x>>4)&0x0f));
		OLED_Dat(OLED,buff->buff[y/8][x]);
	}
}

void OLED_Draw_RanBytes(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff,uint8_t weight,uint8_t x,uint8_t y,const char*arr,uint8_t trans)
{
	uint8_t j;
	if(y%8)
	{
		OLED_Cmd(OLED,0xb0|(y/8));
		OLED_Cmd(OLED,0x00|(x&0x0f));
		OLED_Cmd(OLED,0x10|((x>>4)&0x0f));
		for(j=0;j<weight&&(x+j)<128;j++)
		{
			if(!trans)buff->buff[y/8][x+j]&=~(0xff<<(y%8));
			buff->buff[y/8][x+j]|=arr[j]<<(y%8);
			OLED_Dat(OLED,buff->buff[y/8][x+j]);
		}
		if(y/8+1<8)
		{
			OLED_Cmd(OLED,0xb0|(y/8+1));
			OLED_Cmd(OLED,0x00|(x&0x0f));
			OLED_Cmd(OLED,0x10|((x>>4)&0x0f));
			for(j=0;j<weight&&(x+j)<128;j++)
			{
				if(!trans)buff->buff[y/8+1][x+j]&=~(0xff>>(8-y%8));
				buff->buff[y/8+1][x+j]|=arr[j]>>(8-y%8);
				OLED_Dat(OLED,buff->buff[y/8+1][x+j]);
			}
		}
	}
	else
	{
		OLED_Cmd(OLED,0xb0|(y/8));
		OLED_Cmd(OLED,0x00|(x&0x0f));
		OLED_Cmd(OLED,0x10|((x>>4)&0x0f));
		for(j=0;j<weight&&(x+j)<128;j++)
		{
			if(trans)
				buff->buff[y/8][x+j]|=arr[j];
			else
				buff->buff[y/8][x+j]=arr[j];
			OLED_Dat(OLED,buff->buff[y/8][x+j]);
		}
	}
}

void OLED_Draw_Dot(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff,uint8_t x,uint8_t y,uint8_t color)
{
	if(x>=128||y>=64)return;
	if(color)
	{
		buff->buff[y/8][x]|=1<<(y%8);
	}
	else
	{
		buff->buff[y/8][x]&=~(1<<(y%8));
	}
	if(OLED)
	{
		OLED_Cmd(OLED,0xb0|(y/8));
		OLED_Cmd(OLED,0x00|(x&0x0f));
		OLED_Cmd(OLED,0x10|((x>>4)&0x0f));
		OLED_Dat(OLED,buff->buff[y/8][x]);
	}
}

void OLED_Draw_putsCN(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff,uint8_t x,uint8_t y,const char*string,uint8_t trans)
{
	int i,j=y;
	if(!string)return;
	while((j+15)<64&&*string)
	{
		i=x;
		while((i+15)<128&&*string)
		{
			int k;
			uint8_t c1=*string++,c2;
			if((c2=*string++)==0)return;
			for(k=0;char_cn_index[k].c1;k++)
			{
				if(char_cn_index[k].c1==c1&&char_cn_index[k].c2==c2)
				{
					OLED_Draw_Picture16(OLED,buff,i,j,char_cn[char_cn_index[k].nc*2],trans);
					i+=16;
				}
			}
		}
		j+=16;
	}
}

void OLED_Draw_puts(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff,uint8_t x,uint8_t y,const char*string,uint8_t trans)
{
	int i,j=y;
	if(!string)return;
	while((j+8)<64&&*string)
	{
		i=x;
		while((i+4)<128&&*string)
		{
			if(*string==(char)-1)
			{
				string++;
				OLED_Draw_putsCN(OLED,buff,i,j,string,trans);
				return;
			}
			
			if(*string=='\n'||*string=='\r')
			{
				string++;
				break;
			}else{
				OLED_Draw_RanBytes(OLED,buff,4,i,j,char_ascii[*string],trans);
				if(!trans)if(i+5<128)if(string[1])OLED_Draw_RanByte(OLED,buff,i+5,j,0x00,trans);
				i+=5;
				string++;
			}
		}
		j+=9;
	}
}

void OLED_Draw_Number(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff,uint8_t x,uint8_t y,int32_t num,uint8_t trans)
{
	char str[12];
	int n,len=0;
	if(n==0)
	{
		str[0]='0';
		str[1]=0;
	}else{
		if(num<0)str[0]='-',len++,num=-num;
		n=num;
		while(n)len++,n/=10;
		str[len]=0;
		while(num)str[--len]=num%10+'0',num/=10;
	}
	OLED_Draw_puts(OLED,buff,x,y,str,trans);
}

void OLED_Draw_Picture16(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff,uint8_t x,uint8_t y,const char*pic,uint8_t trans)
{
	OLED_Draw_RanBytes(OLED,buff,16,x,y,pic,trans);
	if(y+8<64)
		OLED_Draw_RanBytes(OLED,buff,16,x,y+8,pic+16,trans);
}

void OLED_Draw_Picture32(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff,uint8_t x,uint8_t y,const char*pic,uint8_t trans)
{
	OLED_Draw_RanBytes(OLED,buff,32,x,y,pic,trans);
	if(y+8<64)
		OLED_Draw_RanBytes(OLED,buff,32,x,y+8,pic+32,trans);
	if(y+16<64)
		OLED_Draw_RanBytes(OLED,buff,32,x,y+16,pic+64,trans);
	if(y+24<64)
		OLED_Draw_RanBytes(OLED,buff,32,x,y+24,pic+96,trans);
}

void OLED_Draw_Line(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff,uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t color)
{
	unsigned int t;
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy;
	uint8_t colormark=0;
	unsigned int row,col;
	
	if(x0>=128||x1>=128||y0>=64||y1>=64)return;

	delta_x = x1-x0;
	delta_y = y1-y0;
	col = x0;
	row = y0;

	if(delta_x>0)
		incx=1;
	else
	{
		if(delta_x==0)
			incx=0;
		else
		{
			incx=-1;
			delta_x=-delta_x;
		}
	}

	if(delta_y>0)
		incy=1;
	else
	{
		if( delta_y==0)
			incy=0;
		else
		{
			incy=-1;
			delta_y=-delta_y;
		}
	}

	if(delta_x > delta_y)
		distance=delta_x;
	else
		distance=delta_y;
	
	for(t=0;t<=distance+1;t++)
	{
		switch(color)
		{
			case 0:
			case 1:
				OLED_Draw_Dot(OLED,buff,col,row,color);
				break;
			case 2:
				OLED_Draw_Dot(OLED,buff,col,row,colormark=!colormark);
				break;
			case 3:
				OLED_Draw_Dot(OLED,buff,col,row,!(colormark++%3));
			default:
				break;
		}
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			col+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			row+=incy;
		}
	}
}

void OLED_Draw_Square(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff,uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t color)
{
	uint8_t xmin,xmax,ymin,ymax,x,y;
	if(x0>=128||x1>=128||y0>=64||y1>=64)return;
	if(x1>x0)xmin=x0,xmax=x1;else xmin=x1,xmax=x0;
	if(y1>y0)ymin=y0,ymax=y1;else ymin=y1,ymax=y0;
    for(x=xmin;x<=xmax;x+=(color==2)?2:1)
    {
        OLED_Draw_Dot(OLED,buff,x,y0,color);
        OLED_Draw_Dot(OLED,buff,x,y1,color);
    }
    for(y=ymin;y<=ymax;y+=(color==2)?2:1)
    {
        OLED_Draw_Dot(OLED,buff,x0,y,color);
        OLED_Draw_Dot(OLED,buff,x1,y,color);
    }   
}

void OLED_Draw_Areafull(OLED_TypeDef*OLED,OLED_Buff_TypeDef*buff,uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t color)
{
    int x,y;
    if(x0>x1)
    {
        uint8_t tmp=x0;
        x0=x1;
        x1=tmp;
    }
    if(y0>y1)
    {
        uint8_t tmp=y0;
        y0=y1;
        y1=tmp;
    }
    for(x=x0;x<=x1;x++)
    {
        for(y=y0;y<=y1;y++)
        {
            if(color==2)
                OLED_Draw_Dot(OLED,buff,x,y,(x+y)&1);
            else
                OLED_Draw_Dot(OLED,buff,x,y,color);
        }
    }
}
