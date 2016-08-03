#ifndef GUI_FRAMEWORK_H
#define GUI_FRAMEWORK_H

/* Includes */
#include "constants.h"
#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>

/* Constants */
#define CLICK 2

/* Types */
typedef struct control control;
typedef struct coordinates coordinates;
typedef void (func) (control*);


/* Structures */
enum control_type {ctrl_window,ctrl_panel,ctrl_button,ctrl_label};

struct control {
    enum control_type ctrl_type;
    SDL_Color color;
    SDL_Rect rect;
    SDL_Surface* surface;
    char print;

    int (*drawControl)(control* self, control* window, SDL_Rect parent);
    void (*freeControl)(control* self);
    int (*clickControl)(control* self, control* window, SDL_MouseButtonEvent event);

    control* next;
    control* children;
};

struct coordinates {
    SDL_Rect from;
    SDL_Rect to;
};


/* Macros */
#define min(x,y) (((x)<(y))?(x):(y))
#define min3(x,y,z) min(min(x,y),z)
#define max(x,y) (((x)>(y))?(x):(y))
#define getRectYPos(rec) (rec.h+rec.y)
#define getRectXPos(rec) (rec.w+rec.x)
#define getYPos(ctrl) (getRectYPos(ctrl->rect))
#define getXPos(ctrl) (getRectXPos(ctrl->rect))


/* Functions */
SDL_Rect initRect(int, int, int, int);
SDL_Color initColor(int, int, int);
void changeControlColor(control*, int, int, int);
control* initControl(enum control_type, int, int, int, int);
control* createWindow(int, int, const char*);
control* createPanel(int, int, int, int);
control* createButton(int, int, int, int, const char*, int (*)(control*,control*, SDL_MouseButtonEvent));
control* createLabel(int, int, int, int, const char*);
void addChildControlLast(control*, control*);
void addChildControlFirst(control*, control*);
void addControlLast(control*, control*);
void removeChildControl(control*, control*);
void removeControlFromList(control*, control*);
void freeControl(control*);
void freeChildren(control*);
int drawWindow(control*, control*, SDL_Rect);
int drawContainer(control*, control*, SDL_Rect);
int drawChildren(control*, control*, SDL_Rect);
int drawComponent(control*, control*, SDL_Rect);
int drawControlToWindow(control*, SDL_Rect, control*, SDL_Rect);
coordinates calculateRectangles(SDL_Rect, SDL_Rect);
void setClickFunction(control*, int (*)(control*, control*, SDL_MouseButtonEvent));
int checkClickEvent(control*, SDL_Event);
int checkClick(control* ctrl, control*, SDL_Rect, SDL_MouseButtonEvent);
void checkMouseOverEvent(control*, SDL_Event);
void checkMouseOver(control*, SDL_Rect, SDL_MouseMotionEvent);

#define drawMainWindow(mainWindow) drawWindow(mainWindow,NULL,initRect(0,0,0,0))

#endif
