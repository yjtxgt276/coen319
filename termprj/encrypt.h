#ifndef ENCRYPT_H
#define ENCRYPT_H
#include "byteSub.h"
#include "addRoundKey.h"
#include "deviceLib.h"
#include "mixCol.h"
#include "shiftRow.h"
//#define BLOCKSIZE 512 
//#define GRIDSIZE 65535
__global__ void encrypt(uint8_t* d_buffer, uint8_t* rdk, off_t filesize);
#endif
