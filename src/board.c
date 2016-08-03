#include "board.h"

/* Creates the initial board structure at the beginning of a new game */
void resetBoard(Board *brd) {

    /* Resets a board */

	int index ,piece, square;

	for(index = 0; index < NUM_SQUARE; ++index) {
		brd->piecesBySquare[index] = NOT_ON_BOARD;
	}

	for(index = 0; index < 64; ++index) {
		brd->piecesBySquare[smallToBig[index]] = noPiece;
	}

	for(index = CONVRC(2,1); index <= CONVRC(2,8); index++) {
        brd->piecesBySquare[index] = wPawn;
	}

	for(index = CONVRC(7,1); index <= CONVRC(7,8); index++) {
        brd->piecesBySquare[index] = bPawn;
	}
    /* Place the pieces */
	brd->piecesBySquare[CONVRC(1,1)] = brd->piecesBySquare[CONVRC(1,8)] = wRook;
	brd->piecesBySquare[CONVRC(1,2)] = brd->piecesBySquare[CONVRC(1,7)] = wKnight;
	brd->piecesBySquare[CONVRC(1,3)] = brd->piecesBySquare[CONVRC(1,6)] = wBishop;
	brd->piecesBySquare[CONVRC(1,4)] = wQueen;
	brd->piecesBySquare[CONVRC(1,5)] = wKing;
	brd->piecesBySquare[CONVRC(8,1)] = brd->piecesBySquare[CONVRC(8,8)] = bRook;
	brd->piecesBySquare[CONVRC(8,2)] = brd->piecesBySquare[CONVRC(8,7)] = bKnight;
	brd->piecesBySquare[CONVRC(8,3)] = brd->piecesBySquare[CONVRC(8,6)] = bBishop;
	brd->piecesBySquare[CONVRC(8,4)] = bQueen;
	brd->piecesBySquare[CONVRC(8,5)] = bKing;

	brd->pieceNum[0] = 0;
	brd->pieceNum[1] = brd->pieceNum[7] = 8;
	brd->pieceNum[2] = brd->pieceNum[3] = brd->pieceNum[4] = brd->pieceNum[8] = brd->pieceNum[9] = brd->pieceNum[10] = 2;
	brd->pieceNum[5] = brd->pieceNum[6] = brd->pieceNum[11] = brd->pieceNum[12] = 1;


    for(piece = 0; piece < 13; piece++) {
        for(index = 0; index < 8; index++) {
            brd->squaresByPieces[piece][index] = NOT_A_SQUARE;
        }
    }

	for(square = 31, index = 0; square < 39; square++, index++) {
            brd->squaresByPieces[1][index] = square;
	}
	for(square = 81, index = 0; square < 89; square++, index++) {
            brd->squaresByPieces[7][index] = square;
	}
	brd->squaresByPieces[2][0] = 21; brd->squaresByPieces[2][1] = 28;
	brd->squaresByPieces[3][0] = 22; brd->squaresByPieces[3][1] = 27;
	brd->squaresByPieces[4][0] = 23; brd->squaresByPieces[4][1] = 26;
	brd->squaresByPieces[5][0] = 24;
	brd->squaresByPieces[6][0] = 25;
	brd->squaresByPieces[8][0] = 91; brd->squaresByPieces[8][1] = 98;
	brd->squaresByPieces[9][0] = 92; brd->squaresByPieces[9][1] = 97;
	brd->squaresByPieces[10][0] = 93; brd->squaresByPieces[10][1] = 96;
	brd->squaresByPieces[11][0] = 94;
	brd->squaresByPieces[12][0] = 95;

	brd->side = WHITE;

	brd->castling = 15;

	brd->numMoves = 0;

    brd->gameMode = DEFAULT_GAMEMODE;

    brd->userCol = DEFAULT_USER_COLOUR;

    brd->difficulty = DEFAULT_DIFFICULTY;

}
/* Saves the current game into a file */
int saveBoard(const Board *brd, const char *fileName) {

    FILE *saveFile;
    int rowNum;
    char rowStr[9];

    saveFile = fopen(fileName, "w");
    if (saveFile == NULL) {
        printf(WRONG_FILE_NAME);
        return ERROR;
    };

    FP(HEADER);
    FP("<game>\n");
    PS;
    FPV("<type>%d</type>\n", brd->gameMode == 0 ? 1 : 2);
    if (brd->gameMode == 1) {
        PS;
        if (brd->difficulty > 0)
            {FPV("<difficulty>%d</difficulty>\n", brd->difficulty);}
        else
            {FP("<difficulty>Best</difficulty>\n");}
        PS;
        if (brd->userCol == WHITE)
            {FP("<user_color>White</user_color>\n");}
        else
            {FP("<user_color>Black</user_color>\n");}
    }
    PS;
    if (brd->side == WHITE)
        {FP("<next_turn>White</next_turn>\n");}
    else
        {FP("<next_turn>Black</next_turn>\n");}
    PS;
    FP("<board>\n");
    for(rowNum = 8; rowNum >= 1; rowNum--) {
        PS2;
        FPV("<row_%d>",rowNum);
        FPV("%s", printRow(brd, rowNum, rowStr));
        FPV("</row_%d>\n",rowNum);
    }
    PS;
    FP("</board>\n");
    PS;
    FP("<general>\n");
    PS2;
    if (fprintf(saveFile,"<castle>%d%d%d%d</castle>\n",(brd->castling & 8) >> 3,
                (brd->castling & 4) >> 2,(brd->castling & 3) >> 1,(brd->castling & 1)) < 0 ) {
        printf("ERROR: Standard function fscanf failed\n");
        fclose(saveFile);
        return ERROR;
    }
    PS;
    FP("</general>\n");
    FP("</game>\n");

    fclose(saveFile);

    return OK;

}

