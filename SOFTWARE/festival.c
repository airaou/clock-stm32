#include <stdio.h>
#include "calendar.h"
#include "festival.h"

#define fest_putchar(__cout,__dat) do{*(__cout)++=(__dat);}while(0)
void fest_puts(char**cout,const char*dat){
	while(*dat){
		**cout=*dat++;
		(*cout)++;
	}
}

void Festival_PrintOldCalendar(char*cout,OldCalendar*oc){
	const char*tiangan="甲乙丙丁戊己庚辛壬癸";
	const char*dizhi= "子丑寅卯辰巳午未申酉戌亥";
	const char*animal="鼠牛虎兔龙蛇马羊猴鸡狗猪";
	const char*number="零一二三四五六七八九十";
	uint16_t y=oc->year+36;
	fest_putchar(cout,tiangan[y%10*2+0]);
	fest_putchar(cout,tiangan[y%10*2+1]);
	fest_putchar(cout,dizhi[y%12*2+0]);
	fest_putchar(cout,dizhi[y%12*2+1]);
	fest_putchar(cout,animal[y%12*2+0]);
	fest_putchar(cout,animal[y%12*2+1]);
	fest_puts(&cout,"年");
	if(oc->mon>12){
		fest_puts(&cout,"闰");
		oc->mon-=12;
	}
	if(oc->mon==1){
		fest_puts(&cout,"正");
	}else if(oc->mon>=2&&oc->mon<=10){
		fest_putchar(cout,number[oc->mon*2]);
		fest_putchar(cout,number[oc->mon*2+1]);
	}else if(oc->mon==11){
		fest_puts(&cout,"冬");
	}else if(oc->mon==12){
		fest_puts(&cout,"腊");
	}
	fest_puts(&cout,"月");
	if(oc->mday>=1&&oc->mday<=10){
		fest_puts(&cout,"初");
		fest_putchar(cout,number[oc->mday*2]);
		fest_putchar(cout,number[oc->mday*2+1]);
	}else if(oc->mday>=11&&oc->mday<=19){
		fest_puts(&cout,"十");
		fest_putchar(cout,number[(oc->mday-10)*2]);
		fest_putchar(cout,number[(oc->mday-10)*2+1]);
	}else if(oc->mday==20){
		fest_puts(&cout,"二十");
	}else if(oc->mday>=21&&oc->mday<=29){
		fest_puts(&cout,"廿");
		fest_putchar(cout,number[(oc->mday-20)*2]);
		fest_putchar(cout,number[(oc->mday-20)*2+1]);
	}else if(oc->mday==30){
		fest_puts(&cout,"三十");
	}
	*cout=0;
}

typedef struct{
	const char*name;
	uint8_t mon;
	uint8_t mday;
}NewAnnualFestival;

typedef struct{
	const char*name;
	uint8_t mon;
	uint8_t mday;
}OldAnnualFestival;

typedef struct{
	const char**name;
	int (*judge)(NewCalendar*nc,OldCalendar*oc);
}AttributeFestival;

typedef struct{
	const char*name;
	uint8_t wday;
	uint8_t order;
	uint8_t mon;
}WeekbasedFestival;

typedef struct{
	const char*name;
	int (*judge)(NewCalendar*nc,OldCalendar*oc);
}SpecialFestival;

const NewAnnualFestival nafes[]={
	{"元旦",1,1},
	{"情人节",2,14},
	{"妇女节",3,8},
	{"植树节",3,12},
	{"消费者权益日",3,15},
	{"愚人节",4,1},
	{"世界地球日",4,22},
	{"劳动节",5,1},
	{"青年节",5,4},
	{"护士节",5,12},
	{"无烟日",5,31},
	{"儿童节",6,1},
	{"禁毒日",6,26},
	{"建党节",7,1},
	{"建军节",8,1},
	{"抗日胜利日",8,8},
	{"教师节",9,10},
	{"九一八事变",9,18},
	{"孔子诞辰",9,28},
	{"国庆节",10,1},
	{"双十一",11,11},
	{"平安夜",12,24},
	{"圣诞节",12,25},
	{0},
};

const OldAnnualFestival oafes[]={
	{"春节",1,1},
	{"元宵",1,15},
	{"端午",5,5},
	{"七夕",7,7},
	{"中元",7,15},
	{"中秋",8,15},
	{"重阳",9,9},
	{"腊八",12,8},
	{"扫房",12,23},
	{"掸尘",12,24},
	{0},
};

const char*NameWeek[7]={
	"星期日","星期一","星期二","星期三","星期四","星期五","星期六"
};

int GetWeek(NewCalendar*nc,OldCalendar*oc){
	return nc->wday;
}

const char*NameConstellation[12]={
	"白羊座","金牛座","双子座",
	"巨蟹座","狮子座","处女座",
	"天秤座","天蝎座","射手座",
	"摩羯座","水瓶座","双鱼座",
};

int GetConstellation(NewCalendar*nc,OldCalendar*oc){
	uint16_t yd;
	yd=nc->yday+((nc->year%(nc->year%100?4:400))?(nc->yday>59):0);
	if(yd>356)return 9;
	if(yd>327)return 8;
	if(yd>297)return 7;
	if(yd>266)return 6;
	if(yd>235)return 5;
	if(yd>204)return 4;
	if(yd>173)return 3;
	if(yd>141)return 2;
	if(yd>110)return 1;
	if(yd>80) return 0;
	if(yd>49) return 11;
	if(yd>19) return 10;
	if(yd>0)  return 9;
	return -1;
}

AttributeFestival atfes[]={
	{NameConstellation,GetConstellation},
	{NameWeek,GetWeek},
	{0},
};

WeekbasedFestival wbfes[]={
  {"母亲节",0,2,5},
  {"父亲节",0,3,6},
  {"感恩节",1,2,10},
  {0},
};

//事实上闰十二月几乎不可能出现
int isEve(NewCalendar*nc,OldCalendar*oc){
	if(oc->mon!=24&&oc->mon!=12)return 0;
	if((OldCalendar_Define[oc->year]&0xf)==12){
		if(oc->mon==24&&oc->mday==(((OldCalendar_Define[oc->year]>>16)&1)?30:29))return 1;
		return 0;
	}else{
		if(oc->mon==12&&oc->mday==(((OldCalendar_Define[oc->year]>>4)&1)?30:29))return 1;
		return 0;
	}
}

SpecialFestival spfes[]={
	{"除夕",isEve},
	{0},
};

void Festival_PrintFestival(char*cout,NewCalendar*nc,OldCalendar*oc){
	int i;
	for(i=0;atfes[i].judge;i++){
		fest_puts(&cout,atfes[i].name[atfes[i].judge(nc,oc)]);
	}
	for(i=0;nafes[i].name;i++){
		if(nc->mon==nafes[i].mon&&nc->mday==nafes[i].mday)fest_puts(&cout,nafes[i].name);
	}
	for(i=0;oafes[i].name;i++){
		if(oc->mon==oafes[i].mon&&oc->mday==oafes[i].mday)fest_puts(&cout,oafes[i].name);
	}
	for(i=0;wbfes[i].name;i++){
		if(wbfes[i].mon==nc->mon&&wbfes[i].wday==nc->wday&&wbfes[i].order-1==(nc->mday-1)/7)
			fest_puts(&cout,wbfes[i].name);
	}
	for(i=0;spfes[i].name;i++){
		if(spfes[i].judge(nc,oc))fest_puts(&cout,spfes[i].name);
	}
	*cout=0;
}
