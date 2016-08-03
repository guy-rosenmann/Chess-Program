#include "game_framework.h"

char saveMode = LOAD_MODE;  /* Game state - playing or setting */

/* Starts the GUI game */
int startGUIGame() {
    control* mainWindow = NULL;
    SDL_Event event;
    char quit = OK;
    char click;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return ERROR;
	}
	atexit(SDL_Quit);

    mainWindow = createGameWindow();
    if (mainWindow != NULL) {
        /* Main GUI loop */
        while(quit != QUIT) {
            drawMainWindow(mainWindow);
            while (SDL_PollEvent(&event) != 0) {
                switch (event.type) {
                    case (SDL_QUIT):
                        quit = QUIT;
                        break;
                    case (SDL_MOUSEBUTTONUP):
                        click = checkClickEvent(mainWindow,event);
                        if (click == QUIT || click == ERROR) quit = QUIT;
                        break;
                    case (SDL_MOUSEMOTION):
                        checkMouseOverEvent(mainWindow,event);
                        break;
                    default:
                        break;
                }
            }
            SDL_Delay(60);
        }
        freeControl(mainWindow);
    }
    else {
        return ERROR;
    }
    return OK;
}

/* Creates the main windows */
control* createGameWindow() {
    control *mainWindow = NULL, *startPanel= NULL;
    initGame();
    mainWindow = createWindow(WIN_WIDTH,WIN_HEIGHT,"Games Program");
    if (mainWindow == NULL) return NULL;
    changeControlColor(mainWindow,0,0,0);
    startPanel = createStartPanel();
    addChildControlFirst(mainWindow,startPanel);
    if (startPanel == NULL) freeAndReturn(mainWindow);
    return mainWindow;
}

/* Creates the start panel - the first panel to show */
control* createStartPanel() {
    control *start_panel = NULL,*temp = NULL;
    int panel_width = 50 + LABEL_WIDTH, panel_height = 60 + LABEL_HEIGHT + (BUTTON_HEIGHT_BIG+20)*3;
    int button_x = (panel_width-BUTTON_WIDTH_BIG)/2;

    start_panel = createPanel((WIN_WIDTH-panel_width)/2,(WIN_HEIGHT-panel_height)/2,panel_width,panel_height);
    if (start_panel == NULL) return NULL;
    changeControlColor(start_panel,PANEL_RGB);
    /* Title Label */
    temp = createLabel((panel_width-LABEL_WIDTH)/2,15,LABEL_WIDTH,LABEL_HEIGHT,TITLE_IMG);
    addChildControlFirst(start_panel,temp);
    if (temp == NULL) freeAndReturn(start_panel);
    /* NEW GAME Button */
    temp = createButton(button_x,30+getYPos(temp),BUTTON_WIDTH_BIG,BUTTON_HEIGHT_BIG,NEW_IMG,newGameClick);
    addChildControlFirst(start_panel,temp);
    if (temp == NULL) freeAndReturn(start_panel);
    /* LOAD GAME Button */
    temp = createButton(button_x,20+getYPos(temp),BUTTON_WIDTH_BIG,BUTTON_HEIGHT_BIG,LOAD_IMG,fileClick);
    addChildControlFirst(start_panel,temp);
    if (temp == NULL) freeAndReturn(start_panel);
    /* QUIT Button */
    temp = createButton(button_x,20+getYPos(temp),BUTTON_WIDTH_BIG,BUTTON_HEIGHT_BIG,QUIT_IMG,quitClick);
    addChildControlFirst(start_panel,temp);
    if (temp == NULL) freeAndReturn(start_panel);

    return start_panel;
}