/* Loads a game from a file */
int loadBoard(Board *brd, const char *fileName) {

    FILE *saveFile;
    int index, rowNum, colNum, piece, square, scan;
    char castle = 0;
    char readStr[40];
    char foundCastle = FALSE;

    saveFile = fopen(fileName, "r");
    if (saveFile == NULL) {
        printf(WRONG_FILE_NAME);
        return ERROR;
    }

    for(index = 0; index < NUM_SQUARE; ++index) {
		brd->piecesBySquare[index] = NOT_ON_BOARD;
	}

	for(index = 0; index < 64; ++index) {
		brd->piecesBySquare[smallToBig[index]] = noPiece;
	}

	for(index = 1; index <= 13; index++) {
        brd->pieceNum[index] = 0;
	}

	for(piece = 0; piece < 13; piece++) {
        for(index = 0; index < 8; index++) {
            brd->squaresByPieces[piece][index] = NOT_A_SQUARE;
        }
    }

    brd->numMoves = 0;
    brd->gameMode = DEFAULT_GAMEMODE;
    brd->userCol = DEFAULT_USER_COLOUR;
    brd->difficulty = DEFAULT_DIFFICULTY;
    brd->side = WHITE;

    while ( (scan = fscanf(saveFile, "%s", readStr)) != EOF) {

        if (scan == 0) {
            printf("ERROR: Standard function fscanf failed\n");
            resetBoard(brd);
            fclose(saveFile);
            return ERROR;
        }

        if (!strncmp(readStr,"<type",5)) {
            if (readStr[6] == '1') {
                    brd->gameMode = 0;
            }
            else {
                    brd->gameMode = 1;
            }
        }

        if (!strncmp(readStr,"<difficulty",11)) {
            if (readStr[12] == 'B') brd->difficulty = BEST_DIFFICULTY;
            else brd->difficulty = readStr[12] - '0';
        }

        if (!strncmp(readStr,"<user_color",11)) {
            if (readStr[12] == 'W') {
                brd->userCol = WHITE;
            }
            else {
                brd->userCol = BLACK;
            }
        }

        if (!strncmp(readStr,"<next_turn",10)) {
            if (readStr[11] == 'W') {
                brd->side = WHITE;
            }
            else {
                brd->side = BLACK;
            }
        }


        if (!strncmp(readStr,"<row",4)) {
            rowNum = readStr[5] - '0';
            for (colNum = Ca; colNum <= Ch; colNum++) {
                if(readStr[colNum+6] == '_') {
                    piece = 0;
                }
                else {
                    piece = pieceFromChar(readStr[colNum+6]);
                }
                square = CONVRC(rowNum, colNum);
                brd->piecesBySquare[square] = piece;
                if (piece != 0) {
                    brd->squaresByPieces[piece][brd->pieceNum[piece]] = square;
                    brd->pieceNum[piece]++;
                }
            }
        }

        if (!strncmp(readStr,"<castle",7)) {
            for (index = 0; index < 4; index++) {
                castle |= (readStr[index + 8] - '0') << (3 - index);
            }
            brd->castling = castle;
            foundCastle = TRUE;
        }
    }
    fclose(saveFile);

    if(!foundCastle) {
        castle = 15;
        if (brd->piecesBySquare[H1] != wRook) castle &= 14;
        if (brd->piecesBySquare[A1] != wRook) castle &= 13;
        if (brd->piecesBySquare[E1] != wKing) castle &= 12;
        if (brd->piecesBySquare[H8] != bRook) castle &= 11;
        if (brd->piecesBySquare[A8] != bRook) castle &= 7;
        if (brd->piecesBySquare[E8] != bKing) castle &= 3;
        brd->castling = castle;
    }

    return OK;
}

