#include "move.h"

/* Converts a string representing a move into a move variable */
Move convertToMove(Board *brd, char *moveStr) {

    Move move;
    int fromRow, toRow, fromCol, toCol, fromSquare, toSquare;
    int prom = 0;

    fromCol = *(moveStr + 1) - 'a' + 1;
    fromRow = *(moveStr + 3) - '1' + 1;

    fromSquare = CONVRC(fromRow,fromCol);
    move.from = CONVRC(fromRow, fromCol);

    if (*(moveStr + 5) == '\0') {
        switch(fromSquare)
        {
            case 28:
                move.to = 26;
                break;
            case 21:
                move.to = 24;
                break;
            case 98:
                move.to = 96;
                break;
            case 91:
                move.to = 94;
                break;
        }
        move.capture = 0;
        move.promotion = 0;
        move.castling = 1;
        return move;
    }

    toCol = *(moveStr + 10) - 'a' + 1;
    toRow = *(moveStr + 12) - '1' + 1;

    toSquare = CONVRC(toRow, toCol);

    if (brd->piecesBySquare[fromSquare] == wPawn && toRow == 8) prom = wQueen;
    if (brd->piecesBySquare[fromSquare] == bPawn && toRow == 1) prom = bQueen;
    if (*(moveStr + 14) != '\0') prom = *(moveStr + 22) - '1';

    move.to = toSquare;
    move.capture = brd->piecesBySquare[toSquare];
    move.promotion = prom;
    move.castling = 0;

    return move;

}

/* Creates a move variable when playing in GUI mode using the from and to squares */
Move createPlayerMoveGUI(Board *brd, int from, int to) {

    Move move;

    move.from = from;
    move.to = to;

    if(pieceCol[brd->piecesBySquare[to]] == (brd->side^1)) move.capture = brd->piecesBySquare[to];
    else move.capture = 0;

    move.promotion = 0;
    if ( ((from == E1) && (to == G1)) || ((from == E1) && (to == C1)) || ((from == E8) && (to == G8)) || ((from == E8) && (to == C8)) )
        move.castling = 1;
    else
        move.castling = 0;

    return move;

}

/* Creates a move when playing in console mode. Also determines it's legality. */
Move createMove(Board *brd, int from, int to, int cap, int prom, char castling) {

    Move move;

    move.from = from;
    move.to = to;
    move.capture = cap;
    move.promotion = prom;
    move.castling = castling;

    if(!makeMove(brd,move)) move.from = 0;
    cancelMove(brd);

    return move;

}

/* This function checks if the current square, sq, is threatened by "side"
       The functions starts from the square, sq, and goes in the direction of all the pieces.
       For instance, it will go into all diagonals and see if a bishop which belongs to "side" is there. */
int isSquareThreat(const Board *brd, const int square, const int side) {

	int piece,index,temp,dir;

	/* pawns */
	if(side == WHITE) {
		if(brd->piecesBySquare[square-11] == wPawn || brd->piecesBySquare[square-9] == wPawn) {
			return TRUE;
		}
	} else {
		if(brd->piecesBySquare[square+11] == bPawn || brd->piecesBySquare[square+9] == bPawn) {
			return TRUE;
		}
	}

	/* knights */
	for(index = 0; index < 8; ++index) {
		piece = brd->piecesBySquare[square + directionsOfPieces[3][index]];
		if( (side == WHITE && piece == wKnight) || (side == BLACK && piece == bKnight) ) {
			return TRUE;
		}
	}

	/* rooks, queens */
	for(index = 0; index < 4; ++index) {
		dir = directionsOfPieces[2][index];
		temp = square + dir;
		piece = brd->piecesBySquare[temp];
		while(OnBoard(temp)) {
			if(piece != noPiece) {
				if( (((piece == wQueen) || (piece == wRook)) && side == WHITE) ||
                                (((piece == bQueen) || (piece == bRook)) && side == BLACK) ) return TRUE;
                break;
			}
			temp += dir;
			piece = brd->piecesBySquare[temp];
		}
	}

	/* bishops, queens */
	for(index = 0; index < 4; ++index) {
		dir = directionsOfPieces[4][index];
		temp = square + dir;
		piece = brd->piecesBySquare[temp];
		while(OnBoard(temp)) {
			if(piece != noPiece) {
				if( (((piece == wQueen) || (piece == wBishop)) && side == WHITE) ||
                                (((piece == bQueen) || (piece == bBishop)) && side == BLACK) ) return TRUE;
                break;

			}
			temp += dir;
			piece = brd->piecesBySquare[temp];
		}
	}

	/* kings */
	for(index = 0; index < 8; ++index) {
		piece = brd->piecesBySquare[square + directionsOfPieces[6][index]];
		if( (piece == wKing && side == WHITE) ||
                               (piece == bKing && side == BLACK) )
					return TRUE;
    }

	return FALSE;

}

