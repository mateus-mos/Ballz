#include "ballz_game_over.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>

void draw_game_over_hud(ALLEGRO_FONT *tittle_font, ALLEGRO_FONT *text_font, GameInfo *g_info)
{
    char coins[10] = "\0";

    snprintf(coins, 10, "%.3d", g_info->coins);


    al_draw_text(
        tittle_font,
        SECONDARY_COLOR,
        BUFFER_W / 2 + (BUFFER_W / 2)/ 70,
        BUFFER_H / 4 + (BUFFER_H / 2)/ 70,
        ALLEGRO_ALIGN_CENTER,
        "G A M E O V E R"
    );

    al_draw_text(
        tittle_font,
        PRIMARY_COLOR,
        BUFFER_W / 2,
        BUFFER_H / 4,
        ALLEGRO_ALIGN_CENTER,
        "G A M E O V E R"
    );

    draw_coin(BUFFER_W / 2 + 20, (BUFFER_H / 8) * 4, COIN_RADIUS);

    al_draw_text(
        tittle_font,
        PRIMARY_COLOR,
        BUFFER_W / 2 - 10,
        (BUFFER_H / 8) * 3.6,
        ALLEGRO_ALIGN_CENTER,
        coins 
    );
    //al_draw_text(
    //    text_font,
    //    SECONDARY_COLOR, 
    //    /* Define position of the g_textFont based on the start button */
    //    START_BUTTON_PLAY_A_X + (START_BUTTON_PLAY_B_X - START_BUTTON_PLAY_A_X)/2 + press_deslocation,
    //    START_BUTTON_PLAY_A_Y + (START_BUTTON_PLAY_B_Y - START_BUTTON_PLAY_A_Y)/6 + press_deslocation,
    //    ALLEGRO_ALIGN_CENTER,
    //    "Play"
    //);
}

State_t state_game_over(ALLEGRO_DISPLAY **disp, ALLEGRO_BITMAP **buffer, ALLEGRO_EVENT_QUEUE *queue, GameInfo *g_info)
{
    #ifdef DEBUG
        log_info("state_start", "State START initialized");
    #endif

    State_t state = GAME_OVER;
    ALLEGRO_EVENT event;
    ALLEGRO_MOUSE_STATE mouse_state;
    ALLEGRO_FONT * tittle_font;
    ALLEGRO_FONT * text_font;

    //bool play_button_pressed = false;

    tittle_font = load_font(DEBUG_FONT, TITTLE_FONT_SIZE);
    log_test_ptr(tittle_font, "state_start", "tittle_font");

    text_font = load_font(JOY_STICK_FONT, TEXT_FONT_SIZE);
    log_test_ptr(text_font, "state_start", "text_font");

    while(state == GAME_OVER)
    {

        al_wait_for_event(queue, &event);

        switch (event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                    disp_pre_draw(*buffer);
                    al_clear_to_color(al_map_rgb(0,0,0));
                    draw_game_over_hud(tittle_font, text_font, g_info);

                    disp_post_draw(*disp, *buffer);
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                    al_get_mouse_state(&mouse_state);

                    //if(play_button_clicked(&mouse_state))
                    //    play_button_pressed = true;

                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                    //al_get_mouse_state(&mouse_state);

                    //if(play_button_clicked(&mouse_state))
                    //{
                    //    #ifdef DEBUG
                    //        log_info("state_start", "Button 'Play' pressed!");
                    //        log_info("state_start", "Change state to PLAY!");
                    //    #endif
                    //    state = PLAYING;
                    //}

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