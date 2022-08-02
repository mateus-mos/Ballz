#ifndef __BALLZ__ 
#define __BALLZ__ 

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>

/* STATES */
    enum state {START, CREATE_MAP, PLAYING, GAME_OVER, ENDGAME} s_state;

/* DISPLAY */
    #define BUFFER_W 320 
    #define BUFFER_H 240 

    #define DISP_SCALE 3
    #define DISP_W (BUFFER_W * DISP_SCALE) 
    #define DISP_H (BUFFER_H * DISP_SCALE) 

/* API */
    
    enum state state_start(ALLEGRO_DISPLAY **, ALLEGRO_BITMAP **, ALLEGRO_EVENT_QUEUE *);
    void state_endgame(void);

    /* INIT AND DEINIT FUNCTIONS */
    void must_init(bool, const char*);

    void disp_init(ALLEGRO_DISPLAY **, ALLEGRO_BITMAP **);
    void disp_deinit(ALLEGRO_DISPLAY **, ALLEGRO_BITMAP **);




#endif