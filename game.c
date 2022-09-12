#include "ballz.h"
#include "ballz_start.h"
#include "ballz_playing.h"
#include "ballz_game_over.h"

#include <stdlib.h>

int main(void)
{
    /* INITIALIZE COMPONENTS */
    must_init(al_init(), "allegro");

    GameInfo g_info;

    ALLEGRO_DISPLAY* disp;
    ALLEGRO_BITMAP* buffer;

    ALLEGRO_TIMER* timer = al_create_timer(1.0/60.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    disp_init(&disp, &buffer);

    must_init(al_install_keyboard(), "keyboard");
    must_init(al_install_mouse(), "install mouse");
    must_init(al_init_font_addon(), "font");
    must_init(al_init_ttf_addon(), "ttf");
    must_init(al_init_primitives_addon(), "primitives");

    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_keyboard_event_source());

    ballz_init();


    al_start_timer(timer);

    State_t game_state;

    game_state = START;

    while(game_state != ENDGAME) 
    {
        switch (game_state)
        {
            case START: game_state = state_start(&disp, &buffer, queue);  break;
            case PLAYING: game_state = state_playing(&disp, &buffer, queue, &g_info); break;
            case GAME_OVER: game_state = state_game_over(&disp, &buffer, queue, &g_info); break;
            default:
                break;
        }
    }


    disp_deinit(&disp, &buffer);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}