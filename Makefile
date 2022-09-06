CFLAGS= -std=c99 -Wall # Compile flags 
ALFLAGS=`pkg-config allegro-5 allegro_font-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 allegro_image-5 allegro_ttf-5 --libs --cflags` # Allegro Flags

all: game

debug: CFLAGS += -DDEBUG -g
debug: all

clean:
	-rm -f *.o *.gch

purge: clean
	-rm -f BallzGame

game:  game.o ballz.o ballz_start.o ballz_playing.o  log.o
	gcc  $(CFLAGS) game.o ballz.o ballz_start.o ballz_playing.o log.o -o BallzGame -lm $(ALFLAGS) 

game.o: game.c  
	gcc -c $(CFLAGS) game.c $(ALFLAGS)

ballz.o: ballz.c  ballz.h
	gcc -c $(CFLAGS) ballz.c $(ALFLAGS)

ballz_start.o: ballz_start.c ballz_start.h log.h
	gcc -c $(CFLAGS) ballz_start.c $(ALFLAGS)

ballz_playing.o: ballz_playing.c ballz_playing.h log.h  
	gcc -c -lm $(CFLAGS) ballz_playing.c $(ALFLAGS)

log.o: log.c log.h
	gcc -c $(CFLAGS) log.c 
