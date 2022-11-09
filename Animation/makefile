SRC=Animation.c
#SRC+=world.c etc1.c etc2.c
#SRC=$(wildcard *.c)  # en commentaire, je ne suis pas un grand amateur
EXE=Anim

CC=gcc
CFLAGS:=-Wall -Wextra -MMD -Og -g $(sdl2-config --cflags)
#CFLAGS:=-Wall -Wextra -MMD -O2 $(sdl2-config --cflags)  # pour la version release
LDFLAGS:=-Wall -Wextra -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lm -lSDL2

all: $(OBJ) 
		$(CC) $(SRC) -o $(EXE) $^ $(LDFLAGS)

clean:
		rm -rf build core *.o

