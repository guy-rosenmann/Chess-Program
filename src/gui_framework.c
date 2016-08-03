#include "gui_framework.h"


/* Initialize and return SDL_Rect */
SDL_Rect initRect(int x, int y, int width, int height) {
    SDL_Rect rect;
    rect.x = (Sint16)x;
    rect.y = (Sint16)y;
    rect.h = (Uint16)height;
    rect.w = (Uint16)width;
    return rect;
}


/* Initialize and return SDL_Color */
SDL_Color initColor(int r, int g, int b) {
    SDL_Color color;
    color.r = (Uint8)r;
    color.g = (Uint8)g;
    color.b = (Uint8)b;
    return color;
}


/* Returns an initialized control */
control* initControl(enum control_type ctrl_type, int x, int y, int width, int height) {
    control* ctrl = NULL;
    ctrl = (control*) malloc(sizeof(control));
    if (ctrl != NULL) {
        ctrl->ctrl_type = ctrl_type;
        ctrl->rect = initRect(x,y,width,height);
        ctrl->color = initColor(255,255,255); /* default color: white */
        ctrl->print = TRUE;
        ctrl->freeControl = &freeControl;
        ctrl->surface = NULL;
        ctrl->next = NULL;
        ctrl->children = NULL;
    }
    else {
        printf("ERROR: standard function malloc has failed\n");
    }
    return ctrl;
}


/* Changes the color of the control */
void changeControlColor(control* ctrl, int r, int g, int b) {
    ctrl->color = initColor(r,g,b);
}


/* Changes the 'click' function of the button control */
void setClickFunction(control* self, int (*func)(control* ctrl, control* window, SDL_MouseButtonEvent event)) {
    if (self != NULL && self->ctrl_type == ctrl_button) {
        self->clickControl = func;
    }
}


