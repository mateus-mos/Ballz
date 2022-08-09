#ifndef __BALLZ__ 
#define __BALLZ__ 

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>

#define FONTS_PATH "./Resources/Fonts/"
#define GREATE_FIGHTER_FONT "GreatFighter-pKK1.ttf"
#define PRESS_START_FONT "PressStart2P.ttf"

/* STATES */
    typedef enum {START, PLAYING, GAME_OVER, ENDGAME} State_t;

/* DISPLAY */
    #define BUFFER_W 320 
    #define BUFFER_H 240 

    #define DISP_SCALE 3
    #define DISP_W (BUFFER_W * DISP_SCALE) 
    #define DISP_H (BUFFER_H * DISP_SCALE) 

/* API */
    
    State_t state_start(ALLEGRO_DISPLAY **, ALLEGRO_BITMAP **, ALLEGRO_EVENT_QUEUE *);

    void state_endgame(void);

    /* INIT AND DEINIT FUNCTIONS */
    void ballz_init(void);
    void ballz_deinit(void);

    void disp_init(ALLEGRO_DISPLAY **, ALLEGRO_BITMAP **);
    void disp_deinit(ALLEGRO_DISPLAY **, ALLEGRO_BITMAP **);

    void must_init(bool, const char *);
    void test_ptr(bool , const char *);
    ALLEGRO_FONT* load_font(const char *, int);

/* OTHERS */
    #define TITTLE_FONT_SIZE 25
    #define TEXT_FONT_SIZE 15 

    ALLEGRO_COLOR g_primaryColor; /* Used for tittles... */
    ALLEGRO_COLOR g_secondaryColor; /* Used for normal text */
    ALLEGRO_COLOR g_buttonsColor;

    ALLEGRO_FONT *g_tittleFont;
    ALLEGRO_FONT *g_textFont;

#endif