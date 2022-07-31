#include <stdio.h>
#include "ballz.h"

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

/* Check the initialization of a component */
void must_init(bool teste, const char *description)
{
    if(teste)
        return;
    fprintf(stderr, "[ERROR]: Initialization of [%s] failed!\n", description);
    exit(1);
}

void start_init()
{
}