/* Creates the game mode selection panel */
control* createModePanel() {
    control *mode_panel = NULL, *temp = NULL;
    int panel_width = LABEL_WIDTH + 50, panel_height = 90+LABEL_HEIGHT*2+(BUTTON_HEIGHT_BIG)*2 + BUTTON_HEIGHT_SMALL;

    mode_panel = createPanel((WIN_WIDTH-panel_width)/2,(WIN_HEIGHT-panel_height)/2,panel_width,panel_height);
    if (mode_panel == NULL) return NULL;
    changeControlColor(mode_panel,PANEL_RGB);
    /* MODE Label */
    temp = createLabel((panel_width-LABEL_WIDTH)/2,15,LABEL_WIDTH,LABEL_HEIGHT,MODE_IMG);
    addChildControlFirst(mode_panel,temp);
    if (temp == NULL) freeAndReturn(mode_panel);
    /* PLAYER MODE Button */
    temp = createButton((panel_width-BUTTON_WIDTH_BIG)/2,30+getYPos(temp),BUTTON_WIDTH_BIG,BUTTON_HEIGHT_BIG,PLAYER_IMG,modeClick);
    addChildControlFirst(mode_panel,temp);
    if (temp == NULL) freeAndReturn(mode_panel);
    /* COMPUTER MODE Button */
    temp = createButton((panel_width-BUTTON_WIDTH_BIG)/2,20+getYPos(temp),BUTTON_WIDTH_BIG,BUTTON_HEIGHT_BIG,COMPUTER_IMG,modeClick);
    addChildControlFirst(mode_panel,temp);
    if (temp == NULL) freeAndReturn(mode_panel);
    /* CANCEL Button */
    temp = createButton((panel_width-BUTTON_WIDTH_SMALL)/2,panel_height-BUTTON_HEIGHT_SMALL-20,BUTTON_WIDTH_SMALL,BUTTON_HEIGHT_SMALL,CANCEL_IMG,cancelClick);
    addChildControlFirst(mode_panel,temp);
    if (temp == NULL) freeAndReturn(mode_panel);

    return mode_panel;
}

/* Creates the user color selection panel */
control* createSettingsColorPanel() {
    control *color_panel = NULL, *temp = NULL;
    int panel_width = LABEL_WIDTH + 50, panel_height = 90+LABEL_HEIGHT*2+(BUTTON_HEIGHT_BIG)*2 + BUTTON_HEIGHT_SMALL;

    color_panel = createPanel((WIN_WIDTH-panel_width)/2,(WIN_HEIGHT-panel_height)/2,panel_width,panel_height);
    if (color_panel == NULL) return NULL;
    changeControlColor(color_panel,PANEL_RGB);
    /* SETTINGS Label */
    temp = createLabel((panel_width-LABEL_WIDTH)/2,15,LABEL_WIDTH,LABEL_HEIGHT,SETTINGS_IMG);
    addChildControlFirst(color_panel,temp);
    if (temp == NULL) freeAndReturn(color_panel);
    /* COLOR Label */
    temp = createLabel((panel_width-LABEL_WIDTH)/2,getYPos(temp),LABEL_WIDTH,LABEL_HEIGHT,COLOR_IMG);
    addChildControlFirst(color_panel,temp);
    if (temp == NULL) freeAndReturn(color_panel);
    /* BLACK COLOR Button */
    temp = createButton((panel_width-BUTTON_WIDTH_BIG)/2,10+getYPos(temp),BUTTON_WIDTH_BIG,BUTTON_HEIGHT_BIG,WHITE_IMG,colorClick);
    addChildControlFirst(color_panel,temp);
    if (temp == NULL) freeAndReturn(color_panel);
    /* WHITE COLOR Button */
    temp = createButton((panel_width-BUTTON_WIDTH_BIG)/2,20+getYPos(temp),BUTTON_WIDTH_BIG,BUTTON_HEIGHT_BIG,BLACK_IMG,colorClick);
    addChildControlFirst(color_panel,temp);
    if (temp == NULL) freeAndReturn(color_panel);
    /* CANCEL Button */
    temp = createButton((panel_width-BUTTON_WIDTH_SMALL)/2,panel_height-BUTTON_HEIGHT_SMALL-20,BUTTON_WIDTH_SMALL,BUTTON_HEIGHT_SMALL,CANCEL_IMG,cancelClick);
    addChildControlFirst(color_panel,temp);
    if (temp == NULL) freeAndReturn(color_panel);

    return color_panel;
}

