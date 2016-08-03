#include "minimax.h"


const int victimScore[13] = {0, 5, 7, 6, 6, 8, 9, 5, 7, 6, 6, 8, 9};
const int attackerScore[13] = {0, 0, 2, 1, 1, 3, 4, 0, 2, 1, 1, 3, 4};


/* Compare function for qsort - sorting moves */
int compareMoveScore(const void *move1, const void *move2) {
    int score1 = returnScore((Move *)move1);
    int score2 = returnScore((Move *)move2);
    return (score2 - score1); /* Reverse sorting - better move to be first */
}


/* Returns the move score for better pruning */
int returnScore(const Move *move) {
    int cap = mvvLva(move);
    int prom = move->promotion ? 1 : 0;
    return (cap + prom);
}


int mvvLva(const Move *move) {
    int victim, attacker;
    victim = move->capture;
    if (victim) {
        attacker = brd.piecesBySquare[move->from];
        return victimScore[victim] - attackerScore[attacker];
    }
    return 0;
}


/* Checks for mate and returns score for minimax */
int minimaxCheckWin(Board *brd, char maxPlayer) {

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

    return 0;
}


/* Minimax with pruning (alpha-beta) */
minimaxMove alphaBeta(Board *brd, double alpha, double beta, int depth, char best) {
    static int boards = 0;
    MoveList list;
    minimaxMove currMove,temp;
    minimaxMove bestMove;
    int index;
    char winStatus;
    char maxPlayer = (brd->userCol == brd->side);

    if (alpha == -MAX_SCORE && beta == MAX_SCORE) {
        boards = 0;
    }
    else {
        boards += 1;
    }

    bestMove.score = minimaxCheckWin(brd,maxPlayer);
    bestMove.status = (boards > MAX_MOVES_IN_MINIMAX);
    winStatus = abs(bestMove.score) == MAX_SCORE; /* Checks for end of game */

    if (depth == 0 || winStatus || (bestMove.status && best)) {
        if (depth == 0) bestMove.score = evalBoardScore(brd);
        return bestMove;
    }

    makeMoveList(brd,&list);
    if (SORT_MOVES_BY_VALUE) qsort(&list, list.numMoves, sizeof(Move), compareMoveScore); /* Sorting moves for better pruning */

    for(index = 0; index < list.numMoves; index++) {
        currMove.move = list.moves[index];
        makeMove(brd, currMove.move);
        brd->side^=1;
        temp = alphaBeta(brd, alpha, beta, depth-1,best);
        brd->side^=1;
        cancelMove(brd);

        currMove.score = temp.score;
        currMove.status = temp.status;
        if (currMove.status && best) return currMove; /* Calculated more than boards limit - breaks */

        if(maxPlayer) {
            if (alpha < currMove.score) {
                alpha = currMove.score;
                bestMove = currMove;
            }
            else if (RAND_EQUAL_MOVES && alpha == currMove.score && (rand() % 2)) {
                bestMove = currMove;
            }
        }
        else {
            if (beta > currMove.score) {
                beta = currMove.score;
                bestMove = currMove;
            }
            else if (RAND_EQUAL_MOVES && beta == currMove.score && (rand() % 2)) {
                bestMove = currMove;
            }
        }

        if (beta <= alpha) break; /* pruning */
    }
    return bestMove;
}