/* Checks if two moves are equal. Two moves are considered equal if they have the same source and destination and are both castling moves or non-castling moves */
int movesEqual(const Move move1, const Move move2) {

    if (move1.from != move2.from) return FALSE;
    if (move1.to != move2.to) return FALSE;
    if (move1.castling != move2.castling) return FALSE;
    return TRUE;

}

/* Checks if a move is legal. A legal move is one that does not expose the king to check or leaves the king in check */
int legalMove(Board *brd , const Move move, const MoveList *list) {

	int index;
	for(index = 0; index < list->numMoves; index++) {

		if( movesEqual(list->moves[index], move) )  {
			return TRUE;
		}
    }
	return FALSE;
}

/* Adds a move to a move list, after verifying it's legality */
void addMove(Board *brd, int from, int to, int cap, int prom, char castling, MoveList *list) {

    Move move;

    move = createMove(brd, from, to, cap, prom, castling);
    if (move.from != 0) {
        list->moves[list->numMoves] = move;
        list->numMoves++;
    }

}

/* Gets all of a specified pawn's moves */
void getPawnMoves(Board *brd, MoveList *list, int square) {

    if(pieceCol[brd->piecesBySquare[square]] == WHITE) {

        if(brd->piecesBySquare[square + 10] == noPiece) {
            if(rows[square + 10] == R8) {
               addMove(brd, square, square + 10, noPiece, wQueen, 0, list);
               addMove(brd, square, square + 10, noPiece, wRook, 0, list);
               addMove(brd, square, square + 10, noPiece, wBishop, 0, list);
               addMove(brd, square, square + 10, noPiece, wKnight, 0, list);
            } else {
               addMove(brd, square, square + 10, noPiece, noPiece, 0, list);
            }
        }

        if( (OnBoard(square + 9)) && pieceCol[brd->piecesBySquare[square + 9]] == BLACK) {
            if(rows[square + 9] == R8) {
               addMove(brd, square, square + 9, brd->piecesBySquare[square + 9], wQueen, 0, list);
               addMove(brd, square, square + 9, brd->piecesBySquare[square + 9], wRook, 0, list);
               addMove(brd, square, square + 9, brd->piecesBySquare[square + 9], wBishop, 0, list);
               addMove(brd, square, square + 9, brd->piecesBySquare[square + 9], wKnight, 0, list);
            } else {
               addMove(brd, square, square + 9, brd->piecesBySquare[square + 9], noPiece, 0, list);
            }
        }

        if( (OnBoard(square + 11)) && pieceCol[brd->piecesBySquare[square + 11]] == BLACK) {
            if(rows[square + 11] == R8) {
               addMove(brd, square, square + 11, brd->piecesBySquare[square + 11], wQueen, 0, list);
               addMove(brd, square, square + 11, brd->piecesBySquare[square + 11], wRook, 0, list);
               addMove(brd, square, square + 11, brd->piecesBySquare[square + 11], wBishop, 0, list);
               addMove(brd, square, square + 11, brd->piecesBySquare[square + 11], wKnight, 0, list);
            } else {
               addMove(brd, square, square + 11, brd->piecesBySquare[square + 11], noPiece, 0, list);
            }
        }

    } else { /* same, but for black */

        if(brd->piecesBySquare[square - 10] == noPiece) {
            if(rows[square - 10] == R8) {
                addMove(brd, square, square - 10, noPiece, wQueen, 0,  list);
                addMove(brd, square, square - 10, noPiece, wRook, 0, list);
                addMove(brd, square, square - 10, noPiece, wBishop, 0, list);
                addMove(brd, square, square - 10, noPiece, wKnight, 0, list);
            } else {
                addMove(brd, square, square - 10, noPiece, noPiece, 0, list);
            }
        }

        if( (OnBoard(square - 9)) && pieceCol[brd->piecesBySquare[square - 9]] == WHITE) {
            if(rows[square - 9] == R8) {
                addMove(brd, square, square - 9, brd->piecesBySquare[square - 9], wQueen, 0, list);
                addMove(brd, square, square - 9, brd->piecesBySquare[square - 9], wRook, 0, list);
                addMove(brd, square, square - 9, brd->piecesBySquare[square - 9], wBishop, 0, list);
                addMove(brd, square, square - 9, brd->piecesBySquare[square - 9], wKnight, 0, list);
            } else {
                addMove(brd, square, square - 9, brd->piecesBySquare[square - 9], noPiece, 0, list);
            }
        }

        if( (OnBoard(square - 11)) && pieceCol[brd->piecesBySquare[square - 11]] == WHITE) {
            if(rows[square - 11] == R8) {
                addMove(brd, square, square - 11, brd->piecesBySquare[square - 11], wQueen, 0, list);
                addMove(brd, square, square - 11, brd->piecesBySquare[square - 11], wRook, 0, list);
                addMove(brd, square, square - 11, brd->piecesBySquare[square - 11], wBishop, 0, list);
                addMove(brd, square, square - 11, brd->piecesBySquare[square - 11], wKnight, 0, list);
            } else {
                addMove(brd, square, square - 11, brd->piecesBySquare[square - 11], noPiece, 0, list);
            }
        }

    }

}

