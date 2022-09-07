#include "ballz_game_over.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>

#define N_SCORES_SAVED 5

void draw_game_over_hud(ALLEGRO_FONT *tittle_font, ALLEGRO_FONT *text_font, GameInfo *g_info, int *scores)
{
    char coins[10] = "\0";
    char score[10] = "\0";

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

    for(int i = 0; i < N_SCORES_SAVED; i++)
    {
        snprintf(score, 10, "%.3d", scores[i]);
        al_draw_text(
            text_font,
            PRIMARY_COLOR,
            BUFFER_W / 2,
            (BUFFER_H / 8) * 3.6 + i*15,
            ALLEGRO_ALIGN_CENTER,
            score 
        );

        draw_coin(BUFFER_W / 2 + 19, (BUFFER_H / 8) * 3.9 + i*15, COIN_RADIUS);
    }

    //draw_coin(BUFFER_W / 2 + 20, (BUFFER_H / 8) * 4, COIN_RADIUS);

    //al_draw_text(
    //    tittle_font,
    //    PRIMARY_COLOR,
    //    BUFFER_W / 2 - 10,
    //    (BUFFER_H / 8) * 3.6,
    //    ALLEGRO_ALIGN_CENTER,
    //    coins 
    //);
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

void save_scores(FILE *ptr_scores, int *scores)
{
    /* Reset score */
    ptr_scores = freopen("./.score", "w", ptr_scores);

    for(int i = 0; i < N_SCORES_SAVED; i++)
    {
        char s_score[10];
        snprintf(s_score, 10, "%.3d", scores[i]);
        fputs(s_score, ptr_scores);
        fputs("\n", ptr_scores);
    }

    #ifdef DEBUG
        log_info("save_scores", "Scores saved!");
    #endif

    fclose(ptr_scores);
}

void insert_score(int *scores, int score)
{
    int i = 0;

    /* Find the position to insert the new score */
    while(i < N_SCORES_SAVED && scores[i] > score)
        i++;
    
    if(i == N_SCORES_SAVED)
    {
        #ifdef DEBUG
            log_info("insert_score", "%d will not be put on the list, because it's lower than all registered scores!");
        #endif 
        return;
    }

    /* Open space for the new score */
    for(int j = N_SCORES_SAVED - 1; j > i; j--)
        scores[j] = scores[j-1];

    scores[i] = score;

    #ifdef DEBUG
        log_info("insert_score", "%d inserted in the registered scores!", score);
    #endif 
}

void read_scores(FILE *ptr_score, int *scores)
{
    #ifdef DEBUG
        log_info("read_scores", "Scores readed from file!");
    #endif
    char line[100];

    int i = 0;

    while(fgets(line, 100, ptr_score) != NULL)
    {
        scores[i] = atoi(line);
        i++;
    }
}

void init_scores(int *scores)
{
    for(int i = 0; i < N_SCORES_SAVED; i++)
        scores[i] = 0;
}

State_t state_game_over(ALLEGRO_DISPLAY **disp, ALLEGRO_BITMAP **buffer, ALLEGRO_EVENT_QUEUE *queue, GameInfo *g_info)
{
    #ifdef DEBUG
        log_info("state_game_over", "State GAME_OVER initialized");
    #endif

    int scores[N_SCORES_SAVED];

    init_scores(scores);

    FILE *ptr_score;

    ptr_score = fopen("./.score", "r");

    /* The score file doesn't exist, so create it */
    if(ptr_score == NULL)
        ptr_score = fopen("./.score", "w+");

    read_scores(ptr_score, scores);

    insert_score(scores, g_info->coins);


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

    disp_pre_draw(*buffer);
    al_clear_to_color(al_map_rgb(0,0,0));
    draw_game_over_hud(tittle_font, text_font, g_info, scores);

    disp_post_draw(*disp, *buffer);

    while(state == GAME_OVER)
    {

        al_wait_for_event(queue, &event);

        switch (event.type)
        {
            case ALLEGRO_EVENT_TIMER:
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

    save_scores(ptr_score, scores);
    al_destroy_font(tittle_font);
    al_destroy_font(text_font);
    return state;
}