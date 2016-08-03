#include "gameplay.h"

int playingMode = 0; /* This is used to switch between settings mode and playing mode */
Board brd; /* The game board */

/* Starts a new console game */
void startConsoleGame() {
    init();
    resetBoard(&brd);
    play(&brd);
}

/* The main play function */
void play(Board *brd) {
    char cmd[MAX_COMMAND_LENGTH] = "";
    int scan;
    printBoard(brd);
    printf("Enter game setting:\n");
    while (strcmp(cmd, "quit")) {
        if (!playingMode) {
            scan = scanf("%s", cmd);
            if (scan == 0) {
                printf("ERROR: Standard function scanf failed\n");
                strcpy(cmd, "quit");
            }
            readSettingCmd(brd, cmd);
        }
        else {
            printf("%s player - enter your move:\n", COLOR(brd->side));
            scan = scanf("%s", cmd);
            if (scan == 0) {
                printf("ERROR: Standard function scanf failed\n");
                strcpy(cmd, "quit");
            }
            readGameCmd(brd, cmd);
        }
    }
}

/* This functions reads and executes the user's commands during the settings stage */
void readSettingCmd(Board *brd, char *cmd) {
    char c;
    int depth, check, scan;
    char temp[MAX_COMMAND_LENGTH] = "";
    if (strcmp(cmd, "start") == 0) {
        playingMode = 1;
        if ( (brd->side != brd->userCol) && (brd->gameMode == 1) ) {
            makeComputerMove(brd,cmd);
        }
    }
    else if (strcmp(cmd, "user_color") == 0) {
        if (brd->gameMode == 1) {
            scan = scanf("%s", temp);
            if (scan == 0) {
                printf("ERROR: Standard function scanf failed\n");
                strcpy(cmd, "quit");
                return;
            }
            if (strcmp(temp, "black") == 0)
                brd->userCol = BLACK;
            else if (strcmp(temp, "white") == 0)
                brd->userCol = WHITE;
            else printf(WRONG_COLOR);
        }
        else {
            while ((c = getchar()) != '\n') {
                if (c == EOF) {
                    printf("ERROR: Standard function getchar failed\n");
                    strcpy(cmd, "quit");
                    return;
                }
            }
            printf(ILLEGAL_COMMAND);
        }
    }
    else if (strcmp(cmd, "game_mode") == 0) {
        scan = scanf("%s", temp);
        if (scan == 0) {
                printf("ERROR: Standard function scanf failed\n");
                strcpy(cmd, "quit");
                return;
        }
        if (strcmp(temp, "1") == 0) {
            brd->gameMode = 0;
            printf(TWO_PLAYER);
        }
        else if (strcmp(temp, "2") == 0) {
            brd->gameMode = 1;
            printf(AI);
        }
        else printf(WRONG_GAME_MODE);
    }
    else if (strcmp(cmd, "difficulty") == 0) {
        if (brd->gameMode == 1) {
            scan = scanf("%s", temp);
            if (scan == 0) {
                printf("ERROR: Standard function scanf failed\n");
                strcpy(cmd, "quit");
                return;
            }
            if(strcmp(temp,"depth") == 0) {
                scan = scanf("%d", &depth);
                if (scan == 0) {
                printf("ERROR: Standard function scanf failed\n");
                strcpy(cmd, "quit");
                return;
                }
                if (depth < 1 || depth > 4) {
                    printf(WRONG_MINIMAX_DEPTH);
                }
                else {
                    brd->difficulty = depth;
                }
            }
            else if (strcmp(temp,"best") == 0) {
                brd->difficulty = BEST_DIFFICULTY;
            }
            else {
                    while ((c = getchar()) != '\n') {
                        if (c == EOF) {
                            printf("ERROR: Standard function getchar failed\n");
                            strcpy(cmd, "quit");
                            return;
                        }
                    }
                    printf(ILLEGAL_COMMAND);
            }
        } else {
            while ((c = getchar()) != '\n') {
                if (c == EOF) {
                    printf("ERROR: Standard function getchar failed\n");
                    strcpy(cmd, "quit");
                    return;
                }
            }
            printf(ILLEGAL_COMMAND);
        }
    }
    else if (strcmp(cmd, "load") == 0) {
        scan = scanf("%s", temp);
        if (scan == 0) {
                printf("ERROR: Standard function scanf failed\n");
                strcpy(cmd, "quit");
                return;
        }
        check = loadBoard(brd, temp);
        if (!check) printBoard(brd);
        return;
    }
    else if (strcmp(cmd, "print") == 0) {
        printBoard(brd);
    }
    else if (strcmp(cmd, "quit")) {
        while ((c = getchar()) != '\n') {
            if (c == EOF) {
                    printf("ERROR: Standard function getchar failed\n");
                    strcpy(cmd, "quit");
                    return;
            }
        }
        printf(ILLEGAL_COMMAND);
    }
}

