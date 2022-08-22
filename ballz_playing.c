#include "ballz_playing.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARRAY_BALLS_SIZE 100
#define ARRAY_BOXS_SIZE  25

/* PLAYABLE AREA */
#define PA_W BUFFER_W / 2
#define PA_H BUFFER_H
#define PA_MARGIN_W_LEFT (BUFFER_W - PA_W) / 2
#define PA_MARGIN_W_RIGHT (BUFFER_W - PA_W) / 2
#define PA_MARGIN_H_TOP BUFFER_H / 25 
#define PA_MARGIN_H_BOTTOM BUFFER_H / 25 

#define TIME_BETWEEN_LAUNCHED_BALLS 0.3
#define BALL_SPEED 10

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
        int num_boxs;
        int num_boxs_allocated;
        Box *a_box;
    } Boxs;

    typedef struct 
    {
        float x;
        float y;
        int r;
        float x_vel;
        float y_vel;
        ALLEGRO_COLOR ball_color;
        bool moving_to_master;
        bool at_bottom;
    } Ball;

    /* Array of Ball */
    typedef struct
    {
        int num_balls;
        int num_balls_allocated;
        Ball *a_ball;
    } Balls;

/* PROTOTYPES */
    void launch_ball(Balls *balls_array, int ball_index, float from_x, float from_y, float to_x, float to_y, float speed);


Balls *create_balls_array(int size)
{
    Balls *p_balls;
    Ball *a_ball;

    p_balls = malloc(sizeof(Ball));
    log_test_ptr(p_balls, "create_balls_array", "p_balls");

    a_ball = malloc(sizeof(Ball) * size);
    log_test_ptr(a_ball,  "create_balls_array", "a_ball");

    p_balls->num_balls = 0;
    p_balls->num_balls_allocated = size;
    p_balls->a_ball = a_ball;

    #ifdef DEBUG
        log_info("create_balls_array", "Balls Array created!");
    #endif
    return p_balls;
}

void destroy_balls_array(Balls *p_balls)
{
    free(p_balls->a_ball);
    free(p_balls);

    #ifdef DEBUG
        log_info("destroy_balls_array", "Balls Array destroyed!");
    #endif
}

Balls *reallocate_balls(Balls *p_balls, int n_balls)
{
    Balls *n_p_balls;

    log_test_ptr(p_balls, "reallocate_balls", "p_balls");
    log_test_ptr(p_balls->a_ball, "reallocate_balls", "p_balls->a_balls");

    n_p_balls = realloc(p_balls->a_ball, sizeof(Ball) * n_balls); 
    log_test_ptr(n_p_balls, "reallocate_balls", "n_p_balls");

    return n_p_balls;
}

void insert_ball(Balls *p_balls, int x, int y, int r, ALLEGRO_COLOR ball_color)
{
    log_test_ptr(p_balls, "insert_ball", "p_balls");
    log_test_ptr(p_balls->a_ball, "insert_ball", "p_balls->a_ball");

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
    p_balls->a_ball[p_balls->num_balls].moving_to_master = false;

   p_balls->num_balls++;

    #ifdef DEBUG
        log_info("insert_ball", "A Ball was inserted!");
    #endif
}

bool collide_balls(Balls *balls_array, int index1, int index2)
{
    if(fabs(balls_array->a_ball[index1].x - balls_array->a_ball[index2].x) < 0.5)
        if(fabs(balls_array->a_ball[index1].y - balls_array->a_ball[index2].y) < 0.5)
            return true;
    return false;
}

void swap_balls(Balls *balls_array, int index1, int index2)
{
    Ball aux_ball = balls_array->a_ball[index1];
    balls_array->a_ball[index1] = balls_array->a_ball[index2];
    balls_array->a_ball[index2] = aux_ball; 
}

bool ball_hit_the_bottom(Balls *balls_array, int index)
{
    return (balls_array->a_ball[index].y > (BUFFER_H - PA_MARGIN_H_BOTTOM)); 
}

bool is_ball_moving(Balls *balls_array, int index)
{
    return !(balls_array->a_ball[index].x_vel == 0 && balls_array->a_ball[index].y_vel == 0);
}

void update_balls(Balls *p_balls)
{
    log_test_ptr(p_balls, "update_balls", "p_balls");

    for(int i = 0; i < p_balls->num_balls; i++)
    {
        p_balls->a_ball[i].x += p_balls->a_ball[i].x_vel;
        p_balls->a_ball[i].y += p_balls->a_ball[i].y_vel;

        /* Test collide with walls*/
        if(p_balls->a_ball[i].x > (BUFFER_W - PA_MARGIN_W_RIGHT) || p_balls->a_ball[i].x < PA_MARGIN_W_LEFT)
        {
            /* Colide, so undo the movement */
            p_balls->a_ball[i].x -= p_balls->a_ball[i].x_vel;
            p_balls->a_ball[i].y -= p_balls->a_ball[i].y_vel;

            p_balls->a_ball[i].x_vel *= -1;  
        }
        /* Test collide with the top */
        else if(p_balls->a_ball[i].y < PA_MARGIN_H_TOP)
        {
            p_balls->a_ball[i].x -= p_balls->a_ball[i].x_vel;
            p_balls->a_ball[i].y -= p_balls->a_ball[i].y_vel;

            p_balls->a_ball[i].y_vel *= -1;  
        }
        /* Hit the bottom */
        else if(ball_hit_the_bottom(p_balls, i))
        {
            #ifdef DEBUG
                log_info("update_balls", "A ball hit the bottom");
            #endif

            p_balls->a_ball[i].x -= p_balls->a_ball[i].x_vel;
            p_balls->a_ball[i].y -= p_balls->a_ball[i].y_vel;

            p_balls->a_ball[i].at_bottom = true;
            p_balls->a_ball[i].x_vel = 0;
            p_balls->a_ball[i].y_vel = 0;
        }

    }
}

