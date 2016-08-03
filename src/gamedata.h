#ifndef GAMEDATA_H
#define GAMEDATA_H

#include "head.h"
#include <stdio.h>

const char pceChar[13];
const char sideChar[2];
const char rowChar[8];
const char colChar[8];

const int pieceVal[13];
const int pieceCol[13];
const int pieceMoves[13];

const int directionsOfPieces[13][8];
const int contPieces[6];
const int discPieces[4];
const int numOfDirections[13];

const double pawnTable[64];
const double rookTable[64];
const double knightTable[64];
const double bishopTable[64];

int pieceFromChar(char);

#endif