/*  This function reads and executes the user's commands during the gameplay stage */
void readGameCmd(Board *brd, char *cmd) {

    int square;
    int r, c;
    int check;
    Move move;
    MoveList moveList;
    char temp[MAX_COMMAND_LENGTH] = "";
    int scan;

    if (strcmp(cmd, "restart") == 0) {
        resetBoard(brd);
        printBoard(brd);
        printf("Enter game setting:\n");
        playingMode = 0;
    }
    else if (strcmp(cmd, "get_moves") == 0) {
        scan = scanf("%s", temp);
        if (scan == 0) {
                printf("ERROR: Standard function scanf failed\n");
                strcpy(cmd, "quit");
                return;
        }
        if (!checkLegalPosition(temp,0)) return;
        c = temp[1] - 'a';
        r = temp[3] - '1';
        square = CONVRC(r+1,c+1);
        if (pieceCol[brd->piecesBySquare[square]] != brd->side) {
            printf(NO_PIECE);
            return;
        }
        getMoves(brd, &moveList, square);
        printMoveList(&moveList);
        return;
    }
    else if ( (strcmp(cmd, "move") == 0) || (strcmp(cmd, "castle") == 0) )  {
        if ((c = getchar()) == EOF) {
            printf("ERROR: Standard function getchar failed\n");
            strcpy(cmd, "quit");
            return;
        }
        scan = scanf("%[^\n]", temp);
        if (scan == 0) {
                printf("ERROR: Standard function scanf failed\n");
                strcpy(cmd, "quit");
                return;
        }
        if (!checkLegalPosition(temp,0)) return;
        if (strcmp(cmd, "move") == 0)
            if (!checkLegalPosition(temp,9)) return;
        move = convertToMove(brd, temp);
        if (pieceCol[brd->piecesBySquare[move.from]] != brd->side) {
            printf(NO_PIECE);
            return;
        }
        if ( (strcmp(cmd, "castle") == 0) )
            if ( (brd->piecesBySquare[move.from] != wRook) && ( (brd->piecesBySquare[move.from] != bRook) ) ) {
                printf(NO_ROOK);
                return;
            }
        getMoves(brd, &moveList, move.from);
        if(!legalMove(brd, move, &moveList)) {
            if (strcmp(cmd, "castle") == 0) {
               printf(ILLEGAL_CASTLE);
            } else {
               printf(ILLEGAL_MOVE);
            }
            return;
        }
        makeMove(brd, move);
        check = checkWin(brd);
        printBoard(brd);
        handleCheck(brd, check, cmd);
        brd->side^=1;
    }
    else if (strcmp(cmd, "save") == 0) {
        scan = scanf("%s", temp);
        if (scan == 0) {
                printf("ERROR: Standard function scanf failed\n");
                strcpy(cmd, "quit");
                return;
        }
        saveBoard(brd, temp);
        return;
    }
    else if (strcmp(cmd, "quit")) {
        while ((c = getchar()) != '\n') {
            if (c == EOF) {
                    printf("ERROR: Standard function getchar failed\n");
                    strcpy(cmd, "quit");
                    return;
            }
        }
        printf(ILLEGAL_COMMAND);
        return;
    }

    if (strcmp(cmd, "quit")) {
        if (brd->gameMode == 1) {
            makeComputerMove(brd,cmd);
        }
    }

}

