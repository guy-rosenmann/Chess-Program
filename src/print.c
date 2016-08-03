#include "print.h"

char *printSquare(int square) { /* Converts a numerical square into a string (i.e. h6) */

	static char str[3];

	int row = rows[square];
	int col = cols[square];

	sprintf(str, "%c%c", ('a'+col), ('1'+row));

	return str;

}

char *printMove(const Move move) { /* Converts a move to a string */

	static char MvStr[25];

	int fromRow = rows[move.from];
	int toRow = rows[move.to];
	int fromCol = cols[move.from];
	int toCol = cols[move.to];

	char promotion = move.promotion;
	char promChar;

	char castling = move.castling;
	if (castling) {
       sprintf(MvStr, "castle <%c,%c>", ('a'+fromCol-1), ('1'+fromRow-1));
       return MvStr;
	}


	if(promotion) {
		promChar = 'q';
		if ( (promotion == wKnight) || (promotion == bKnight) ) promChar = 'n';
		if ( (promotion == wBishop) || (promotion == bBishop) ) promChar = 'b';
		if ( (promotion == wRook) || (promotion == bRook) ) promChar = 'r';
		sprintf(MvStr, "<%c,%c> to <%c,%c> [%c]", ('a'+fromCol-1), ('1'+fromRow-1), ('a'+toCol-1), ('1'+toRow-1), promChar);
	} else {
		sprintf(MvStr, "<%c,%c> to <%c,%c>", ('a'+fromCol-1), ('1'+fromRow-1), ('a'+toCol-1), ('1'+toRow-1));
	}

	return MvStr;
}

/* Prints a move list */
void printMoveList(const MoveList *list) {

    int i;
    char *move;

    for(i = 0; i < list->numMoves; i++) {
        move = printMove(list->moves[i]);
        printf("%s\n", move);
    }

}

/* Prints a row. Is used when printing into a file while saving */
char *printRow(const Board *brd, int rowNum, char rowStr[]) {

    int col, square, piece;

    for(col = Ca; col <= Ch; col++) {
        square = CONVRC(rowNum, col);
        piece = brd->piecesBySquare[square];
        if (piece > 0) rowStr[col - 1] = pceChar[piece];
        else rowStr[col - 1] = '_';
    }
    rowStr[col - 1] = '\0';

    return rowStr;


}