/* Creates the difficulty selection panel */
control* createSettingsDiffPanel() {
    control *difficulty_panel = NULL, *temp = NULL;
    int i,space,yPos;
    char* diff_pic = NULL;
    int num_of_diff = getDifficulties();
    int panel_width = max(LABEL_WIDTH + 50,(DIFF_BUTTON_SIZE+10)*num_of_diff + 40), panel_height = 90 + 2*LABEL_HEIGHT + 2*DIFF_BUTTON_SIZE + BUTTON_HEIGHT_SMALL;

    diff_pic = (char*)calloc(strlen(DIFFICULTY_IMG) +(int)log10(num_of_diff) + 2,sizeof(char*));
    if (diff_pic == NULL) {
        printf("ERROR: standard function malloc has failed\n");
        return NULL;
    }
    /* DIFFICULTY Panel */
    difficulty_panel = createPanel((WIN_WIDTH-panel_width)/2,(WIN_HEIGHT-panel_height)/2,panel_width,panel_height);
    if (difficulty_panel == NULL) free2AndReturn(difficulty_panel,diff_pic);
    changeControlColor(difficulty_panel,PANEL_RGB);
    /* SETTINGS Label */
    temp = createLabel((panel_width-LABEL_WIDTH)/2,15,LABEL_WIDTH,LABEL_HEIGHT,SETTINGS_IMG);
    addChildControlFirst(difficulty_panel,temp);
    if (temp == NULL) free2AndReturn(difficulty_panel,diff_pic);
    /* DIFFICULTY Label */
    temp = createLabel((panel_width-LABEL_WIDTH)/2,getYPos(temp),LABEL_WIDTH,LABEL_HEIGHT,DIFFICULTY_HEAD_IMG);
    addChildControlFirst(difficulty_panel,temp);
    if (temp == NULL) free2AndReturn(difficulty_panel,diff_pic);
    /* DIFFICULTY Buttons */
    space = max(25,(panel_width-((DIFF_BUTTON_SIZE+10)*num_of_diff-10))/2);
    yPos = getYPos(temp);
    for (i = 0; i < num_of_diff; ++i) {
        /* Gets difficulty button picture location */
        if (sprintf(diff_pic, DIFFICULTY_IMG, i+1) < 0) free2AndReturn(difficulty_panel,diff_pic);
        temp = createButton(space + (DIFF_BUTTON_SIZE+10)*i,yPos+10,DIFF_BUTTON_SIZE,DIFF_BUTTON_SIZE,diff_pic,difficultyClick);
        addChildControlFirst(difficulty_panel,temp);
        if (temp == NULL) free2AndReturn(difficulty_panel,diff_pic);
    }
    /* BEST Button */
    temp = createButton((panel_width-100)/2,getYPos(temp)+20,DIFF_BUTTON_SIZE*2,DIFF_BUTTON_SIZE,BEST_IMG,difficultyClick);
    addChildControlFirst(difficulty_panel,temp);
    if (temp == NULL) free2AndReturn(difficulty_panel,diff_pic);
    /* CANCEL Button */
    temp = createButton((panel_width-BUTTON_WIDTH_SMALL)/2,panel_height-BUTTON_HEIGHT_SMALL-20,BUTTON_WIDTH_SMALL,BUTTON_HEIGHT_SMALL,CANCEL_IMG,cancelClick);
    addChildControlFirst(difficulty_panel,temp);
    if (temp == NULL) free2AndReturn(difficulty_panel,diff_pic);

    free(diff_pic);
    return difficulty_panel;
}

/* Creates the gaming panel - side panel and game panel */
control* createGamePanel() {
    control *game_panel = NULL, *temp = NULL;

    game_panel = createPanel(0,0,WIN_WIDTH,WIN_HEIGHT);
    if (game_panel == NULL) return NULL;
    temp = createGameControl();
    addChildControlFirst(game_panel,temp);
    if (temp == NULL) freeAndReturn(game_panel);
    temp = createGameSidePanel();
    addChildControlFirst(game_panel,temp);
    if (temp == NULL) freeAndReturn(game_panel);

    return game_panel;
}

/* Creates the side panel for the game */
control* createGameSidePanel() {
    control *side_panel = NULL,*temp = NULL;
    int panel_width = 30 + BUTTON_WIDTH_SMALL, panel_height = WIN_HEIGHT;
    int button_x = (panel_width-BUTTON_WIDTH_SMALL)/2;

    side_panel = createPanel(WIN_WIDTH-panel_width,0,panel_width,panel_height);
    if (side_panel == NULL) freeAndReturn(side_panel);
    changeControlColor(side_panel,PANEL_RGB);
    /* RESTART Button */
    temp = createButton(button_x,30,BUTTON_WIDTH_SMALL,BUTTON_HEIGHT_SMALL,RESTART_IMG,restartClick);
    addChildControlFirst(side_panel,temp);
    if (temp == NULL) freeAndReturn(side_panel);
    /* SAVE Button */
    temp = createButton(button_x,getYPos(temp)+20,BUTTON_WIDTH_SMALL,BUTTON_HEIGHT_SMALL,SAVE_IMG,fileClick);
    addChildControlFirst(side_panel,temp);
    if (temp == NULL) freeAndReturn(side_panel);
    /* MENU Button */
    temp = createButton(button_x,getYPos(temp)+20,BUTTON_WIDTH_SMALL,BUTTON_HEIGHT_SMALL,MENU_IMG,menuClick);
    addChildControlFirst(side_panel,temp);
    if (temp == NULL) freeAndReturn(side_panel);
    /* QUIT Button */
    temp = createButton(button_x,panel_height-BUTTON_HEIGHT_SMALL-20,BUTTON_WIDTH_SMALL,BUTTON_HEIGHT_SMALL,SMALL_QUIT_IMG,quitClick);
    addChildControlFirst(side_panel,temp);
    if (temp == NULL) freeAndReturn(side_panel);

    return side_panel;
}

