CFLAGS = -Wall
all: game

game: game.c ballz.c ballz.h
	gcc $(CFLAGS) game.c ballz.c -o BallzGame

clean:
	- rm *.o

purge:
	- rm BallzGame 