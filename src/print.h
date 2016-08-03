#ifndef PRINT_H
#define PRINT_H

#include <stdio.h>
#include "head.h"
#include "gamedata.h"
#include "init.h"

char *printSquare(int);
char *printMove(const Move);
void printMoveList(const MoveList *);
char *printRow(const Board *, int, char[]);

#endif
