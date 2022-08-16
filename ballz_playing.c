#include "ballz_playing.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_BALLS_SIZE 100

/* STRUCTS */
typedef struct 
{
    float x_left;
    float y_bottom;
    float x_right;
    float y_top;
    float rx;
    float ry;
    ALLEGRO_COLOR box_color;
} Box;

typedef struct 
{
    float x;
    float y;
    int r;
    float x_vel;
    float y_vel;
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
    test_ptr(p_balls, "p_balls", "create_balls_array");

    a_ball = malloc(sizeof(Ball) * size);
    test_ptr(a_ball, "a_ball", "create_balls_array");

    p_balls->num_balls = 0;
    p_balls->num_balls_allocated = size;
    p_balls->a_ball = a_ball;

    return p_balls;
}

void destroy_balls_array(Balls *p_balls)
{
    test_ptr(p_balls, "p_balls", "destroy_balls_array");

    /* Destroy colors of the balls */
    free(p_balls->a_ball);
    free(p_balls);
}

Balls *reallocate_balls(Balls *p_balls, int n_balls)
{
    Balls *n_p_balls;

    test_ptr(p_balls, "p_balls", "reallocate_balls");
    test_ptr(p_balls->a_ball, "p_balls->a_balls", "reallocate_balls");

    n_p_balls = realloc(p_balls->a_ball, sizeof(Ball) * n_balls); 
    test_ptr(n_p_balls, "n_p_balls", "reallocate_balls");

    return n_p_balls;
}

void insert_ball(Balls *p_balls, int x, int y, int r, float x_vel, float y_vel, ALLEGRO_COLOR ball_color)
{
    test_ptr(p_balls, "p_balls", "insert_ball");
    test_ptr(p_balls->a_ball, "p_balls->a_ball", "insert_ball");

    if(p_balls->num_balls_allocated == p_balls->num_balls)
    {
        log_error("insert_ball", "p_balls doesn't have enough space to a new ball!");
        exit(1);
    }

    p_balls->a_ball[p_balls->num_balls].x = x;
    p_balls->a_ball[p_balls->num_balls].y = y;
    p_balls->a_ball[p_balls->num_balls].r = r;
    p_balls->a_ball[p_balls->num_balls].x_vel = x_vel;
    p_balls->a_ball[p_balls->num_balls].y_vel = y_vel;
    p_balls->a_ball[p_balls->num_balls].ball_color = ball_color;

   p_balls->num_balls++;

    #ifdef DEBUG
        log_info("insert_ball", "A Ball was inserted!");
    #endif
}

void update_balls(Balls *p_balls)
{
    test_ptr(p_balls, "p_balls", "update_balls");

    for(int i = 0; i < p_balls->num_balls; i++)
    {
        p_balls->a_ball[i].x += p_balls->a_ball[i].x_vel; 
        p_balls->a_ball[i].y += p_balls->a_ball[i].y_vel; 

        /* Test collide with wall or map */
        if(p_balls->a_ball[i].x > BUFFER_W || p_balls->a_ball[i].x < 0)
            p_balls->a_ball[i].x_vel *= -1;  

        if(p_balls->a_ball[i].y > BUFFER_H || p_balls->a_ball[i].y < 0)
            p_balls->a_ball[i].y_vel *= -1;  
    }
    
}

void draw_balls(Balls *p_balls)
{
    test_ptr(p_balls, "p_balls", "update_balls");

    for(int i = 0; i < p_balls->num_balls; i++)
       al_draw_filled_circle(p_balls->a_ball[i].x, p_balls->a_ball[i].y, p_balls->a_ball[i].r, p_balls->a_ball[i].ball_color); 
}

State_t state_playing(ALLEGRO_DISPLAY **disp, ALLEGRO_BITMAP **buffer, ALLEGRO_EVENT_QUEUE *queue)
{
    State_t state = PLAYING;
    ALLEGRO_EVENT event;
    ALLEGRO_MOUSE_STATE mouse_state;
    ALLEGRO_FONT * tittle_font;
    ALLEGRO_FONT * text_font;

    tittle_font = load_font(GREATE_FIGHTER_FONT, TITTLE_FONT_SIZE);
    test_ptr(tittle_font, "tittle_font", "state_playing");

    text_font = load_font(GREATE_FIGHTER_FONT, TEXT_FONT_SIZE);
    test_ptr(text_font, "text_font", "state_playing");

    Balls *balls_array = create_balls_array(ARRAY_BALLS_SIZE);
    test_ptr(balls_array, "balls_array", "state_playing");

    while(state == PLAYING)
    {

        al_wait_for_event(queue, &event);

        switch (event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                    disp_pre_draw(*buffer);
                    al_clear_to_color(al_map_rgb(0,0,0));

                    //hud_start_draw(tittle_font, text_font);
                    update_balls(balls_array);

                    draw_balls(balls_array);

                    disp_post_draw(*disp, *buffer);
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                    al_get_mouse_state(&mouse_state);

                    insert_ball(balls_array, BUFFER_H/2, BUFFER_W/2, 2, 1, 1, SECONDARY_COLOR);

                    //if(play_button_clicked(&mouse_state))
                    //{
                    //    fprintf(stderr, "[INFO]: Button 'Play' pressed! \n");
                    //    fprintf(stderr, "[INFO]: Change state to PLAY! \n");
                    //    //state = PLAY;
                    //}

                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    state = ENDGAME;
                    #ifdef DEBUG
                        log_info("state_playing", "ENDGAME state!");
                    #endif
                break;
        }
    }

    destroy_balls_array(balls_array);
    al_destroy_font(tittle_font);
    al_destroy_font(text_font);
    return state;
}