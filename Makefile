CFLAGS=`sdl2-config --cflags --libs` -lSDL2

all:
	gcc xadrez.c -o xadrez.out $(CFLAGS)
