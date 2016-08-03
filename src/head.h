#ifndef HEAD_H
#define HEAD_H

#include "constants.h"

/* Constants */
#define NUM_SQUARE 120
#define MAX_GAME_MOVES 2048
#define MAX_MOVES_PER_TURN 256
#define MAX_COMMAND_LENGTH 100
#define DEFAULT_DIFFICULTY 1
#define DEFAULT_USER_COLOUR WHITE
#define DEFAULT_GAMEMODE 0
#define MAX_SCORE 10000.0
#define MAX_MOVES_IN_MINIMAX 1000000
#define NUMBER_OF_DIFFICULTIES 4
#define NO_CHECK 0
#define CHECK 1
#define CHECK_MATE 2

/* Messages */
#define WRONG_MINIMAX_DEPTH "Wrong value for minimax depth. Value should be between 1 to 4\n"
#define WRONG_POSITION "Invalid position on the board\n"
#define NO_PIECE "The specified position does not contain your piece\n"
#define NO_ROOK "Wrong position for a rook\n"
#define ILLEGAL_COMMAND "Illegal command, please try again\n"
#define ILLEGAL_MOVE "Illegal move\n"
#define ILLEGAL_CASTLE "Illegal castling move\n"
#define WRONG_GAME_MODE "Wrong game mode\n"
#define CHECK_MSG "Check!\n"
#define BLACK_WINS "Mate! Black player wins the game\n"
#define WHITE_WINS "Mate! White player wins the game\n"
#define TWO_PLAYER "Running game in 2 players mode\n"
#define AI "Running game in player vs. AI mode\n"
#define WRONG_COLOR "Please enter either 'black' or 'white'\n"
#define WRONG_FILE_NAME "Wrong file name\n"

/* XML */
#define HEADER "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
#define SPACE "\t"
#define SPACE2 "\t\t"

/* Macros */
#define perror_message(func_name) (printf("Error: standard function %s has failed", func_name))
#define print_message(message) (printf("%s", message));
#define CONVRC(r,c)  ( ( 20 + (c) ) + ( (r - 1) * 10 ) ) /* Turn a row and column into a square */
#define OnBoard(s) !( (cols[s]) == (NOT_ON_BOARD) )
#define COLOR(c) ((c == 0)?("White"):("Black"))
#define PS FP(SPACE)
#define PS2 FP(SPACE2)
#define FP(str) {\
    if (fprintf(saveFile,str) < 0 ) {\
        printf("ERROR: Standard function fscanf failed\n");\
        fclose(saveFile);\
        return ERROR;\
    }\
}
#define FPV(str,var) {\
    if (fprintf(saveFile,str,var) < 0 ) {\
        printf("ERROR: Standard function fscanf failed\n");\
        fclose(saveFile);\
        return ERROR;\
    }\
}



enum {noPiece, wPawn, wRook, wKnight, wBishop, wQueen, wKing,
        bPawn, bRook, bKnight, bBishop, bQueen, bKing}; /*The Pieces */

enum {R1 = 1, R2, R3, R4, R5, R6, R7, R8}; /* The Rows */
enum {Ca = 1, Cb, Cc, Cd, Ce, Cf, Cg, Ch}; /* The Columns */

enum { /* The 64 squares, plus "not a square" and offboard squares */
  A1 = 21, B1, C1, D1, E1, F1, G1, H1,
  A2 = 31, B2, C2, D2, E2, F2, G2, H2,
  A3 = 41, B3, C3, D3, E3, F3, G3, H3,
  A4 = 51, B4, C4, D4, E4, F4, G4, H4,
  A5 = 61, B5, C5, D5, E5, F5, G5, H5,
  A6 = 71, B6, C6, D6, E6, F6, G6, H6,
  A7 = 81, B7, C7, D7, E7, F7, G7, H7,
  A8 = 91, B8, C8, D8, E8, F8, G8, H8, NOT_A_SQUARE, NOT_ON_BOARD
};

enum { wKingCas = 1, wQueenCas = 2, bKingCas = 4, bQueenCas = 8 }; /* Castling permissions */

typedef struct { /* A move */

	int from;
	int to;
    int capture;
    int promotion;
	char castling;

} Move;

typedef struct { /* A move with score for minimax */
	double score;
	Move move;
	char status;
} minimaxMove;

typedef struct { /* A list of moves */

	Move moves[MAX_MOVES_PER_TURN];
	int numMoves;

} MoveList;

typedef struct { /* The Board */

    int piecesBySquare[NUM_SQUARE]; /* An array that stores which piece is on which square */
    int squaresByPieces[13][8]; /* A double array that stores what square each piece is on */
    int pieceNum[13]; /* Keeps track of how many there are of each piece */

    char side; /* 0 for white, 1 for black */

    int numMovesInSearch; /* The number of moves made during current move search */

    char castling; /* A 4 bit number to store the four possible castling */

    /*double boardScore;  The score of the current board, based only on the pieces */

    Move prevMoves[MAX_GAME_MOVES]; /* An array of all moves made */

    char prevCastling[MAX_GAME_MOVES];

    int numMoves; /* The total number of moves made from the beginning of the game */

    char gameMode; /* Player Vs. Player or AI */

    char userCol; /*The user's color in the case of AI mode */

    char difficulty; /*The game's difficulty in the case of AI mode */

} Board;

extern Board brd;

#endif