void draw_balls(Balls *p_balls)
{
    log_test_ptr(p_balls, "update_balls", "p_balls");

    for(int i = 0; i < p_balls->num_balls; i++)
       al_draw_filled_circle(p_balls->a_ball[i].x, p_balls->a_ball[i].y, p_balls->a_ball[i].r, BALL_COLOR); 

}

void launch_ball(Balls *balls_array, int ball_index, float from_x, float from_y, float to_x, float to_y, float speed)
{
    if(ball_index > balls_array->num_balls || ball_index < 0)
    {
        log_error("launch_ball", "Invalid ball_index!");
        exit(1);
    }

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
    for(int i = 0; i < balls_array->num_balls; i++)
    {
        launch_ball(balls_array, i, balls_array->a_ball[i].x, balls_array->a_ball[i].y, to_x, to_y, BALL_SPEED);
        al_rest(0.5);
    }
}

bool balls_ready_for_launch(Balls *balls_array)
{
    for(int i = 0; i < balls_array->num_balls; i++)
        if(is_ball_moving(balls_array, i))
            return false;

    return true;
}

Boxs *create_boxs_array(int size)
{
    Boxs *p_boxs = malloc(sizeof(Balls));
    log_test_ptr(p_boxs, "create_boxs_array", "p_boxs");

    p_boxs->a_box = malloc(sizeof(Box) * size);
    log_test_ptr(p_boxs->a_box, "create_boxs_array", "p_boxs->a_box");

    p_boxs->num_boxs = 0;
    p_boxs->num_boxs_allocated = size;

    #ifdef DEBUG
        log_info("create_boxs_array", "Boxs Array created!");
    #endif
    return p_boxs;
}

void destroy_boxs_array(Boxs *boxs_array)
{
    free(boxs_array->a_box);
    free(boxs_array);
    #ifdef DEBUG
        log_info("destroy_boxs_array", "Boxs Array destroyed!");
    #endif
}

void insert_box(Boxs *boxs_array, float x, float y, float height, float width, ALLEGRO_COLOR *collor)
{
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

    bool launching_balls = false;
    int balls_launched = 0;
    double time_last_ball_launch = 0;

    tittle_font = load_font(GREATE_FIGHTER_FONT, TITTLE_FONT_SIZE);
    log_test_ptr(tittle_font, "state_playing", "tittle_font");

    text_font = load_font(GREATE_FIGHTER_FONT, TEXT_FONT_SIZE);
    log_test_ptr(text_font, "state_playing", "text_font");

    Balls *balls_array = create_balls_array(ARRAY_BALLS_SIZE);
    log_test_ptr(balls_array, "state_playing", "balls_array");

    Boxs *boxs_array = create_boxs_array(ARRAY_BOXS_SIZE);
    log_test_ptr(boxs_array, "state_playing", "boxs_array");

    for(int i = 0; i < 10; i++)
        insert_ball(balls_array, BUFFER_W/2, BUFFER_H - PA_MARGIN_H_BOTTOM - 10, 2, BALL_COLOR);


    while(state == PLAYING)
    {
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                    disp_pre_draw(*buffer);
                    al_clear_to_color(al_map_rgb(0,0,0));

                    
                    if(launching_balls && !(time_last_ball_launch + TIME_BETWEEN_LAUNCHED_BALLS > al_get_time()))
                    {
                        if(balls_launched == balls_array->num_balls)
                            launching_balls = false;

                        launch_ball(balls_array, balls_launched, balls_array->a_ball[balls_launched].x, balls_array->a_ball[balls_launched].y, mouse_state.x/DISP_SCALE, mouse_state.y/DISP_SCALE, BALL_SPEED);

                        balls_array->a_ball[balls_launched].at_bottom = false;
                        balls_launched++;

                        time_last_ball_launch = al_get_time();
                    }

                    update_balls(balls_array);
                    draw_hud();
                    draw_balls(balls_array);

                    disp_post_draw(*disp, *buffer);
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:

                    if(!launching_balls && balls_ready_for_launch(balls_array))
                    {
                        al_get_mouse_state(&mouse_state);
                        launching_balls = true;
                        balls_launched = 0;
                    }

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
    destroy_boxs_array(boxs_array);

    al_destroy_font(tittle_font);
    al_destroy_font(text_font);
    return state;
}