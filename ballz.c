#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ballz.h"

/* Position of the button "Play" in the hud of the state START */
#define START_BUTTON_PLAY_A_X (BUFFER_W / 12 * 5)
#define START_BUTTON_PLAY_A_Y ((BUFFER_H / 12) * 6)
#define START_BUTTON_PLAY_B_X ((BUFFER_W / 12) * 7)
#define START_BUTTON_PLAY_B_Y ((BUFFER_H / 12) * 7)

#define TITTLE_FONT_SIZE 25
#define TEXT_FONT_SIZE 15 

ALLEGRO_COLOR g_primaryColor; /* Used for tittles... */
ALLEGRO_COLOR g_secondaryColor; /* Used for normal text */
ALLEGRO_COLOR g_buttonsColor;

ALLEGRO_FONT *g_tittleFont;
ALLEGRO_FONT *g_textFont;

/* PROTOTYPES */
ALLEGRO_FONT* load_font(const char *, int);
void hud_start_draw(ALLEGRO_FONT*, ALLEGRO_FONT*);
void ballz_color_init(void);
void test_ptr(bool , const char *);


bool ballz_font_init(void)
{
    g_tittleFont = load_font(GREATE_FIGHTER_FONT, TITTLE_FONT_SIZE);
    test_ptr(g_tittleFont, "g_tittleFont");

    g_textFont = load_font(GREATE_FIGHTER_FONT, TEXT_FONT_SIZE);
    test_ptr(g_textFont, "g_textFont");

    return true;
};

void ballz_font_deinit(void)
{
    al_destroy_font(g_tittleFont);
    al_destroy_font(g_textFont);
}

void ballz_init(void)
{
    srand(time(NULL));

    must_init(ballz_font_init(), "ballz font");
    ballz_color_init();
}

void ballz_deinit(void)
{
    ballz_font_deinit();
}

/* Initialize the display and the buffer */
void disp_init(ALLEGRO_DISPLAY **disp, ALLEGRO_BITMAP **buffer)
{
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    *disp = al_create_display(DISP_W, DISP_H);
    must_init(*disp, "display");

    *buffer = al_create_bitmap(BUFFER_W, BUFFER_H);
    must_init(*buffer, "bitmap buffer");
}

/* Deinitialize the display and the buffer */
void disp_deinit(ALLEGRO_DISPLAY **disp, ALLEGRO_BITMAP **buffer)
{
    al_destroy_bitmap(*buffer);
    al_destroy_display(*disp);
}

void disp_pre_draw(ALLEGRO_BITMAP *buffer)
{
    al_set_target_bitmap(buffer);
}

void disp_post_draw(ALLEGRO_DISPLAY *disp, ALLEGRO_BITMAP *buffer)
{
    al_set_target_backbuffer(disp);
    al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISP_W, DISP_H, 0);

    al_flip_display();
}

/* Check the initialization of a component */
void must_init(bool test, const char *description)
{
    if(test)
        return;

    fprintf(stderr, "[ERROR]: Initialization of [%s] failed!\n", description);
    exit(EXIT_FAILURE);
}

/* Test if a pointer is NULL, if so exit */
void test_ptr(bool test, const char *description)
{
    if(test)
        return;

    fprintf(stderr, "[ERROR]: [%s] pointer is NULL!\n", description);
    exit(EXIT_FAILURE);
}

/* Initialize colors, dont need a deinit function */
void ballz_color_init(void)
{
    g_primaryColor.r = (float)(242)/(float)(255);
    g_primaryColor.g = (float)(226)/(float)(255);
    g_primaryColor.b = (float)(5)/(float)(255);
    g_primaryColor.a = 1;

    g_secondaryColor.r = (float)(242)/(float)(255);
    g_secondaryColor.g = (float)(5)/(float)(255);
    g_secondaryColor.b = (float)(203)/(float)(255);
    g_secondaryColor.a = 1;

    g_buttonsColor.r = 1;
    g_buttonsColor.g = 0;
    g_buttonsColor.b = (float)(116)/(float)(255);
    g_buttonsColor.a = 1;
}

ALLEGRO_FONT* load_font(const char *font_name, int font_size)
{
    test_ptr(font_name, "font_name");
    char font_path_buffer[100];

    strcpy(font_path_buffer, FONTS_PATH);
    strcat(font_path_buffer, font_name); 

    return al_load_font(font_path_buffer, font_size, ALLEGRO_TTF_MONOCHROME);
}