/* Gets the castling move of the piece on a certain square, if any */
void getCastlingMoves(Board *brd, MoveList *list, int square) {

    int piece = brd->piecesBySquare[square];

    if (piece == wRook && (brd->castling & wKingCas) && square == H1) {
        if(brd->piecesBySquare[F1] == noPiece && brd->piecesBySquare[G1] == noPiece) {
            if(!isSquareThreat(brd,E1,BLACK) && !isSquareThreat(brd,F1,BLACK) ) {
                addMove(brd, H1, F1, noPiece, noPiece, 1, list);
            }
        }
    }
    else if (piece == wRook && (brd->castling & wQueenCas) && square == A1) {
        if(brd->piecesBySquare[D1] == noPiece && brd->piecesBySquare[C1] == noPiece && brd->piecesBySquare[B1] == noPiece) {
            if(!isSquareThreat(brd,E1,BLACK) && !isSquareThreat(brd,D1,BLACK) ) {
                addMove(brd, A1, D1, noPiece, noPiece, 1, list);
            }
        }
    }

    if (piece == wKing && (brd->castling & wKingCas)) {
        if(brd->piecesBySquare[F1] == noPiece && brd->piecesBySquare[G1] == noPiece) {
            if(!isSquareThreat(brd,E1,BLACK) && !isSquareThreat(brd,F1,BLACK) ) {
                addMove(brd, E1, G1, noPiece, noPiece, 1, list);
            }
        }
    }
    if (piece == wKing && (brd->castling & wQueenCas)) {
        if(brd->piecesBySquare[D1] == noPiece && brd->piecesBySquare[C1] == noPiece && brd->piecesBySquare[B1] == noPiece) {
            if(!isSquareThreat(brd,E1,BLACK) && !isSquareThreat(brd,D1,BLACK) ) {
                addMove(brd, E1, C1, noPiece, noPiece, 1, list);
            }
        }
    }

    if (piece == bRook && (brd->castling & bKingCas) && square == H8) {
        if(brd->piecesBySquare[F8] == noPiece && brd->piecesBySquare[G8] == noPiece) {
            if(!isSquareThreat(brd,E8,WHITE) && !isSquareThreat(brd,F8,WHITE) ) {
                addMove(brd, H8, F8, noPiece, noPiece, 1, list);
            }
        }
    }
    else if (piece == bRook && (brd->castling & bQueenCas) && square == A1) {
        if(brd->piecesBySquare[D8] == noPiece && brd->piecesBySquare[C8] == noPiece && brd->piecesBySquare[B8] == noPiece) {
            if(!isSquareThreat(brd,E8,WHITE) && !isSquareThreat(brd,D8,WHITE) ) {
                addMove(brd, A8, D8, noPiece, noPiece, 1, list);
            }
        }
    }
    if (piece == bKing && (brd->castling & bKingCas)) {
        if(brd->piecesBySquare[F8] == noPiece && brd->piecesBySquare[G8] == noPiece) {
            if(!isSquareThreat(brd,E8,WHITE) && !isSquareThreat(brd,F8,WHITE) ) {
                addMove(brd, E8, G8, noPiece, noPiece, 1, list);
            }
        }
    }
    if (piece == bKing && (brd->castling & bQueenCas)) {
        if(brd->piecesBySquare[D8] == noPiece && brd->piecesBySquare[C8] == noPiece && brd->piecesBySquare[B8] == noPiece) {
            if(!isSquareThreat(brd,E8,WHITE) && !isSquareThreat(brd,D8,WHITE) ) {
                addMove(brd, E8, C8, noPiece, noPiece, 1, list);
            }
        }
    }
}

