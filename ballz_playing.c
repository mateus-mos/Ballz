#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ballz_playing.h"

/* STRUCTS */
typedef struct
{
    int x_left;
    int y_bottom;
    int x_right;
    int y_top;
    float rx;
    float ry;
} Dimension;

typedef struct 
{
    int x;
    int y;
    Dimension box_dimension;
    ALLEGRO_COLOR box_color;
} Box;

typedef struct 
{
    int x;
    int y;
    Dimension ball_dimension;
    ALLEGRO_COLOR ball_color;
} Ball;

/* Array of Ball */
typedef struct
{
    int num_balls;
    int num_balls_allocated;
    Ball *a_ball;
} Balls;

Balls *create_balls_array(int size)
{
    Balls *p_balls;
    Ball *a_ball;

    p_balls = malloc(sizeof(Ball));
    test_ptr(p_balls, "p_balls");

    a_ball = malloc(sizeof(Ball) * size);
    test_ptr(a_ball, "a_ball");

    p_balls->num_balls = 0;
    p_balls->num_balls_allocated = size;
    p_balls->a_ball = a_ball;

    return p_balls;
}

void *destroy_balls_array(Balls *p_balls)
{
    test_ptr(p_balls, "p_balls in destroy_balls_array");

    free(p_balls->a_ball);
    free(p_balls);
}

int insert_ball(Balls *p_balls)
{
}

State_t state_playing(ALLEGRO_DISPLAY **disp, ALLEGRO_BITMAP **buffer, ALLEGRO_EVENT_QUEUE *queue)
{
    State_t state = PLAYING;
    ALLEGRO_EVENT event;
    ALLEGRO_MOUSE_STATE mouse_state;
    ALLEGRO_FONT * tittle_font;
    ALLEGRO_FONT * text_font;

    tittle_font = load_font(GREATE_FIGHTER_FONT, TITTLE_FONT_SIZE);
    test_ptr(tittle_font, "tittle_font");

    text_font = load_font(GREATE_FIGHTER_FONT, TEXT_FONT_SIZE);
    test_ptr(text_font, "text_font");

    while(state == PLAYING)
    {

        al_wait_for_event(queue, &event);

        switch (event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                    //disp_pre_draw(*buffer);
                    //al_clear_to_color(al_map_rgb(0,0,0));

                    //hud_start_draw(tittle_font, text_font);

                    //disp_post_draw(*disp, *buffer);
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                    al_get_mouse_state(&mouse_state);

                    //if(play_button_clicked(&mouse_state))
                    //{
                    //    fprintf(stderr, "[INFO]: Button 'Play' pressed! \n");
                    //    fprintf(stderr, "[INFO]: Change state to PLAY! \n");
                    //    //state = PLAY;
                    //}

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