bool collide(int a_x1, int a_y1, int a_x2, int a_y2, int b_x1, int b_y1, int b_x2, int b_y2)
{
    if(a_x1 > b_x2)
        return false;
    if(a_x2 < b_x1)
        return false;
    if(a_y1 > b_y2)
        return false;
    if(a_y2 < b_y1)
        return false;

    return true;
}


bool play_button_clicked(ALLEGRO_MOUSE_STATE *mouse_state)
{
    return collide
    (
        mouse_state->x,
        mouse_state->y,
        mouse_state->x,
        mouse_state->y,
        /* Multiply by DISP_SCALE because the START_BUTTON coordinate is relative to the buffer */
        /* and collide the mouse coordinate is relative to the display */
        START_BUTTON_PLAY_A_X * DISP_SCALE,
        START_BUTTON_PLAY_A_Y * DISP_SCALE,
        START_BUTTON_PLAY_B_X * DISP_SCALE,
        START_BUTTON_PLAY_B_Y * DISP_SCALE
    );
}

State_t state_start(ALLEGRO_DISPLAY **disp, ALLEGRO_BITMAP **buffer, ALLEGRO_EVENT_QUEUE *queue)
{
    bool done = false;
    State_t state = START;
    ALLEGRO_EVENT event;
    ALLEGRO_MOUSE_STATE mouse_state;
    unsigned char key[ALLEGRO_KEY_MAX];


    while(state == START)
    {

        al_wait_for_event(queue, &event);


        switch (event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                    disp_pre_draw(*buffer);
                    al_clear_to_color(al_map_rgb(0,0,0));

                    hud_start_draw(g_tittleFont, g_textFont);

                    disp_post_draw(*disp, *buffer);
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                    al_get_mouse_state(&mouse_state);

                    if(play_button_clicked(&mouse_state))
                    {
                        fprintf(stderr, "[INFO]: Button 'Play' pressed! \n");
                        fprintf(stderr, "[INFO]: Change state to PLAY! \n");
                        //state = PLAY;
                    }

                    fprintf(stderr, "[INFO]: Mouse pressed! x: %d, y: %d\n", mouse_state.x, mouse_state.y);
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    state = ENDGAME;
                break;
        }
    }

    return state;
}


void hud_start_draw(ALLEGRO_FONT* g_tittleFont, ALLEGRO_FONT* g_textFont)
{

    /* Shadow Tittle */
    al_draw_text(
        g_tittleFont,
        g_secondaryColor,
        BUFFER_W / 2 + BUFFER_W/90, 
        BUFFER_H / 5,
        ALLEGRO_ALIGN_CENTER,
        "Ballz"
    );

    /* Tittle */
    al_draw_text(
        g_tittleFont,
        g_primaryColor,
        BUFFER_W / 2,
        BUFFER_H / 5,
        ALLEGRO_ALIGN_CENTER,
        "Ballz"
    );

    /* Shadow play button */
    al_draw_filled_rounded_rectangle(
        START_BUTTON_PLAY_A_X + START_BUTTON_PLAY_A_X/90,
        START_BUTTON_PLAY_A_Y + START_BUTTON_PLAY_A_Y/90,
        START_BUTTON_PLAY_B_X + START_BUTTON_PLAY_B_X/90,
        START_BUTTON_PLAY_B_Y + START_BUTTON_PLAY_B_Y/90,
        10,
        10,
        g_secondaryColor 
    );
    
    /* Play button */
    al_draw_filled_rounded_rectangle(
        START_BUTTON_PLAY_A_X,
        START_BUTTON_PLAY_A_Y,
        START_BUTTON_PLAY_B_X,
        START_BUTTON_PLAY_B_Y,
        10,
        10,
        g_primaryColor 
    );

    /* Play button g_textFont */
    al_draw_text(
        g_textFont,
        g_secondaryColor, 
        /* Define position of the g_textFont based on the start button */
        START_BUTTON_PLAY_A_X + (START_BUTTON_PLAY_B_X - START_BUTTON_PLAY_A_X)/2,
        START_BUTTON_PLAY_A_Y + (START_BUTTON_PLAY_B_Y - START_BUTTON_PLAY_A_Y)/6,
        ALLEGRO_ALIGN_CENTER,
        "Play"
    );
}


void state_endgame()
{}