/* Adds to the score based on a piece's position on the board */
double addPositionScore(const Board *brd, int piece, int index, const double table[]) {

    if (pieceCol[piece] == WHITE) index = 63 - index;
    if (pieceCol[piece] == brd->userCol) return table[index];
    return -table[index];

}

/* Evaluates the current score of the boar */
double evalBoardScore(Board *brd) {

    int index, piece;
    double score = 0;
    char maxPlayer = (brd->userCol == brd->side);
    char winStatus;

    brd->side^=1;
    winStatus = checkWin(brd);
    brd->side^=1;

    if (winStatus == CHECK_MATE) {
        if (maxPlayer) {
            return -MAX_SCORE;
        }
        else {
            return MAX_SCORE;
        }
    }

    for(index = 0; index < 64; index++) {
        piece = brd->piecesBySquare[smallToBig[index]];
        if (pieceCol[piece] == brd->userCol) {
            score += pieceVal[piece];
        }
        else {
            score -= pieceVal[piece];
        }

        if ( (piece == wPawn) || (piece == bPawn) ) {
            score += addPositionScore(brd, piece, index, pawnTable);
        }
        if ( (piece == wRook) || (piece == bRook) ) {
            score += addPositionScore(brd, piece, index, rookTable);
        }
        if ( (piece == wKnight) || (piece == bKnight) ) {
            score += addPositionScore(brd, piece, index, knightTable);
        }
        if ( (piece == wBishop) || (piece == bBishop) ) {
            score += addPositionScore(brd, piece, index, bishopTable);
        }

    }
    return score;

}

/* Prints a row onto the console */
void print_line() {
	char i;
	printf(" |");
		for (i = 1; i < 32; i++){
			printf("-");
		}
		printf("|\n");
}

/* Prints the board to the console */
void printBoard(const Board *brd) {

	int i,j,square,piece;
		print_line();
		for (j = R8; j >= R1 ; j--)
		{
			printf("%d", j);
			for (i = Ca; i <= Ch; i++){
                square = CONVRC(j,i);
                piece = brd->piecesBySquare[square];
				printf("| %c ", pceChar[piece]);
			}
			printf("|\n");
			print_line();
		}
		printf("  ");
		for (j = Ca; j <= Ch; j++){
			printf(" %c  ", (char)('a' + j - 1));
		}
		printf("\n");
}
