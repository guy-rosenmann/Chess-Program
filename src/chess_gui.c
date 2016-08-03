#include "chess_gui.h"

/* Pieces ans selections images */
const char* piecesIMG[15] = {
    chosenIMG,
    wPawnIMG,
    wRookIMG,
    wKnightIMG,
    wBishopIMG,
    wQueenIMG,
    wKingIMG,
    bPawnIMG,
    bRookIMG,
    bKnightIMG,
    bBishopIMG,
    bQueenIMG,
    bKingIMG,
    optionIMG,
    lastIMG
};

char firstClick = TRUE;         /* Is it the first click (or second) */
int fromSquare = 0;             /* Selected piece square */
int toSquare = 0;               /* Move to square */
char boardMode = BOARD_PLAYING; /* Board status */

control* boardButton = NULL;    /* Board control - contains all pieces and selection panels */


/* Initialize game */
void initGame() {
    init();
    resetBoard(&brd);
}

/* Start game */
int startGame() {
    boardMode = BOARD_PLAYING;
    if (brd.gameMode == COMP_MODE && brd.userCol != brd.side) {
        computerMove(&brd);
        brd.side^=1;
        if (addPieces(boardButton->next) != OK) return ERROR;
    }
    return OK;
}

/* Restart game */
void restartGame() {
    resetBoard(&brd);
    firstClick = TRUE;
    boardMode = BOARD_PLAYING;
}

/* Set game difficulty */
void setDifficulty(int diff) {
    brd.difficulty = diff;
}

/* Set user color */
void setUserColor(int color) {
    brd.userCol = color;
}

/* Set game mode */
void setGameMode(int mode) {
    brd.gameMode = mode;
}

/* Get number of available difficulties (not including 'best') */
int getDifficulties() {
    return NUMBER_OF_DIFFICULTIES;
}

/* Saves the game in the given path */
void saveGame(char* path) {
    saveBoard(&brd,path);
}

/* Loads a game from given file */
int loadGame(char* path) {
    return loadBoard(&brd,path);
}

/* Creates the game panel to be inserted at the dedicated place in the main window */
control* createGameControl() {
    control *game_panel = NULL, *temp = NULL;
    int posX = (GAME_PANEL_SIZE - BOARD_PANEL_SIZE)/2;
    int posY = posX;
    game_panel = createPanel(0,0,GAME_PANEL_SIZE,GAME_PANEL_SIZE);
    if (game_panel != NULL) {
        /* Background */
        temp = createLabel(0,0,GAME_PANEL_SIZE,GAME_PANEL_SIZE,BOARD_BACKGROUND_IMG);
        addChildControlFirst(game_panel,temp);
        if (temp == NULL) freeAndReturn(game_panel);
        /* BOARD Button */
        temp = createButton(posX,posY,BOARD_PANEL_SIZE,BOARD_PANEL_SIZE,BOARD_IMG,boardClick);
        addChildControlLast(game_panel,temp);
        if (temp == NULL) freeAndReturn(game_panel);
        boardButton = temp;
        /* Pieces Panel = boardButton->next */
        temp = createPiecesPanel(posX,posY);
        addChildControlLast(game_panel,temp);
        if (temp == NULL) freeAndReturn(game_panel);
        temp->print = 0;
        /* Selection Panel = boardButton->next->next */
        temp = createSelectionPanel(posX,posY);
        addChildControlLast(game_panel,temp);
        if (temp == NULL) freeAndReturn(game_panel);
        temp->print = 0;
        /* Add message (check / mate) if needed */
        brd.side^=1;
        if (checkGUIWin(boardButton) != OK) freeAndReturn(game_panel);
        brd.side^=1;
    }
    return game_panel;
}

/* Pieces panel - contains all pieces on board */
control* createPiecesPanel(int x, int y) {
    control* pieces_panel = NULL;
    pieces_panel = createPanel(x,y,GAME_PANEL_SIZE,GAME_PANEL_SIZE);
    if (pieces_panel == NULL) return NULL;
    if (addPieces(pieces_panel) != OK) freeAndReturn(pieces_panel);
    return pieces_panel;
}

/* Selection panel - contains all available moves for selected piece */
control* createSelectionPanel(int x, int y) {
    control* selection_panel = NULL;
    selection_panel = createPanel(x,y,GAME_PANEL_SIZE,GAME_PANEL_SIZE);
    if (selection_panel == NULL) return NULL;
    if (addSelections(selection_panel) != OK) freeAndReturn(selection_panel);
    return selection_panel;
}

