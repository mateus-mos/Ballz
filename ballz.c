#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ballz.h"
#include "log.h"

/* Position of the button "Play" in the hud of the state START */
#define START_BUTTON_PLAY_A_X (BUFFER_W / 12 * 5)
#define START_BUTTON_PLAY_A_Y ((BUFFER_H / 12) * 6)
#define START_BUTTON_PLAY_B_X ((BUFFER_W / 12) * 7)
#define START_BUTTON_PLAY_B_Y ((BUFFER_H / 12) * 7)

/* PROTOTYPES */
void hud_start_draw(ALLEGRO_FONT*, ALLEGRO_FONT*);

void ballz_init(void)
{
    srand(time(NULL));
}

void ballz_deinit(void)
{
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

ALLEGRO_FONT* load_font(const char *font_name, int font_size)
{
    log_test_ptr(font_name, "load_font", "font_name");
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

void hud_start_draw(ALLEGRO_FONT* tittle_font, ALLEGRO_FONT* text_font)
{

    /* Shadow Tittle */
    al_draw_text(
        tittle_font,
        SECONDARY_COLOR,
        BUFFER_W / 2 + BUFFER_W/90, 
        BUFFER_H / 5,
        ALLEGRO_ALIGN_CENTER,
        "Ballz"
    );

    /* Tittle */
    al_draw_text(
        tittle_font,
        PRIMARY_COLOR,
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
        SECONDARY_COLOR 
    );
    
    /* Play button */
    al_draw_filled_rounded_rectangle(
        START_BUTTON_PLAY_A_X,
        START_BUTTON_PLAY_A_Y,
        START_BUTTON_PLAY_B_X,
        START_BUTTON_PLAY_B_Y,
        10,
        10,
        PRIMARY_COLOR 
    );

    /* Play button g_textFont */
    al_draw_text(
        text_font,
        SECONDARY_COLOR, 
        /* Define position of the g_textFont based on the start button */
        START_BUTTON_PLAY_A_X + (START_BUTTON_PLAY_B_X - START_BUTTON_PLAY_A_X)/2,
        START_BUTTON_PLAY_A_Y + (START_BUTTON_PLAY_B_Y - START_BUTTON_PLAY_A_Y)/6,
        ALLEGRO_ALIGN_CENTER,
        "Play"
    );
}

State_t state_start(ALLEGRO_DISPLAY **disp, ALLEGRO_BITMAP **buffer, ALLEGRO_EVENT_QUEUE *queue)
{
    #ifdef DEBUG
        log_info("state_start", "State START initialized");
    #endif

    State_t state = START;
    ALLEGRO_EVENT event;
    ALLEGRO_MOUSE_STATE mouse_state;
    ALLEGRO_FONT * tittle_font;
    ALLEGRO_FONT * text_font;

    tittle_font = load_font(DEBUG_FONT, TITTLE_FONT_SIZE);
    log_test_ptr(tittle_font, "state_start", "tittle_font");

    text_font = load_font(DEBUG_FONT, TEXT_FONT_SIZE);
    log_test_ptr(text_font, "state_start", "text_font");

    while(state == START)
    {

        al_wait_for_event(queue, &event);

        switch (event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                    disp_pre_draw(*buffer);
                    al_clear_to_color(al_map_rgb(0,0,0));

                    hud_start_draw(tittle_font, text_font);

                    disp_post_draw(*disp, *buffer);
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                    al_get_mouse_state(&mouse_state);

                    if(play_button_clicked(&mouse_state))
                    {
                        #ifdef DEBUG
                            log_info("state_start", "Button 'Play' pressed!");
                            log_info("state_start", "Change state to PLAY!");
                        #endif
                        state = PLAYING;
                    }

                    #ifdef DEBUG
                        log_info("state_start", "Mouse Pressed!");
                    #endif
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    state = ENDGAME;
                break;
        }
    }

    al_destroy_font(tittle_font);
    al_destroy_font(text_font);
    return state;
}