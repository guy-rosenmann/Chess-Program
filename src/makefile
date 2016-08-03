PROGRAM = chessprog
FLAGS_C = -ansi -pedantic-errors -c -Wall -g -lm
FLAGS_L = -lm -ansi -pedantic-errors -g
SDL_C = `sdl-config --cflags` -I/usr/local/lib/SDL_image-1.2.12/include/SDL
SDL_L = `sdl-config --libs` -L/usr/local/lib/SDL_image-1.2.12/lib/ -lSDL_image
GAME_OBJECTS = board.o gamedata.o gameplay.o init.o minimax.o move.o print.o
GUI_OBJECTS = gui_framework.o game_framework.o chess_gui.o
DEPS = head.h

all: $(PROGRAM)

clean:
	-rm $(GAME_OBJECTS) $(GUI_OBJECTS) main.o $(PROGRAM)

$(PROGRAM): main.o $(GAME_OBJECTS) $(GUI_OBJECTS)
	gcc -o $@ $^ $(FLAGS_L) $(SDL_L)

main.o: main.c main.h game_framework.h $(DEPS)
	gcc $(FLAGS_C) $< $(SDL_C)

gui_framework.o: gui_framework.c gui_framework.h
	gcc $(FLAGS_C) $< $(SDL_C)

game_framework.o: game_framework.c game_framework.h chess_gui.h gui_framework.h
	gcc $(FLAGS_C) $< $(SDL_C)

chess_gui.o: chess_gui.c chess_gui.h gui_framework.h $(DEPS)
	gcc $(FLAGS_C) $< $(SDL_C)

board.o: board.c board.h gamedata.h gameplay.h init.h print.h $(DEPS)
	gcc $(FLAGS_C) $<

gameplay.o: gameplay.c gameplay.h board.h gamedata.h minimax.h move.h print.h $(DEPS)
	gcc $(FLAGS_C) $<

minimax.o: minimax.c minimax.h gameplay.h $(DEPS)
	gcc $(FLAGS_C) $<

move.o: move.c move.h gamedata.h board.h $(DEPS)
	gcc $(FLAGS_C) $<

print.o: print.c print.h gamedata.h init.h $(DEPS)
	gcc $(FLAGS_C) $<

%.o: %.c %.h $(DEPS)
	gcc $(FLAGS_C) $<
