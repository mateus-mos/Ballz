#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ballz_playing.h"

State_t state_playing(ALLEGRO_DISPLAY **disp, ALLEGRO_BITMAP **buffer, ALLEGRO_EVENT_QUEUE *queue)
{
    State_t state = PLAYING;
    ALLEGRO_EVENT event;
    ALLEGRO_MOUSE_STATE mouse_state;

    while(state == PLAYING)
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