/* Add all the pieces on board to the given panel */
int addPieces(control* panel) {
    control* piece = NULL;
    Move last;
    int r,c,square;
    if (panel != NULL) {
        freeChildren(panel);
        for (r = R1; r <= R8; ++r) {
            for (c = Ca; c <= Ch; ++c) {
                square = CONVRC(r,c);
                if (brd.piecesBySquare[square] != noPiece) {
                    piece = createLabel((c-1)*PIECE_SIZE,(BOARD_SIZE-r)*PIECE_SIZE,PIECE_SIZE,PIECE_SIZE,piecesIMG[brd.piecesBySquare[square]]);
                    if (piece == NULL) return ERROR;
                    addChildControlFirst(panel,piece);
                }
            }
        }
    }
    /* Adds last move labels */
    if (brd.numMoves > 0) {
        last = brd.prevMoves[brd.numMoves-1];
        r = getSquareRow(last.to);
        c = getSquareCol(last.to);
        piece = createLabel(c*PIECE_SIZE,(BOARD_SIZE-r-1)*PIECE_SIZE,PIECE_SIZE,PIECE_SIZE,piecesIMG[14]);
        if (piece == NULL) return ERROR;
        addChildControlFirst(panel,piece);
        r = getSquareRow(last.from);
        c = getSquareCol(last.from);
        piece = createLabel(c*PIECE_SIZE,(BOARD_SIZE-r-1)*PIECE_SIZE,PIECE_SIZE,PIECE_SIZE,piecesIMG[14]);
        if (piece == NULL) return ERROR;
        addChildControlFirst(panel,piece);
    }
    return OK;
}

/* Adds all available moves for selected piece to the given panel */
int addSelections(control* panel) {
    control* selection = NULL;
    MoveList moveList;
    int r,c,index;
    if (panel != NULL && firstClick == FALSE) {
        freeChildren(panel);
        selection = createLabel(getSquareCol(fromSquare)*PIECE_SIZE,(BOARD_SIZE-getSquareRow(fromSquare)-1)*PIECE_SIZE,PIECE_SIZE,PIECE_SIZE,piecesIMG[0]);
        if (selection == NULL) return ERROR;
        addChildControlLast(panel,selection);
        getMoves(&brd, &moveList, fromSquare);
        for(index = 0; index < moveList.numMoves; index++) {
            r = getSquareRow(moveList.moves[index].to);
            c = getSquareCol(moveList.moves[index].to);
            selection = createLabel(c*PIECE_SIZE,(BOARD_SIZE-r-1)*PIECE_SIZE,PIECE_SIZE,PIECE_SIZE,piecesIMG[13]);
            if (selection == NULL) return ERROR;
            addChildControlLast(panel,selection);
        }
    }
    return OK;
}

/* Board panel click function - all the magic happens here */
int boardClick(control* ctrl, control* window, SDL_MouseButtonEvent event) {
    int r = BOARD_SIZE - (event.y - ctrl->rect.y)/SQUARE_SIZE, c = (event.x - ctrl->rect.x)/SQUARE_SIZE + 1;
    control* prom_panel = NULL;
    Move move;
    MoveList moveList;
    toSquare = CONVRC(r,c);
    /* If we are playing */
    if (boardMode == BOARD_PLAYING) {
        if (firstClick) {   /* This is the first click */
            if (pieceCol[brd.piecesBySquare[toSquare]] == brd.side) {
                firstClick = FALSE;
                fromSquare = toSquare;
                if (addSelections(ctrl->next->next)) return ERROR;
            }
        }
        else {  /* This is the second click */
            if (toSquare == fromSquare) {
                /* Reselected the same piece - clear selection labels */
                freeChildren(ctrl->next->next); /* Removes selections */
                firstClick = TRUE;
            }
            else if (pieceCol[brd.piecesBySquare[toSquare]] == brd.side) {
                /* Selected another piece - update selections */
                fromSquare = toSquare;
                if (addSelections(ctrl->next->next)) return ERROR;
            }
            else {
                /* Selected other square */
                move = createPlayerMoveGUI(&brd,fromSquare,toSquare);
                getMoves(&brd, &moveList, fromSquare);
                if(legalMove(&brd, move, &moveList)) {   /* If selected squares = legal move */
                    freeChildren(ctrl->next->next); /* Removes selections */
                    firstClick = TRUE;
                    if ((brd.piecesBySquare[fromSquare] == wPawn && toSquare >= A8) || (brd.piecesBySquare[fromSquare] == bPawn && toSquare <= H1)) {
                        /* If promotion is available - show promotions panel */
                        prom_panel = createPromotionPanel();
                        if (prom_panel == NULL) return ERROR;
                        boardMode = BOARD_PAUSED;
                        addControlLast(ctrl,prom_panel);
                        return OK;
                    }
                    makeMove(&brd, move);
                    if (checkGUIWin(ctrl) != OK) return ERROR;
                    brd.side^=1;
                    if (addPieces(ctrl->next) != OK) return ERROR;
                    if (brd.gameMode == COMP_MODE && boardMode == BOARD_PLAYING) {
                        /* If playing agains the computer - make computer move */
                        drawMainWindow(window);
                        computerMove(&brd);
                        if (checkGUIWin(ctrl) != OK) return ERROR;
                        brd.side^=1;
                        if (addPieces(ctrl->next) != OK) return ERROR;
                    }
                }
            }
        }
    }
    return OK;
}