/* This functions determines the AI's next move and executes it */
void makeComputerMove(Board *brd, char *cmd) {

    char *moveStr;
    int check;

    moveStr = computerMove(brd);
    printf("Computer: move %s\n", moveStr);
    printBoard(brd);

    check = checkWin(brd);
    handleCheck(brd, check, cmd);

    brd->side^=1;

}

/* This functions determines how many moves the current player can make. This is used for 'best' difficulty */
int addNumMoves(const Board *brd, char turn) {

    int numMoves = 0;
    int index, square, piece;

    for (index = 0; index < 64; index++) {
        square = smallToBig[index];
        piece = brd->piecesBySquare[square];
        if ( ((pieceCol[piece] == brd->userCol) && (turn = USER)) || ((pieceCol[piece] != brd->userCol) && (turn = COMP)) )
             numMoves += pieceMoves[piece];
    }
    return numMoves;
}

/* This function calculates the maximum minimax depth that won't surpass one million boards */
int calcBestDepth(const Board *brd) {

    int numMoves = 0;
    int prevLayer = 1;
    int depth = 1;
    char turn = COMP;

    prevLayer += addNumMoves(brd, turn);
    numMoves = prevLayer;
    turn ^= 1;

    while (numMoves <= MAX_MOVES_IN_MINIMAX) {
        prevLayer *= addNumMoves(brd, turn);
        numMoves += prevLayer;
        if (numMoves <= MAX_MOVES_IN_MINIMAX) depth += 1;
        turn ^= 1;
    }
    return depth;

}

/* This functions makes the actual AI move using minimax */
char *computerMove(Board *brd) {

    char *moveStr;
    int depth = NUMBER_OF_DIFFICULTIES;
    minimaxMove res;
    Move move;

    if (brd->difficulty != BEST_DIFFICULTY) {
        move = alphaBeta(brd,-MAX_SCORE,MAX_SCORE,brd->difficulty,FALSE).move;
    }
    else {
        res = alphaBeta(brd,-MAX_SCORE,MAX_SCORE,depth,FALSE);
        res.status = OK;
        while (!res.status) {
            move = res.move;
            depth += 1;
            res = alphaBeta(brd,-MAX_SCORE,MAX_SCORE,depth,TRUE);
        }
    }
    moveStr = printMove(move);
    makeMove(brd,move);

    return moveStr;

}

/* This function checks whether the last move resulted in a checkmate or check */
int checkWin(Board *brd) {

   MoveList list;
   int kingSquare;

   brd->side^=1;
   makeMoveList(brd, &list);

   brd->side^=1;

   if(list.numMoves == 0) return CHECK_MATE;

   if(((brd->side)^1) == WHITE) {
        kingSquare = brd->squaresByPieces[wKing][0];
        if(isSquareThreat(brd, kingSquare, BLACK)) return CHECK;
   } else {
        kingSquare = brd->squaresByPieces[bKing][0];
        if(isSquareThreat(brd, kingSquare, WHITE)) return CHECK;
   }

   return NO_CHECK;

}

/* This function checks whether the position the user has entered is a legal board position */
int checkLegalPosition(char *move, int k) {

    int i,j;

    i = move[k+1] - 'a';
    j = move[k+3] - '1';

    if ( i < 0 || i > 7 || j < 0 || j > 7 ) {
        printf(WRONG_POSITION);
        return FALSE;
    }
    return TRUE;
}

/* This function handles the cases where the last move resulted in a checkmate or check */
void handleCheck(Board *brd, int check, char *cmd) {

    if (check == CHECK_MATE) {
        if (brd->side == WHITE) {
            printf(WHITE_WINS);
            strcpy(cmd, "quit");
        } else {
            printf(BLACK_WINS);
            strcpy(cmd, "quit");
        }
    } else if (check == CHECK) printf(CHECK_MSG);

}
