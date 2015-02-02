#ifndef KEYSCHEDULE_H
#define KEYSCHEDULE_H
//#include <stdint.h>
#include "myAESlib.h"
static uint8_t roundconst[11]={
//0	1    2     3    4	5    6	   7     8       9   a	
0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};
void expandKey(void* k, uint8_t rk[Nrd+1][Nrow][Ncol]);
void transform(uint8_t rki[Nrow][Ncol], uint8_t* temp,int round);

#endif