/* Gets all moves of a certain queen, rook or bishop */
void getContMoves(Board *brd, MoveList *list, int square) {

    int piece, toSquare, dir, j;

    piece = brd->piecesBySquare[square];

    for(j = 0; j < numOfDirections[piece]; j++) {
        dir = directionsOfPieces[piece][j];
        toSquare = square + dir;

        while(OnBoard(toSquare)) {
            if(brd->piecesBySquare[toSquare] != noPiece) {
                if( pieceCol[brd->piecesBySquare[toSquare]] == (brd->side ^ 1)) {
                    addMove(brd, square, toSquare, brd->piecesBySquare[toSquare], noPiece , 0, list);
                }
                break;
            }
            addMove(brd, square, toSquare, noPiece, noPiece , 0, list);
            toSquare += dir;
        }
    }

}

/* Gets all moves of a certain king or knight */
void getDiscMoves(Board *brd, MoveList *list, int square) {

    int piece, toSquare, dir, j;

    piece = brd->piecesBySquare[square];

    for(j = 0; j < numOfDirections[piece]; j++) {
        dir = directionsOfPieces[piece][j];
        toSquare = square + dir;

        if(OnBoard(toSquare)) {

            if(brd->piecesBySquare[toSquare] != noPiece) {
                if( pieceCol[brd->piecesBySquare[toSquare]] == (brd->side ^ 1)) {
                    addMove(brd, square, toSquare, brd->piecesBySquare[toSquare], noPiece , 0, list);
                }
                continue;
            }
            addMove(brd, square, toSquare, noPiece, noPiece , 0, list);
        }
    }
}