/* Creates the save/load panel */
control* createFilePanel() {
    control *file_panel = NULL,*temp = NULL;
    int i;
    char* slot_pic = NULL;
    int panel_width = LABEL_WIDTH + 50, panel_height = (BUTTON_HEIGHT_BIG+10)*SAVE_SLOTS + 130;

    slot_pic = (char*)calloc(strlen(SAVE_IMG) +(int)log10(SAVE_SLOTS) + 2,sizeof(char*));
    if (slot_pic == NULL) {
        printf("ERROR: standard function malloc has failed\n");
        return NULL;
    }
    /* FILE (Save / Load) Panel */
    file_panel = createPanel((WIN_WIDTH-panel_width)/2,(WIN_HEIGHT-panel_height)/2,panel_width,panel_height);
    if (file_panel == NULL) free2AndReturn(file_panel,slot_pic);
    changeControlColor(file_panel,PANEL_RGB);
    /* SAVE or LOAD Label */
    if (saveMode) {
        temp = createLabel((panel_width-LABEL_WIDTH)/2,10,LABEL_WIDTH,LABEL_HEIGHT,SAVE_HEAD_IMG);
    }
    else {
        temp = createLabel((panel_width-LABEL_WIDTH)/2,10,LABEL_WIDTH,LABEL_HEIGHT,LOAD_HEAD_IMG);
    }
    addChildControlFirst(file_panel,temp);
    if (temp == NULL) free2AndReturn(file_panel,slot_pic);
    /* SLOT Buttons */
    for (i = 0; i < SAVE_SLOTS; ++i) {
        /* Gets slot button picture location */
        if (sprintf(slot_pic, SLOT_IMG, i+1) < 0) free2AndReturn(file_panel,slot_pic);
        temp = createButton((panel_width-BUTTON_WIDTH_BIG)/2,70 + (BUTTON_HEIGHT_BIG+10)*i,BUTTON_WIDTH_BIG,BUTTON_HEIGHT_BIG,slot_pic,slotClick);
        addChildControlFirst(file_panel,temp);
        if (temp == NULL) free2AndReturn(file_panel,slot_pic)
    }
    /* CANCEL Button */
    temp = createButton((panel_width-BUTTON_WIDTH_SMALL)/2,panel_height-BUTTON_HEIGHT_SMALL-20,BUTTON_WIDTH_SMALL,BUTTON_HEIGHT_SMALL,CANCEL_IMG,cancelClick);
    addChildControlFirst(file_panel,temp);
    if (temp == NULL) free2AndReturn(file_panel,slot_pic);

    free(slot_pic);
    return file_panel;
}

/* Free all controls under the main window */
void clearWindow(control* window) {
    if (window != NULL) {
        freeChildren(window);
    }
}

/* New game button click function */
int newGameClick(control* ctrl, control* window, SDL_MouseButtonEvent event) {
    control* mode_panel = NULL;
    mode_panel = createModePanel();
    if (mode_panel == NULL) return ERROR;
    clearWindow(window);
    addChildControlFirst(window,mode_panel);
    return OK;
}

/* Mode selection click function */
int modeClick(control* ctrl, control* window, SDL_MouseButtonEvent event) {
    control* next_panel = NULL;
    if (ctrl->rect.y == 95) {
        clearWindow(window);
        saveMode = SAVE_MODE;
        setGameMode(PLAYER_MODE);
        next_panel = createGamePanel();
        if (next_panel == NULL) return ERROR;
        addChildControlFirst(window,next_panel);
        drawMainWindow(window);
        if (startGame() != OK) return ERROR;
    }
    else {
        clearWindow(window);
        setGameMode(COMP_MODE);
        next_panel = createSettingsColorPanel();
        if (next_panel == NULL) return ERROR;
        addChildControlFirst(window,next_panel);
    }
    return OK;
}

