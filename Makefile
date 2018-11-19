CC=gcc
C_FLAGS=-Wall -g
SDL_FLAGS=`sdl-config --cflags`
SDL_LINK=`sdl-config --libs` -lSDL_ttf -lSDL_image
LIBS = -lSDL -lSDL_image -lSDL_ttf

HEADERS=jeu.h menu.h
SRC=jeu.c main.c menu.c
OBJ=$(SRC:.c=.o)

all : jeu

jeu : $(OBJ)
	$(CC) $(SDL_LINK) -o $@ $(OBJ) $(LIBS)

%.o : %.c $(HEADERS)
	$(CC) $(C_FLAGS) $(SDL_FLAGS) -c $< -o $@ $(LIBS)

clean :
	rm jeu $(OBJ)
