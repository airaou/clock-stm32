#ifndef __FONT_H__
#define __FONT_H__

#include <stdint.h>

typedef struct{
	uint8_t c1;
	uint8_t c2;
	uint8_t nc;
}char_cn_t;

extern const char char_ascii[][4];
extern const char char_cn[][16];
extern const char_cn_t char_cn_index[];
extern const char icon32[][16];
extern const char pic[][16];

#endif
