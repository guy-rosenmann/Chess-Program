#include "init.h"

int bigToSmall[NUM_SQUARE]; /* Arrays which convert from 64 to 120 and vice versa */
int smallToBig[64];

int rows[NUM_SQUARE]; /*Arrays which assign squares their rank and file info */
int cols[NUM_SQUARE];

/* Initializes the rows and column arrays */
void initRowAndCol() {

	int index, row, col, square;

	for(index = 0; index < NUM_SQUARE; ++index) {
		rows[index] = NOT_ON_BOARD;
		cols[index] = NOT_ON_BOARD;
	}

	for(row = R1; row <= R8; row++) {
		for(col = Ca; col <= Ch; col++) {
			square = CONVRC(row,col);
			rows[square] = row;
			cols[square] = col;
		}
	}
}

/* Initializes the square conversion arrays */
void initConversionArrays() {

	int index = 0;
	int row = R1;
	int col = Ca;
	int square;
	int square64 = 0;

	for(index = 0; index < NUM_SQUARE; ++index) {
		bigToSmall[index] = 65;
	}

	for(index = 0; index < 64; ++index) {
		smallToBig[index] = 0;
	}

	for(row = R1; row <= R8; ++row) {
		for(col = Ca; col <= Ch; ++col) {
			square = CONVRC(row,col);
			smallToBig[square64] = square;
			bigToSmall[square] = square64;
			square64++;
		}
	}
}

void init() {
    initRowAndCol();
    initConversionArrays();
}