/* Color selection click function */
int colorClick(control* ctrl, control* window, SDL_MouseButtonEvent event) {
    control* difficulty_panel = NULL;
    if (ctrl->rect.y == 125) {
        setUserColor(WHITE);
    }
    else {
        setUserColor(BLACK);
    }
    difficulty_panel = createSettingsDiffPanel();
    if (difficulty_panel == NULL) return ERROR;
    clearWindow(window);
    addChildControlFirst(window,difficulty_panel);
    return OK;
}

/* Difficulty selection click function */
int difficultyClick(control* ctrl, control* window, SDL_MouseButtonEvent event) {
    control* game_panel = NULL;
    int num_of_diff = getDifficulties();
    int space = max(25,(max(LABEL_WIDTH + 50,(DIFF_BUTTON_SIZE+10)*num_of_diff + 40)-((DIFF_BUTTON_SIZE+10)*num_of_diff-10))/2);
    saveMode = SAVE_MODE;
    if (ctrl->rect.y == 125) {
        setDifficulty((ctrl->rect.x-space)/(DIFF_BUTTON_SIZE+10)+1);
    }
    else {
        setDifficulty(BEST_DIFFICULTY);
    }
    game_panel = createGamePanel();
    if (game_panel == NULL) return ERROR;
    clearWindow(window);
    addChildControlFirst(window,game_panel);
    drawMainWindow(window);
    if (startGame() != OK) return ERROR;
    return OK;
}

/* Main menu button click function */
int menuClick(control* ctrl, control* window, SDL_MouseButtonEvent event) {
    control* start_panel = NULL;
    saveMode = LOAD_MODE;
    restartGame();
    start_panel = createStartPanel();
    if (start_panel == NULL) return ERROR;
    clearWindow(window);
    addChildControlFirst(window,start_panel);
    return OK;
}

/* Save/Load slot button click function */
int slotClick(control* ctrl, control* window, SDL_MouseButtonEvent event) {
    control* game_panel = NULL;
    int slot = (ctrl->rect.y-70)/(BUTTON_HEIGHT_BIG+10) + 1;
    char* slot_path = NULL;

    slot_path = (char*)calloc(strlen(SAVE_PATH) +(int)log10(SAVE_SLOTS) + 2,sizeof(char*));
    if (slot_path == NULL) {
        printf("ERROR: standard function malloc has failed\n");
        return ERROR;
    }
    if (sprintf(slot_path, SAVE_PATH, slot) < 0) {
        free(slot_path);
        return ERROR;
    }
    if (saveMode == SAVE_MODE) {
        /* Saving game */
        saveGame(slot_path);
        game_panel = createGamePanel();
        if (game_panel == NULL) {
            free(slot_path);
            return ERROR;
        }
        clearWindow(window);
        addChildControlFirst(window,game_panel);
    }
    else {
        /* Loading game */
        if (loadGame(slot_path) == OK) { /* Successfuly loaded game */
            if (startGame() != OK) {
                free(slot_path);
                return ERROR;
            }
            game_panel = createGamePanel();
            saveMode = SAVE_MODE;
        }
        else { /* Failed to load game */
            game_panel = createStartPanel();
        }
        if (game_panel == NULL) {
            free(slot_path);
            return ERROR;
        }
        clearWindow(window);
        addChildControlFirst(window,game_panel);
    }
    free(slot_path);
    return OK;
}

/* Restart button click function */
int restartClick(control* ctrl, control* window, SDL_MouseButtonEvent event) {
    control* mode_panel = NULL;
    saveMode = LOAD_MODE;
    restartGame();
    mode_panel = createModePanel();
    if (mode_panel == NULL) return ERROR;
    clearWindow(window);
    addChildControlFirst(window,mode_panel);
    return OK;
}

/* Save/Load game button click function */
int fileClick(control* ctrl, control* window, SDL_MouseButtonEvent event) {
    control* file_panel = NULL;
    file_panel = createFilePanel();
    if (file_panel == NULL) return ERROR;
    clearWindow(window);
    addChildControlFirst(window,file_panel);
    return OK;
}

/* Cancel button click function */
int cancelClick(control* ctrl, control* window, SDL_MouseButtonEvent event) {
    control* next_panel = NULL;
    clearWindow(window);
    if (saveMode) {
        next_panel = createGamePanel();
    }
    else {
        next_panel = createStartPanel();
    }
    if (next_panel == NULL) return ERROR;
    addChildControlFirst(window,next_panel);
    return OK;
}

/* Quit button click function */
int quitClick(control* ctrl, control* window, SDL_MouseButtonEvent event) {
    return QUIT;
}

