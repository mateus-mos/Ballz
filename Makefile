CFLAGS= -std=c99 -Wall # Compile flags 
ALFLAGS=`pkg-config allegro-5 allegro_font-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 allegro_image-5 allegro_ttf-5 --libs --cflags` # Allegro Flags

all: game

debug: CFLAGS += -DEBUG -g
debug: all

clean:
	-rm -f *.o *.gch

purge: clean
	-rm -f BallzGame

game:  game.o ballz.o ballz_playing.o
	gcc $(CFLAGS) game.o ballz.o ballz_playing.o -o BallzGame $(ALFLAGS) 

game.o: game.c  
	gcc -c $(CFLAGS) game.c $(ALFLAGS)

ballz.o: ballz.c  ballz.h
	gcc -c $(CFLAGS) ballz.c $(ALFLAGS)

ballz_playing.o: ballz_playing.c ballz_playing.h  ballz.h
	gcc -c $(CFLAGS) ballz_playing.c $(ALFLAGS)

 
