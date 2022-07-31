CFLAGS=-Wall # Compile flags 
ALFLAGS=`pkg-config allegro-5 allegro_font-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 allegro_image-5 --libs --cflags` # Allegro Flags

all: game

game: game.c ballz.c ballz.h
	gcc $(CFLAGS) game.c ballz.c -o BallzGame $(ALFLAGS)

clean:
	-rm -f *.o

purge: 
	-rm -f *.o BallzGame 