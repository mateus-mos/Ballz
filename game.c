#include <stdlib.h>
#include "ballz.h"

int main(void)
{
    /* INITIALIZE COMPONENTS */
    must_init(al_init(), "allegro");

    ALLEGRO_DISPLAY* disp;
    ALLEGRO_BITMAP* buffer;

    ALLEGRO_TIMER* timer = al_create_timer(1.0/60.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    disp_init(&disp, &buffer);



    state = START;
    for (;;)
        switch (state)
        {
            case START: start_init() ;  break ;
            default: break ;
        }

    return 0;
}