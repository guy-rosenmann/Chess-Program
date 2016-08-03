#ifndef MINIMAX_H
#define MINIMAX_H

#include <stdlib.h>
#include "head.h"
#include "gameplay.h"

/* Minimax options */
#define SORT_MOVES_BY_VALUE 1
#define RAND_EQUAL_MOVES 1

/* Globals */
const int victimScore[13];
const int attackerScore[13];

/* Functions */
int compareMoveScore(const void *, const void *);
int returnScore(const Move *);
int mvvLva(const Move *);
int minimaxCheckWin(Board*, char);
minimaxMove alphaBeta(Board *,double, double, int, char);


#endif
