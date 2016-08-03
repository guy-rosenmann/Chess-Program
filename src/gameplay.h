#ifndef GMAEPLAY_H
#define GMAEPLAY_H

#include <stdio.h>
#include <string.h>
#include "head.h"
#include "gamedata.h"
#include "board.h"
#include "move.h"
#include "print.h"
#include "minimax.h"

enum {
    USER,
    COMP
};

void play(Board *);
void readSettingCmd(Board *, char *);
void readGameCmd(Board *, char *);
void makeComputerMove(Board *, char *);
int addNumMoves(const Board *, char);
int calcBestDepth(const Board *);
char *computerMove(Board *);
int checkWin(Board *);
int checkLegalPosition(char *, int);
void startConsoleGame();
void handleCheck(Board *, int, char *);

#endif
