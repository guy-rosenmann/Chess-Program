#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <string.h>
#include "head.h"
#include "gamedata.h"
#include "init.h"
#include "print.h"
#include "gameplay.h"


void resetBoard(Board *brd);
void printBoard(const Board *brd) ;
double evalBoardScore(Board *brd);
int saveBoard(const Board *, const char *);
int loadBoard(Board *, const char *);

#endif