/* Get all the moves of the piece in the specified square */
void getMoves(Board *brd, MoveList *list, int square) {

    int piece;

    list->numMoves = 0;
    piece = brd->piecesBySquare[square];
    if(piece == wPawn || piece == bPawn) {
        getPawnMoves(brd, list, square);
    } else if(piece == wKnight || piece == bKnight) {
        getDiscMoves(brd, list, square);
    } else if (piece == wKing || piece == bKing) {
        getCastlingMoves(brd, list, square);
        getDiscMoves(brd, list, square);
    } else if(piece == wRook || piece == bRook) {
        getCastlingMoves(brd, list, square);
        getContMoves(brd, list, square);
    } else if(piece == wQueen || piece == wBishop || piece == bQueen || piece == bBishop) {
        getContMoves(brd, list, square);
    }

}

/* Create a list of a player's all possible moves */
void makeMoveList(Board *brd, MoveList *list) {

    int piece, square, pieceNum, i, pieceIndex;

	list->numMoves = 0;

    /* Pawns */

	if(brd->side == WHITE) {

		for(pieceNum = 0; pieceNum < brd->pieceNum[wPawn]; pieceNum++) {
            getPawnMoves(brd, list, brd->squaresByPieces[wPawn][pieceNum]);
		}

	} else { /* same, but for black */

       for(pieceNum = 0; pieceNum < brd->pieceNum[bPawn]; pieceNum++) {
            getPawnMoves(brd, list, brd->squaresByPieces[bPawn][pieceNum]);
		}

    }

    /* Castling */

    if(brd->side == WHITE) {

		getCastlingMoves(brd, list, E1);

	} else { /* same, but for black */

        getCastlingMoves(brd, list, E8);

    }


	/* Continuous pieces for both colors */

	if (brd->side == WHITE) {
        pieceIndex = 0;
	} else {
	    pieceIndex = 3;
    }

    piece = contPieces[pieceIndex];

	for(i = 0; i < 3; i++) {

		for(pieceNum = 0; pieceNum < brd->pieceNum[piece]; pieceNum++) {

			square = brd->squaresByPieces[piece][pieceNum];
			getContMoves(brd, list, square);

		}
		piece = contPieces[++pieceIndex];
	}

	/* Discrete Pieces */

	if (brd->side == WHITE) {
        pieceIndex = 0;
	} else {
	    pieceIndex = 2;
    }

    piece = discPieces[pieceIndex];

	for(i = 0; i < 2; i++) {

		for(pieceNum = 0; pieceNum < brd->pieceNum[piece]; pieceNum++) {
			square = brd->squaresByPieces[piece][pieceNum];
            getDiscMoves(brd, list, square);
		}

		piece = discPieces[++pieceIndex];
	}

}

/* Removes a piece from the board */
void removePiece(Board *brd, int square) {

    int piece = brd->piecesBySquare[square];
    int tempSquare = NOT_A_SQUARE;
    int index = 0;

    brd->piecesBySquare[square] = noPiece;
    brd->pieceNum[piece]--;

    while(tempSquare != square) {
        tempSquare = brd->squaresByPieces[piece][index];
        index++;
    }

    brd->squaresByPieces[piece][--index] = brd->squaresByPieces[piece][brd->pieceNum[piece]];

}

/* Adds a piece to the board */
void addPiece(Board *brd, int square, int piece) {

    brd->piecesBySquare[square] = piece;
    brd->squaresByPieces[piece][brd->pieceNum[piece]] = square;
    brd->pieceNum[piece]++;

}

/* Moves a piece on the board */
void movePiece(Board *brd, int from, int to) {

    int piece = brd->piecesBySquare[from];
    int tempSquare = NOT_A_SQUARE;
    int index = 0;

    while(tempSquare != from) {
        tempSquare = brd->squaresByPieces[piece][index];
        index++;
    }

    brd->squaresByPieces[piece][--index] = to;
    brd->piecesBySquare[from] = noPiece;
    brd->piecesBySquare[to] = piece;

}

