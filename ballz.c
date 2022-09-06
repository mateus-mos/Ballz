#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ballz.h"
#include "log.h"

void ballz_init(void)
{
    srand(time(NULL));
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