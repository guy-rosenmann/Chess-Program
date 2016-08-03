#include "gamedata.h"

const char pceChar[] = " mrnbqkMRNBQK"; /* How the pieces appear on the board */
const char sideChar[] = "wb"; /* The two sides */
const char rowChar[] = "12345678"; /* Rank numbers */
const char colChar[] = "abcdefgh"; /*File letters */

const int pieceVal[13]= { 0, 1, 5, 3, 3, 9, 400, 1, 5, 3, 3, 9, 400 }; /* The score of every piece */
const int pieceCol[13] = { NONE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, /* The color of each piece */
	BLACK, BLACK, BLACK, BLACK, BLACK, BLACK };
const int pieceMoves[13] = { 0 , 6, 14, 8, 14, 28, 8, 6, 14, 8, 14, 28, 8} ; /* The max numbers of moves a piece can make */

const int directionsOfPieces[13][8] = { /* The directions the pieces can go to (pawns not included) */
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, 0, 0, 0, 0 },
	{ -8, -19,	-21, -12, 8, 19, 21, 12 },
	{ -9, -11, 11, 9, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, 0, 0, 0, 0 },
	{ -8, -19,	-21, -12, 8, 19, 21, 12 },
	{ -9, -11, 11, 9, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
};

const int contPieces[6] = { /* Pieces with continuous movement */
 wBishop, wRook, wQueen, bBishop, bRook, bQueen
};

const int discPieces[4] = { /* Pieces with discrete movement */
 wKnight, wKing, bKnight, bKing
};

const int numOfDirections[13] = { /* How many directions a piece has (no pawns) */
 0, 0, 4, 8, 4, 8, 8, 0, 4, 8, 4, 8, 8
};

const double pawnTable[64] = { /* Value of pawns based on their positions */
0,0,0,0,0,0,0,0,
0.2,0.2,0.2,0.3,0.3,0.2,0.2,0.2,
0.1,0.1,0.1,0.2,0.2,0.1,0.1,0.1,
0.05,0.05,0.05,0.1,0.1,0.05,0.05,0.05,
0,0,0.1,0.2,0.2,0.1,0,0,
0.05,0,0,0.11,0.11,0,0,0.05,
0.1,0.1,0,-0.1,-0.1,0,0.1,0.1,
0,0,0,0,0,0,0,0
};

const double knightTable[64] = { /* Value of knights based on their positions */
0,0,0,0,0,0,0,0,
0,0,0.046,0.092,0.092,0.046,0,0,
0.046,0.092,0.092,0.18,0.18,0.092,0.092,0.046,
0.046,0.092,0.14,0.18,0.18,0.14,0.092,0.046,
0,0,0.092,0.18,0.18,0.092,0.046,0,
0,0,0.092,0.092,0.092,0.092,0,0,
0,0,0,0.046,0.046,0,0,0,
0,-0.092,0,0,0,0,-0.092,0
};

const double bishopTable[64] = { /* Value of bishops based on their positions */
0,0,0,0,0,0,0,0,
0,0,0,0.092,0.092,0,0,0,
0,0,0.092,0.14,0.14,0.092,0,0,
0,0.092,0.14,0.18,0.18,0.14,0.092,0,
0,0.092,0.14,0.18,0.18,0.14,0.092,0,
0,0,0.092,0.14,0.14,0.092,0,0,
0,0,0,0.092,0.092,0,0,0,
0,0,-0.092,0,0,-0.092,0,0,
};

const double rookTable[64] = { /* Value of rooks based on their positions */
0,0,0.045,0.091,0.091,0.045,0,0,
0.23,0.23,0.23,0.23,0.23,0.23,0.23,0.23,
0,0,0.045,0.091,0.091,0.045,0,0,
0,0,0.045,0.091,0.091,0.045,0,0,
0,0,0.045,0.091,0.091,0.045,0,0,
0,0,0.045,0.091,0.091,0.045,0,0,
0,0,0.045,0.091,0.091,0.045,0,0,
0,0,0.045,0.091,0.091,0.045,0,0,
};

/* Converts a character representing a piece into that piece's number */
int pieceFromChar(char p) {

    int i;

    for(i = 1; i <= 13; i++) {
        if (pceChar[i] == p) return i;
    }

    return 0;

}





