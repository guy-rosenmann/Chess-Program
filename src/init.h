#ifndef INIT_H
#define INIT_H

#include "head.h"

extern int bigToSmall[NUM_SQUARE];
extern int smallToBig[64];
extern int rows[NUM_SQUARE];
extern int cols[NUM_SQUARE];

void initRowAndCol();
void initConversionArrays();
extern void init();

#endif
