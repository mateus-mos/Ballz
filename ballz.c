#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ballz.h"

/* PROTOTYPES */
void hud_start_draw(ALLEGRO_FONT*);

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

ALLEGRO_FONT* load_font(const char *font_name, int font_size)
{
    test_ptr(font_name, "font_name");
    char font_path_buffer[100];

    strcpy(font_path_buffer, FONTS_PATH);
    strcat(font_path_buffer, font_name); 

    return al_load_font(font_path_buffer, font_size, 0);
}

State_t state_start(ALLEGRO_DISPLAY **disp, ALLEGRO_BITMAP **buffer, ALLEGRO_EVENT_QUEUE *queue)
{
    ALLEGRO_EVENT event;
    bool done = false;
    unsigned char key[ALLEGRO_KEY_MAX];

    ALLEGRO_FONT* font;
    font = load_font(ATARI_CLASSIC_FONT, 15);

    test_ptr(font, "font");


    disp_pre_draw(*buffer);

    hud_start_draw(font);

    disp_post_draw(*disp, *buffer);

    while(1)
    {
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done)
            return ENDGAME;
    }
}

void hud_start_draw(ALLEGRO_FONT* font)
{
    al_draw_text(
        font,
        al_map_rgb_f(1, 1, 1),
        BUFFER_W / 2,
        BUFFER_H / 5,
        ALLEGRO_ALIGN_CENTER,
        "Ballz"
    );
}


void state_endgame()
{}