/* Creates game status label (check / mate) */
control* createGameMessageLabel(char status) {
    control *message_label = NULL;
    int posX = (GAME_PANEL_SIZE - MESSAGE_WIDTH)/2;
    if (status == 2) {
        if (brd.side == WHITE) {
            message_label = createLabel(posX,5,MESSAGE_WIDTH,MESSAGE_HEIGHT,WHITE_WINS_IMG);
        }
        else {
            message_label = createLabel(posX,5,MESSAGE_WIDTH,MESSAGE_HEIGHT,BLACK_WINS_IMG);
        }
    }
    else if (status == 1) {
        message_label = createLabel(posX,5,MESSAGE_WIDTH,MESSAGE_HEIGHT,CHECK_IMG);
    }
    return message_label;
}

/* Checks board for check/mate (and displays the correct message label) */
int checkGUIWin(control* board_panel) {
    control *message_label = NULL;
    int check = checkWin(&brd);
    if (board_panel != NULL) {
        freeControl(board_panel->next->next->next);
        switch (check) {
        case 2:
            boardMode = BOARD_END;
        case 1:
            message_label = createGameMessageLabel(check);
            if (message_label == NULL) return ERROR;
            break;
        }
        board_panel->next->next->next = message_label;
    }
    return OK;
}

/* Creates the promotions panel */
control* createPromotionPanel() {
    control *prom_panel = NULL, *temp = NULL;
    int panel_width = PROM_LABEL_WIDTH + 50, panel_height = 30+PROM_LABEL_HEIGHT+(PROM_BUTTON_HEIGHT+20)*4;

    prom_panel = createPanel((GAME_PANEL_SIZE-panel_width)/2,(GAME_PANEL_SIZE-panel_height)/2,panel_width,panel_height);
    if (prom_panel == NULL) return NULL;
    changeControlColor(prom_panel,150,150,160);
    /* PROMOTION Label */
    temp = createLabel((panel_width-PROM_LABEL_WIDTH)/2,15,PROM_LABEL_WIDTH,PROM_LABEL_HEIGHT,PROM_LABEL);
    addChildControlFirst(prom_panel,temp);
    if (temp == NULL) freeAndReturn(prom_panel);
    /* ROOK Button */
    temp = createButton((panel_width-PROM_BUTTON_WIDTH)/2,15+getYPos(temp),PROM_BUTTON_WIDTH,PROM_BUTTON_HEIGHT,PROM_ROOK_IMG,promotionClick);
    addChildControlFirst(prom_panel,temp);
    if (temp == NULL) freeAndReturn(prom_panel);
    /* KNIGHT Button */
    temp = createButton((panel_width-PROM_BUTTON_WIDTH)/2,20+getYPos(temp),PROM_BUTTON_WIDTH,PROM_BUTTON_HEIGHT,PROM_KNIGHT_IMG,promotionClick);
    addChildControlFirst(prom_panel,temp);
    if (temp == NULL) freeAndReturn(prom_panel);
    /* BISHOP Button */
    temp = createButton((panel_width-PROM_BUTTON_WIDTH)/2,20+getYPos(temp),PROM_BUTTON_WIDTH,PROM_BUTTON_HEIGHT,PROM_BISHOP_IMG,promotionClick);
    addChildControlFirst(prom_panel,temp);
    if (temp == NULL) freeAndReturn(prom_panel);
    /* QUEEN Button */
    temp = createButton((panel_width-PROM_BUTTON_WIDTH)/2,panel_height-PROM_BUTTON_HEIGHT-20,PROM_BUTTON_WIDTH,PROM_BUTTON_HEIGHT,PROM_QUEEN_IMG,promotionClick);
    addChildControlFirst(prom_panel,temp);
    if (temp == NULL) freeAndReturn(prom_panel);

    return prom_panel;
}

/* Promotion choose click function - make the promotion and continue playing */
int promotionClick(control* ctrl, control* window, SDL_MouseButtonEvent event) {
    Move move;
    int promotion = (ctrl->rect.y - 20 - PROM_LABEL_HEIGHT)/(20 + PROM_BUTTON_HEIGHT);
    promotion = brd.side == WHITE ? promotion + 2 : promotion + 8;
    boardMode = BOARD_PLAYING;
    move = createPlayerMoveGUI(&brd,fromSquare,toSquare);
    move.promotion = promotion;
    makeMove(&brd, move);
    freeControl(removePanelFromList(boardButton->next->next));
    if (checkGUIWin(boardButton) != OK) return ERROR;
    brd.side^=1;
    if (addPieces(boardButton->next) != OK) return ERROR;
    if (brd.gameMode == COMP_MODE) {   /* If against the computer - make computer move */
        drawMainWindow(window);
        computerMove(&brd);
        if (checkGUIWin(boardButton) != OK) return ERROR;
        brd.side^=1;
        if (addPieces(boardButton->next) != OK) return ERROR;
    }
    return OK;
}

/* Removes first label from controls list - and returns the label */
control* removePanelFromList(control* head) {
    control* curr = head;
    if (curr != NULL) {
        while (curr != NULL && curr->ctrl_type != ctrl_label) {
            curr = curr->next;
        }
        removeControlFromList(head,curr);
    }
    return curr;
}
