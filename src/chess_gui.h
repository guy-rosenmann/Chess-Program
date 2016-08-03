#ifndef CHESS_GUI_H
#define CHESS_GUI_H

/* Includes */
#include "constants.h"
#include "gui_framework.h"
#include "head.h"
#include "board.h"
#include "gameplay.h"
#include "move.h"
#include <stdlib.h>


/* Constants */
#define GAME_PANEL_SIZE 480


/* Globals */
extern const char* piecesIMG[15];
extern int fromSquare;
extern char firstClick;


/* Board */
#define BOARD_SIZE 8
#define BOARD_PANEL_SIZE 400
#define SQUARE_SIZE 50

#define BOARD_PLAYING 0
#define BOARD_END 1
#define BOARD_PAUSED 2

#define BOARD_BACKGROUND_IMG "./img/chess/board_background.png"
#define BOARD_IMG "./img/chess/board.png"
#define chosenIMG "./img/chess/chosen.png"
#define optionIMG "./img/chess/option.png"
#define lastIMG "./img/chess/last.png"

#define MESSAGE_WIDTH 300
#define MESSAGE_HEIGHT 30
#define BLACK_WINS_IMG "./img/chess/messages/mate_black.png"
#define WHITE_WINS_IMG "./img/chess/messages/mate_white.png"
#define CHECK_IMG "./img/chess/messages/check.png"


/* Pieces */
#define PIECE_SIZE 50

#define wPawnIMG "./img/chess/pieces/white_pawn.png"
#define wRookIMG "./img/chess/pieces/white_rook.png"
#define wKnightIMG "./img/chess/pieces/white_knight.png"
#define wBishopIMG "./img/chess/pieces/white_bishop.png"
#define wQueenIMG "./img/chess/pieces/white_queen.png"
#define wKingIMG "./img/chess/pieces/white_king.png"
#define bPawnIMG "./img/chess/pieces/black_pawn.png"
#define bRookIMG "./img/chess/pieces/black_rook.png"
#define bKnightIMG "./img/chess/pieces/black_knight.png"
#define bBishopIMG "./img/chess/pieces/black_bishop.png"
#define bQueenIMG "./img/chess/pieces/black_queen.png"
#define bKingIMG "./img/chess/pieces/black_king.png"


/* Promotions */
#define PROM_LABEL_WIDTH 240
#define PROM_LABEL_HEIGHT 50
#define PROM_BUTTON_WIDTH 220
#define PROM_BUTTON_HEIGHT 50

#define PROM_LABEL "./img/chess/promotion/promotion_label.png"
#define PROM_QUEEN_IMG "./img/chess/promotion/queen.png"
#define PROM_ROOK_IMG "./img/chess/promotion/rook.png"
#define PROM_KNIGHT_IMG "./img/chess/promotion/knight.png"
#define PROM_BISHOP_IMG "./img/chess/promotion/bishop.png"


/* Macros */
#define getSquareRow(square) ((square)/10-2)
#define getSquareCol(square) ((square)%10-1)


/* Functions */
void initGame();
int startGame();
void restartGame();
void setDifficulty(int);
void setUserColor(int);
void setGameMode(int);
int getDifficulties();
void saveGame(char*);
int loadGame(char*);
control* createGameControl();
control* createPiecesPanel(int,int);
control* createSelectionPanel(int,int);
int addPieces(control*);
int addSelections(control*);
int boardClick(control*, control*, SDL_MouseButtonEvent);
control* createGameMessageLabel(char);
int checkGUIWin(control*);
control* createPromotionPanel();
int promotionClick(control*, control*, SDL_MouseButtonEvent);
control* removePanelFromList(control*);

#define freeAndReturn(ctrl) {\
    freeControl(ctrl);\
    return NULL;\
}


#endif

