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

/* Deinitialize the display and the buffer */
void disp_deinit(ALLEGRO_DISPLAY **disp, ALLEGRO_BITMAP **buffer)
{
    al_destroy_bitmap(*buffer);
    al_destroy_display(*disp);
}

/* Check the initialization of a component */
void must_init(bool teste, const char *description)
{
    if(teste)
        return;
    fprintf(stderr, "[ERROR]: Initialization of [%s] failed!\n", description);
    exit(1);
}

enum state state_start(ALLEGRO_DISPLAY **disp, ALLEGRO_BITMAP **buffer, ALLEGRO_EVENT_QUEUE *queue)
{
    ALLEGRO_EVENT event;
    bool done = false;
    unsigned char key[ALLEGRO_KEY_MAX];

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

void state_endgame()
{}