/* Creates a GUI window control */
control* createWindow(int width, int height, const char* title) {
    control* window = NULL;
    SDL_Surface *w = NULL;
    window = initControl(ctrl_window,0,0,width,height);
    if (window != NULL) {
        w  = SDL_SetVideoMode(width, height, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
        if (w == NULL) {
            printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
            free(window);
            window = NULL;
        }
        else {
            SDL_WM_SetCaption(title, title);
            window->surface = w;
            window->drawControl = &drawWindow;
        }
    }
    return window;
}


/* Creates a panel control */
control* createPanel(int x, int y, int width, int height) {
    control* panel = NULL;
    panel = initControl(ctrl_panel,x,y,width,height);
    if (panel != NULL) {
        panel->drawControl = &drawContainer;
    }
    return panel;
}


/* creates a button control */
control* createButton(int x, int y, int width, int height, const char* img_path, int (*func)(control*, control*, SDL_MouseButtonEvent)) {
    control* button = NULL;
    SDL_Surface *img = NULL;
    button = initControl(ctrl_button,x,y,width,height);
    if (button != NULL) {
        button->print = FALSE;
        img = IMG_Load(img_path);
        if (img == NULL) {
            printf("ERROR: failed to load image: %s\n", IMG_GetError());
            free(button);
            button = NULL;
        }
        else {
            button->surface = img;
            button->drawControl = &drawComponent;
            button->clickControl = func;
        }
    }
    return button;
}


/* Creates a label control */
control* createLabel(int x, int y, int width, int height, const char* img_path) {
    control* label = NULL;
    SDL_Surface *img = NULL;
    label = initControl(ctrl_label,x,y,width,height);
    if (label != NULL) {
        img = IMG_Load(img_path);
        if (img == NULL) {
            printf("ERROR: failed to load image: %s\n", IMG_GetError());
            free(label);
            label = NULL;
        }
        else {
            label->surface = img;
            label->drawControl = &drawComponent;
        }
    }
    return label;
}


/* Adds child control (last in the children list) to a given windows/panel */
void addChildControlLast(control* parent, control* child) {
    control *curr = NULL;
    if (parent != NULL && child != NULL) {
        if (parent->ctrl_type == ctrl_window || parent->ctrl_type == ctrl_panel) {
            curr = parent->children;
            if (curr == NULL) {
                parent->children = child;
            }
            else {
                while (curr->next != NULL) {
                    curr = curr->next;
                }
                curr->next = child;
            }
        }
    }
}


/* Adds child control (first in the children list) to a given windows/panel */
void addChildControlFirst(control* parent, control* child) {
    if (parent != NULL && child != NULL) {
        if (parent->ctrl_type == ctrl_window || parent->ctrl_type == ctrl_panel) {
            child->next = parent->children;
            parent->children = child;
        }
    }
}


/* Adds control to the end of controls list */
void addControlLast(control* head, control* newCtrl) {
    if (head != NULL && newCtrl != NULL) {
        while (head->next != NULL) {
            head = head->next;
        }
        head->next = newCtrl;
    }
}


/* Removes control from the children list of a given control */
void removeChildControl(control* parent, control* child) {
    control *curr = NULL;
    if (parent != NULL && child != NULL) {
        if (parent->children == child) {
            parent->children = child->next;
            child->next = NULL;
        }
        else {
            curr = parent->children;
            while (curr != NULL && curr->next != child) {
                curr = curr->next;
            }
            if (curr != NULL) {
                curr->next = curr->next->next;
                child->next = NULL;
            }
        }
    }
}


/* Removes control from the given controls list */
void removeControlFromList(control* head, control* ctrl) {
    if (head != NULL && ctrl != NULL) {
        if (head->next == ctrl) {
            head->next = ctrl->next;
            ctrl->next = NULL;
        }
        else {
            while (head != NULL && head->next != ctrl) {
                head = head->next;
            }
            if (head != NULL) {
                head->next = head->next->next;
                ctrl->next = NULL;
            }
        }
    }
}


/* Frees the given control */
void freeControl(control* ctrl) {
    if (ctrl != NULL) {
        if (ctrl->surface != NULL) {
            SDL_FreeSurface(ctrl->surface);
        }
        if (ctrl->children != NULL) {
            freeControl(ctrl->children);
        }
        if (ctrl->next != NULL) {
            freeControl(ctrl->next);
        }
        free(ctrl);
    }
}


/* Frees all children of given control */
void freeChildren(control* ctrl) {
    if (ctrl != NULL) {
        freeControl(ctrl->children);
        ctrl->children = NULL;
    }
}


/* Draw function of a window control */
int drawWindow(control* self, control* dummy_window, SDL_Rect dummy_parent) {
    int err = OK;
    if ((self != NULL) && (self->ctrl_type == ctrl_window)) {
        err = drawContainer(self,self,self->rect);
        if (err == OK) {
            if ((err = SDL_Flip(self->surface)) != OK) {
                printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
            }
        }
    }
    return err;
}


/* Draw function of a container (window/panel) control */
int drawContainer(control* self, control* window, SDL_Rect parent) {
    coordinates calculated_rects;
    int err = OK;
    if (self != NULL && window != NULL) {
        if (self->ctrl_type == ctrl_window || self->ctrl_type == ctrl_panel) {
            err = drawComponent(self,window,parent);
            if (err == OK) {
                calculated_rects = calculateRectangles(parent,self->rect);
                drawChildren(self,window,calculated_rects.to);
            }
        }
    }
    return err;
}


/* Draws all children of a container control */
int drawChildren(control* self, control* window, SDL_Rect parent) {
    control* child = NULL;
    int err = OK;
    if (self != NULL) {
        child = self->children;
        while (child != NULL && err == OK) {
            err = child->drawControl(child,window,parent);
            child = child->next;
        }
    }
    return err;
}


/* Draw function of a component (control) */
int drawComponent(control* self, control* window, SDL_Rect parent) {
    coordinates calculated_rects;
    if (self != NULL && window != NULL) {
        calculated_rects = calculateRectangles(parent,self->rect);
        if (calculated_rects.from.w > 0 && calculated_rects.from.h > 0) {
            return drawControlToWindow(self,calculated_rects.from,window,calculated_rects.to);
        }
    }
    return OK;
}


/* Draws the control to the given window */
int drawControlToWindow(control* ctrl, SDL_Rect ctrl_rect, control* window, SDL_Rect window_rect) {
    int err = OK;
    if (ctrl != NULL && window != NULL) {
        switch(ctrl->ctrl_type) {
            case ctrl_panel:
            case ctrl_window:
                if (ctrl->print) err = SDL_FillRect(window->surface, &window_rect, SDL_MapRGB(window->surface->format, ctrl->color.r, ctrl->color.g, ctrl->color.b));
                if (err != OK) printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
                break;
            case ctrl_button:
                if (ctrl->print) ctrl_rect.x = ctrl_rect.x + ctrl_rect.w;
            case ctrl_label:
                err = SDL_BlitSurface(ctrl->surface,&ctrl_rect,window->surface,&window_rect);
                if (err != OK) printf("ERROR: failed to blit image: %s\n", SDL_GetError());
                break;
        }
    }
    return err;
}


/* Calculates the absolute rectangle position of the control from the relative rectangle */
coordinates calculateRectangles(SDL_Rect parent, SDL_Rect child) {
    coordinates result;
    int tmp;
    SDL_Rect from_rect, to_rect;
    if (child.x < 0) {
        from_rect.x = -child.x;
        to_rect.x = parent.x;
    }
    else {
        from_rect.x = 0;
        to_rect.x = parent.x + child.x;
    }
    if (child.y < 0) {
        from_rect.y = -child.y;
        to_rect.y = parent.y;
    }
    else {
        from_rect.y = 0;
        to_rect.y = parent.y + child.y;
    }
    tmp = min3(child.w, getRectXPos(parent) - to_rect.x, getRectXPos(child));
    from_rect.w = (tmp < 0) ? 0 : tmp;
    tmp = min3(child.h, getRectYPos(parent) - to_rect.y, getRectYPos(child));
    from_rect.h = (tmp < 0) ? 0 : tmp;
    to_rect.w = from_rect.w;
    to_rect.h = from_rect.h;
    result.from = from_rect;
    result.to = to_rect;
    return result;
}


/* Chescks for a mouse click event in the given window */
int checkClickEvent(control* window, SDL_Event event) {
    SDL_MouseButtonEvent e = event.button;
    if (window != NULL) {
        if (window->ctrl_type == ctrl_window) {
            return checkClick(window,window,window->rect,e);
        }
    }
    return OK;
}


/* Checks for a click and clicks the control */
int checkClick(control* ctrl, control* window, SDL_Rect parent, SDL_MouseButtonEvent event) {
    coordinates calculated_rects;
    SDL_Rect ctrl_rect;
    int err;
    if (ctrl != NULL) {
        err = checkClick(ctrl->next,window,parent,event);
        if (err != OK) return err;
        calculated_rects = calculateRectangles(parent,ctrl->rect);
        ctrl_rect = calculated_rects.to;
        switch(ctrl->ctrl_type) {
            case ctrl_window:
            case ctrl_panel:
                err = checkClick(ctrl->children,window,ctrl_rect,event);
                if (err != OK) return err;
                break;
            case ctrl_button:
                if ((event.x >= ctrl_rect.x) && (event.x <= (ctrl_rect.x+ctrl_rect.w)) && (event.y >= ctrl_rect.y) && (event.y <= (ctrl_rect.y+ctrl_rect.h))) {
                    err = ctrl->clickControl(ctrl,window,event);
                    if (err == OK) return CLICK;
                    return err;
                }
                break;
            default:
                break;
        }
    }
    return OK;
}


/* Chescks for a mouse over event in the given window */
void checkMouseOverEvent(control* window, SDL_Event event) {
    SDL_MouseMotionEvent e = event.motion;
    if (window != NULL) {
        if (window->ctrl_type == ctrl_window) {
            checkMouseOver(window,window->rect,e);
        }
    }
}


/* Checks for a mouse over and updates button sprite img */
void checkMouseOver(control* ctrl, SDL_Rect parent, SDL_MouseMotionEvent event)  {
    coordinates calculated_rects;
    SDL_Rect ctrl_rect;
    if (ctrl != NULL) {
        calculated_rects = calculateRectangles(parent,ctrl->rect);
        ctrl_rect = calculated_rects.to;
        checkMouseOver(ctrl->next,parent,event);
        checkMouseOver(ctrl->children,ctrl_rect,event);
        if (ctrl->ctrl_type == ctrl_button) {
            if ((event.x >= ctrl_rect.x) && (event.x <= (ctrl_rect.x+ctrl_rect.w)) && (event.y >= ctrl_rect.y) && (event.y <= (ctrl_rect.y+ctrl_rect.h))) {
                ctrl->print = TRUE;
            }
            else {
                ctrl->print = FALSE;
            }
        }
    }
}

