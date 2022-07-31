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


    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    ALLEGRO_EVENT event;

    al_start_timer(timer);

    state = START;
    for (;;)
        switch (state)
        {
            case START: state_start(&disp, &buffer, queue) ;  break ;
            default: break ;
        }


    disp_deinit(&disp, &buffer);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}