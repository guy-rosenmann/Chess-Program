#include "main.h"


int main(int argc, char *argv[]) {
    time_t t;
    /* Intializes random number generator - used in minimax */
    srand((unsigned) time(&t));

    switch(argc) {
        case 1:
            startConsoleGame();
            break;
        case 2:
            if (strcmp(argv[1],"console") == 0) {
                startConsoleGame();
            }
            else if (strcmp(argv[1],"gui") == 0) {
                startGUIGame();
            }
            else {
                printf("ERROR: Wrong argument\n");
            }
    }
    return 0;
}

