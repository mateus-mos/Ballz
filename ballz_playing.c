#include "ballz_playing.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARRAY_BALLS_SIZE 100

/* PLAYABLE AREA */
#define PA_W BUFFER_W / 2
#define PA_H BUFFER_H
#define PA_MARGIN_W_LEFT (BUFFER_W - PA_W) / 2
#define PA_MARGIN_W_RIGHT (BUFFER_W - PA_W) / 2
#define PA_MARGIN_H_TOP BUFFER_H / 25 
#define PA_MARGIN_H_BOTTOM BUFFER_H / 25 

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

void insert_ball(Balls *p_balls, int x, int y, int r, ALLEGRO_COLOR ball_color)
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
    p_balls->a_ball[p_balls->num_balls].x_vel = 0;
    p_balls->a_ball[p_balls->num_balls].y_vel = 0;
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
        if(p_balls->a_ball[i].x > (BUFFER_W - PA_MARGIN_W_RIGHT) || p_balls->a_ball[i].x < PA_MARGIN_W_LEFT)
            p_balls->a_ball[i].x_vel *= -1;  

        if(p_balls->a_ball[i].y > (BUFFER_H - PA_MARGIN_H_BOTTOM) || p_balls->a_ball[i].y < PA_MARGIN_H_TOP)
            p_balls->a_ball[i].y_vel *= -1;  
    }
    
}

void draw_balls(Balls *p_balls)
{
    test_ptr(p_balls, "p_balls", "update_balls");

    for(int i = 0; i < p_balls->num_balls; i++)
       al_draw_filled_circle(p_balls->a_ball[i].x, p_balls->a_ball[i].y, p_balls->a_ball[i].r, BALL_COLOR); 

}

void launch_ball(Balls *balls_array, int ball_index, float from_x, float from_y, float to_x, float to_y, float speed)
{
    float A = to_x - from_x; 
    float B = to_y - from_y;

    float k = speed /(float)sqrt( A*A + B*B);

    balls_array->a_ball[ball_index].x = from_x;
    balls_array->a_ball[ball_index].y = from_y;

    balls_array->a_ball[ball_index].x_vel = A * k;
    balls_array->a_ball[ball_index].y_vel = B * k;

    #ifdef DEBUG
        log_info("launch_ball", "Ball launched!");
    #endif

}

void launch_all_balls(Balls *balls_array, float to_x, float to_y, float speed)
{
}

bool balls_ready_for_launch(Balls *balls_array)
{
    int i = 0;
    while(i < balls_array->num_balls - 1 )
    {
        if(balls_array->a_ball[i].y < (BUFFER_H - PA_MARGIN_H_TOP))
            return false;

        if(fabs(balls_array->a_ball[i].y - balls_array->a_ball[i+1].y) > 0.01)
            return false;
        i++;
    }

    if(balls_array->a_ball[i].y < (BUFFER_H - PA_MARGIN_H_TOP))
        return false;

    return true;
}

void draw_hud()
{
    
    /* Draw vertical lines */
    al_draw_line(PA_MARGIN_W_LEFT, PA_MARGIN_H_TOP, PA_MARGIN_W_LEFT, BUFFER_H - PA_MARGIN_H_BOTTOM, PRIMARY_COLOR, 2);
    al_draw_line(BUFFER_W - PA_MARGIN_W_RIGHT, PA_MARGIN_H_TOP, BUFFER_W - PA_MARGIN_W_RIGHT, BUFFER_H - PA_MARGIN_H_BOTTOM, PRIMARY_COLOR, 2);

    /* Draw horizontal lines */
    al_draw_line(PA_MARGIN_W_LEFT, PA_MARGIN_H_TOP, BUFFER_W - PA_MARGIN_W_RIGHT, PA_MARGIN_H_TOP, PRIMARY_COLOR, 2);
    al_draw_line(PA_MARGIN_W_LEFT, BUFFER_H - PA_MARGIN_H_BOTTOM, BUFFER_W - PA_MARGIN_W_RIGHT, BUFFER_H - PA_MARGIN_H_BOTTOM, PRIMARY_COLOR, 2);

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

                    /* if all balls at bottom, so can launch balls again */
                        /* Print arrow to aim */

                    draw_hud();
                    draw_balls(balls_array);

                    disp_post_draw(*disp, *buffer);
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                    al_get_mouse_state(&mouse_state);

                    insert_ball(balls_array, BUFFER_W/2, BUFFER_H, 1, SECONDARY_COLOR);

                    if(balls_ready_for_launch(balls_array))
                    {
                        //launch_all_balls();
                    }
                    /* if all balls at bottom, so can launch balls again */
                    /* And mouse button down */
                        /* Launch balls to mouse's position */


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