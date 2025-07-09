CFLAGS=`sdl2-config --cflags --libs` -lSDL2

all:
	gcc xadrez2.c -o xadrez.out $(CFLAGS)