/*Updates the board's castling permissions */
void updateCastling(Board *brd, Move move) {

    int from = move.from;
    int piece = brd->piecesBySquare[move.to];

    brd->prevCastling[brd->numMoves] = brd->castling;

    switch(piece) {

    case(wKing):
        brd->castling &= 12;
        break;
    case(bKing):
        brd->castling &= 3;
        break;
    case(wRook):
        if(from == H1) {
           brd->castling &= 14;
        } else {
           brd->castling &= 13;
        }
        break;
    case(bRook):
        if(from == H8) {
           brd->castling &= 11;
        } else {
           brd->castling &= 7;
        }
        break;
    }

}

/* Executes a move */
int makeMove(Board *brd, Move move) {

    int piece, kingSquare;

    piece = brd->piecesBySquare[move.from];

    if(move.capture != noPiece) removePiece(brd,move.to);

    if (move.promotion != noPiece) {
        addPiece(brd, move.to, move.promotion);
        removePiece(brd, move.from);
    } else {
        movePiece(brd, move.from, move.to);
    }

    if (move.from == H1 && move.to == F1 && move.castling == 1) movePiece(brd,E1,G1);
    if (move.from == A1 && move.to == D1 && move.castling == 1) movePiece(brd,E1,C1);
    if (move.from == H8 && move.to == F8 && move.castling == 1) movePiece(brd,E8,G8);
    if (move.from == A8 && move.to == D8 && move.castling == 1) movePiece(brd,E8,C8);
    if (move.from == E1 && move.to == G1 && move.castling == 1) movePiece(brd,H1,F1);
    if (move.from == E1 && move.to == C1 && move.castling == 1) movePiece(brd,A1,D1);
    if (move.from == E8 && move.to == G8 && move.castling == 1) movePiece(brd,H8,F8);
    if (move.from == E8 && move.to == C8 && move.castling == 1) movePiece(brd,A8,D8);

    updateCastling(brd, move);

    brd->prevMoves[brd->numMoves++] = move;

    if ( ((piece == wKing) && (isSquareThreat(brd, move.to, BLACK)))  || ((piece == bKing) && (isSquareThreat(brd, move.to, WHITE))) )
        return FALSE;

    if(brd->side == WHITE) {
        kingSquare = brd->squaresByPieces[wKing][0];
        if(isSquareThreat(brd, kingSquare, BLACK)) {
            return FALSE;
        }
    } else {
        kingSquare = brd->squaresByPieces[bKing][0];
        if(isSquareThreat(brd, kingSquare, WHITE)) {
            return FALSE;
        }
    }

    return TRUE;

}

/* Cancels the last executed move */
void cancelMove(Board *brd) {

    Move move = brd->prevMoves[--brd->numMoves];

    if (move.promotion != noPiece) {
        removePiece(brd, move.to);
        if (pieceCol[move.promotion == WHITE]) {
           addPiece(brd, move.from, wPawn);
        } else {
           addPiece(brd, move.from, bPawn);
        }

    } else {
        movePiece(brd, move.to, move.from);
    }

    if (move.capture != noPiece) {
        addPiece(brd, move.to, move.capture);
    }

    if (move.from == H1 && move.to == F1 && move.castling == 1) movePiece(brd,G1,E1);
    if (move.from == A1 && move.to == D1 && move.castling == 1) movePiece(brd,C1,E1);
    if (move.from == H8 && move.to == F8 && move.castling == 1) movePiece(brd,G8,E8);
    if (move.from == A8 && move.to == D8 && move.castling == 1) movePiece(brd,C8,E8);
    if (move.from == E1 && move.to == G1 && move.castling == 1) movePiece(brd,F1,H1);
    if (move.from == E1 && move.to == C1 && move.castling == 1) movePiece(brd,D1,A1);
    if (move.from == E8 && move.to == G8 && move.castling == 1) movePiece(brd,F8,H8);
    if (move.from == E8 && move.to == C8 && move.castling == 1) movePiece(brd,D8,A8);

    brd->castling = brd->prevCastling[brd->numMoves];

}

