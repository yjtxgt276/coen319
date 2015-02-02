#ifndef SHIFTROW_H
#define SHIFTROW_H
#include "deviceLib.h"

__device__ void shiftRow(uint8_t* blockptr) ;
__device__ void invShiftRow(uint8_t* block);

#endif
