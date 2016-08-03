#ifndef GAME_FRAMEWORK_H
#define GAME_FRAMEWORK_H

/* Includes */
#include "constants.h"
#include "gui_framework.h"
#include "chess_gui.h"
#include <SDL_video.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Game Constants */
#define SAVE_SLOTS 5
#define SAVE_PATH "./data/slot%d.xml"


/* Constants */
#define WIN_HEIGHT 480
#define WIN_WIDTH 640

#define QUIT -1


/* Globals */
extern char saveMode;

/* Panels */
#define PANEL_RGB 150,150,160


/* Buttons */
#define BUTTON_WIDTH_BIG 220
#define BUTTON_HEIGHT_BIG 50
#define BUTTON_WIDTH_SMALL 130
#define BUTTON_HEIGHT_SMALL 30
#define DIFF_BUTTON_SIZE 50

#define CANCEL_IMG "./img/buttons/cancel.png"
#define SAVE_IMG "./img/buttons/save_game.png"
#define MENU_IMG "./img/buttons/main_menu.png"
#define RESTART_IMG "./img/buttons/reastart_game.png"
#define SMALL_QUIT_IMG "./img/buttons/quit_small.png"
#define NEW_IMG "./img/buttons/new_game.png"
#define LOAD_IMG "./img/buttons/load_game.png"
#define QUIT_IMG "./img/buttons/quit.png"
#define PLAYER_IMG "./img/buttons/player.png"
#define COMPUTER_IMG "./img/buttons/computer.png"
#define BLACK_IMG "./img/buttons/color_black.png"
#define WHITE_IMG "./img/buttons/color_white.png"
#define SLOT_IMG "./img/buttons/slot_%d.png"
#define DIFFICULTY_IMG "./img/buttons/difficulty_%d.png"
#define BEST_IMG "./img/buttons/difficulty_best.png"


/* Labels */
#define LABEL_WIDTH 240
#define LABEL_HEIGHT 50

#define TITLE_IMG "./img/labels/game_title.png"
#define SETTINGS_IMG "./img/labels/game_settings.png"
#define DIFFICULTY_HEAD_IMG "./img/labels/difficulty.png"
#define COLOR_IMG "./img/labels/color.png"
#define MODE_IMG "./img/labels/game_mode.png"
#define SAVE_HEAD_IMG "./img/labels/save_game.png"
#define LOAD_HEAD_IMG "./img/labels/load_game.png"



/* Functions */
int startGUIGame();
control* createGameWindow();
control* createGamePanel();
control* createStartPanel();
control* createGameSidePanel();
control* createFilePanel();
control* createModePanel();
control* createSettingsColorPanel();
control* createSettingsDiffPanel();
void clearWindow(control*);
int newGameClick(control*,control*,SDL_MouseButtonEvent);
int modeClick(control*,control*,SDL_MouseButtonEvent);
int colorClick(control*,control*,SDL_MouseButtonEvent);
int difficultyClick(control*,control*,SDL_MouseButtonEvent);
int menuClick(control*,control*,SDL_MouseButtonEvent);
int slotClick(control*,control*,SDL_MouseButtonEvent);
int restartClick(control*,control*,SDL_MouseButtonEvent);
int fileClick(control*,control*,SDL_MouseButtonEvent);
int cancelClick(control*,control*,SDL_MouseButtonEvent);
int quitClick(control*,control*,SDL_MouseButtonEvent);

#define freeAndReturn(ctrl) {\
    freeControl(ctrl);\
    return NULL;\
}

#define free2AndReturn(ctrl,str) {\
    if (str == NULL) printf("ERROR: standard function malloc has failed\n");\
    free(str);\
    freeControl(ctrl);\
    return NULL;\
}

#endif
