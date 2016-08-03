#ifndef MOVE_H
#define MOVE_H

#include "head.h"
#include <stdio.h>
#include "gamedata.h"
#include "board.h"

void makeMoveList(Board *, MoveList *);
int makeMove(Board *, Move);
void cancelMove(Board *);
Move convertToMove(Board *, char *);
Move createMove(Board *,int, int, int, int, char);
int isSquareThreat(const Board *, const int, const int);
int movesEqual(const Move, const Move);
int kingExposed(Board *, const Move);
int legalMove(Board *, const Move, const MoveList *);
void addMove(Board *, int, int, int, int, char, MoveList *);
void validateList(Board *, MoveList *);
void getPawnMoves(Board *, MoveList *, int);
void getCastlingMoves(Board *, MoveList *, int);
void getContMoves(Board *, MoveList *, int);
void getDiscMoves(Board *, MoveList *, int);
void getMoves(Board *, MoveList *, int);
void makeMoveList(Board *, MoveList *);
void removePiece(Board *, int);
void addPiece(Board *, int, int);
void movePiece(Board *, int, int);
void updateCastling(Board *, Move);
void cancelMove(Board *);
Move createPlayerMoveGUI(Board *, int , int);

#endif
