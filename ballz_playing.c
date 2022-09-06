#include "ballz_playing.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BALLS_ARRAY_SIZE 100
#define BOXS_ARRAY_SIZE  50 
#define OBJECTS_ARRAY_SIZE 50 

/* PLAYABLE AREA */
#define PA_W BUFFER_W 
#define PA_H BUFFER_H
#define PA_MARGIN_W_LEFT 0 
#define PA_MARGIN_W_RIGHT 0 
#define PA_MARGIN_H_TOP BUFFER_H / 15 
#define PA_MARGIN_H_BOTTOM 0 

#define N_BOXS_PER_ROW 6
#define BOX_X_SCALE 0.8
#define BOX_Y_SCALE 0.9

#define AIM_RADIUS 50
//#define PA_MARGIN_W_LEFT (BUFFER_W - PA_W) / 2
//#define PA_MARGIN_W_RIGHT (BUFFER_W - PA_W) / 2
//#define PA_MARGIN_H_TOP BUFFER_H / 25 
//#define PA_MARGIN_H_BOTTOM BUFFER_H / 25 

#define TIME_BETWEEN_LAUNCHED_BALLS 0.3
#define BALL_SPEED 4 
#define BALL_SIZE 3
#define BALL_HIT_BOTTOM_MARGIN 5
#define BALL_COLOR PIXEL(255, 255, 255)


#define OBJ_BALL 101
#define OBJ_COIN 102
#define OBJ_RADIUS 4 


/* STRUCTS */
    typedef struct 
    {
        float x_center;
        float y_center;
        float x_left;
        float y_bottom;
        float x_right;
        float y_top;
        float x_vel;
        float y_vel;
        int points;
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
        bool first_ball_to_hit_bottom;
        bool is_moving_to_ball_master;
        bool at_bottom;
    } Ball;

    /* Array of Ball */
    typedef struct
    {
        int num_balls;
        int num_balls_allocated;
        Ball *a_ball;
    } Balls;

    typedef struct 
    {
        float x;
        float y;
        float r;
        bool free;
        int object_id;
    } Object;

    typedef struct 
    {
        int num_obj_allocated;
        Object *a_objects;
    } Objects;

    typedef struct
    {
        int level;
        int coins;
        int balls;
    } GameInfo;
    
    

/* PROTOTYPES */
    void launch_ball(Balls *balls_array, int ball_index, float from_x, float from_y, float to_x, float to_y, float speed);
    bool ball_collide_with_a_box(Ball *p_ball, Boxs *boxs_array, int *index_box_collide);
    void decrease_box_point(Boxs *boxs_array, int index);
    void remove_box(Boxs *boxs_array, int index);
    void draw_coin(float x, float y, float r);


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
        log_info("create_balls_array", "Balls Array created with size!");
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

void insert_ball(Balls *p_balls, float x, float y, float r, ALLEGRO_COLOR ball_color)
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
    p_balls->a_ball[p_balls->num_balls].first_ball_to_hit_bottom = false;
    p_balls->a_ball[p_balls->num_balls].is_moving_to_ball_master = false;

   p_balls->num_balls++;

    #ifdef DEBUG
        log_info("insert_ball", "A Ball was inserted at (%.2f, %.2f)!", x, y);
    #endif
}

bool collide_balls(Balls *balls_array, int index1, int index2)
{
    if(fabs(balls_array->a_ball[index1].x - balls_array->a_ball[index2].x) < 2)
        if(fabs(balls_array->a_ball[index1].y - balls_array->a_ball[index2].y) < 2)
            return true;
    return false;
}

/* Return the index of master ball */
/* If the master ball doesn't exist, return -1 */
int index_master_ball(Balls *balls_array)
{
    for(int i = 0; i < balls_array->num_balls; i++)
        if(balls_array->a_ball[i].first_ball_to_hit_bottom == true)
            return i;
    return -1;
}

void swap_balls(Balls *balls_array, int index1, int index2)
{
    Ball aux_ball = balls_array->a_ball[index1];
    balls_array->a_ball[index1] = balls_array->a_ball[index2];
    balls_array->a_ball[index2] = aux_ball; 
}

bool ball_hit_the_bottom(Balls *balls_array, int index)
{
    return (balls_array->a_ball[index].y + BALL_HIT_BOTTOM_MARGIN  > (BUFFER_H - PA_MARGIN_H_BOTTOM)); 
}

bool is_ball_moving(Balls *balls_array, int index)
{
    return !(balls_array->a_ball[index].x_vel == 0 && balls_array->a_ball[index].y_vel == 0);
}

bool collide_ball_and_obj(Ball *p_ball, Object *obj)
{
    if((p_ball->x - BALL_SIZE) > (obj->x + obj->r) || (p_ball->x + BALL_SIZE) < (obj->x - obj->r))
        return false;
    else if((p_ball->y - BALL_SIZE) > (obj->y + obj->r) || (p_ball->y + BALL_SIZE) < (obj->y - obj->r))
        return false;
    
    return true;
}

bool ball_collide_with_an_object(Ball *p_ball, Objects *objs_array, int *index)
{
    for(int i = 0; i < objs_array->num_obj_allocated; i++)
    {
        if(!objs_array->a_objects[i].free && collide_ball_and_obj(p_ball, &objs_array->a_objects[i]))
        {
            #ifdef DEBUG
                log_info("ball_collide_with_an_object", "An Ball hit the object %d!", i);
            #endif
            *index = i;
            return true;
        }
    }

    return false;
}

void update_balls_and_boxs(Balls *p_balls, Boxs *boxs_array, Objects *objs_array, GameInfo *g_info)
{
    log_test_ptr(p_balls, "update_balls", "p_balls");

    for(int i = 0; i < p_balls->num_balls; i++)
    {
        p_balls->a_ball[i].x += p_balls->a_ball[i].x_vel;
        p_balls->a_ball[i].y += p_balls->a_ball[i].y_vel;

        int index_box_collide;
        int index_obj_collide;

        /* It's moving to master ball */
        if(p_balls->a_ball[i].is_moving_to_ball_master == true)
        {
            int index_m_ball = index_master_ball(p_balls);

            if(collide_balls(p_balls, i, index_m_ball))
            {
                #ifdef DEBUG
                    log_info("update_balls_and_boxs", "Ball %d hit the master ball %d", i, index_m_ball);
                #endif
                p_balls->a_ball[i].x_vel = 0;
                p_balls->a_ball[i].y_vel = 0;

                p_balls->a_ball[i].x = p_balls->a_ball[index_m_ball].x;
                p_balls->a_ball[i].y = p_balls->a_ball[index_m_ball].y;

                p_balls->a_ball[i].is_moving_to_ball_master = false;
            }
        }
        /* Test collide with walls*/
        else if(p_balls->a_ball[i].x > (BUFFER_W - PA_MARGIN_W_RIGHT) || p_balls->a_ball[i].x < PA_MARGIN_W_LEFT)
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
                log_info("update_balls", "The ball with index %d hit the bottom", i);
            #endif

            p_balls->a_ball[i].x -= p_balls->a_ball[i].x_vel;
            p_balls->a_ball[i].y -= p_balls->a_ball[i].y_vel;

            p_balls->a_ball[i].at_bottom = true;
            p_balls->a_ball[i].x_vel = 0;
            p_balls->a_ball[i].y_vel = 0;

            /* Check if it's the first ball to hit the bottom */
            int index_m_ball = index_master_ball(p_balls); 
            if(index_m_ball == -1)
            {
                p_balls->a_ball[i].at_bottom = true;
                p_balls->a_ball[i].first_ball_to_hit_bottom = true;
            }
            /* Launch to master */
            else
            {
                /* Check if the ball is already at right place with the master ball */
                if(!collide_balls(p_balls, i, index_m_ball))
                {
                    launch_ball(p_balls, i, p_balls->a_ball[i].x, p_balls->a_ball[i].y, p_balls->a_ball[index_m_ball].x,  p_balls->a_ball[index_m_ball].y, BALL_SPEED - 2);
                    p_balls->a_ball[i].is_moving_to_ball_master = true;
                }
            }
        }
        /* Collide with a box */
        else if(ball_collide_with_a_box(&p_balls->a_ball[i], boxs_array, &index_box_collide) && boxs_array->a_box[index_box_collide].points > 0)
        {
            #ifdef DEBUG
                log_info("update_balls", "The ball %d collide with the box %d! (%.2f, %.2f)", i, index_box_collide, p_balls->a_ball[i].x, p_balls->a_ball[i].y);
            #endif

            decrease_box_point(boxs_array, index_box_collide);

            p_balls->a_ball[i].x -= p_balls->a_ball[i].x_vel;
            p_balls->a_ball[i].y -= p_balls->a_ball[i].y_vel;

            /* Hit the left or right side */
            if(boxs_array->a_box[index_box_collide].x_right < p_balls->a_ball[i].x || boxs_array->a_box[index_box_collide].x_left > p_balls->a_ball[i].x)
                p_balls->a_ball[i].x_vel *= -1;  
            /* Hit the top or bottom */
            else
                p_balls->a_ball[i].y_vel *= -1;  

            p_balls->a_ball[i].x += p_balls->a_ball[i].x_vel;
            p_balls->a_ball[i].y += p_balls->a_ball[i].y_vel;
        }
        /* Collide with an object */
        else if(ball_collide_with_an_object(&p_balls->a_ball[i], objs_array, &index_obj_collide))
        {
            if(objs_array->a_objects[index_obj_collide].object_id == OBJ_COIN)
            {
                g_info->coins++;
                objs_array->a_objects[index_obj_collide].free = true;
            }
            else if(objs_array->a_objects[index_obj_collide].object_id == OBJ_BALL)
            {
                g_info->balls++;
                objs_array->a_objects[index_obj_collide].free = true;
            }
        }
    }
}

void decrease_box_point(Boxs *boxs_array, int index)
{
    boxs_array->a_box[index].points -= 1;
}

bool collide_ball_and_box(Ball *p_ball, Box *p_box)
{
    if((p_ball->x - BALL_SIZE) > p_box->x_right || (p_ball->x + BALL_SIZE) < p_box->x_left)
        return false;
    else if((p_ball->y - BALL_SIZE) > p_box->y_top || (p_ball->y + BALL_SIZE) < p_box->y_bottom)
        return false;
    
    return true;
}

/* If the ball collide with a box, return true and the index in the 'index_box_collide' */
/* Otherwise, return false */
bool ball_collide_with_a_box(Ball *p_ball, Boxs *boxs_array, int *index_box_collide)
{
    for(int i = 0; i < boxs_array->num_boxs; i++)
        if(collide_ball_and_box(p_ball, &boxs_array->a_box[i]))
        {
            *index_box_collide = i;
            return true;
        }
    return false;
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
        log_info("launch_ball", "Ball %d launched from (%.2f,%.2f) to (%.2f,%.2f)!", ball_index, from_x, from_y, to_x, to_y);
    #endif

}

void launch_all_balls(Balls *balls_array, float to_x, float to_y, float speed)
{
    for(int i = 0; i < balls_array->num_balls; i++)
    {
        launch_ball(balls_array, i, balls_array->a_ball[i].x, balls_array->a_ball[i].y, to_x, to_y, BALL_SPEED);
        //al_rest(0.5);
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

void insert_box(Boxs *boxs_array, float x, float y, float height, float width, int points, ALLEGRO_COLOR collor)
{
    log_test_ptr(boxs_array, "insert_box", "boxs_array");


    int i = 0;
    while(boxs_array->a_box[i].points > 0 && i < boxs_array->num_boxs_allocated)
        i++;

    if(i == boxs_array->num_boxs_allocated)
    {
        log_error("insert_box", "There's not enough space in the box array for a new box!");
        exit(1);
    }

    boxs_array->a_box[i].x_center = x; 
    boxs_array->a_box[i].y_center = y; 
    boxs_array->a_box[i].x_left = x - width / 2;
    boxs_array->a_box[i].x_left = x - width / 2;
    boxs_array->a_box[i].x_right = x + width / 2;
    boxs_array->a_box[i].y_top = y + height / 2;
    boxs_array->a_box[i].y_bottom =  y - height / 2;
    boxs_array->a_box[i].box_color =  collor;
    boxs_array->a_box[i].x_vel =  0;
    boxs_array->a_box[i].y_vel =  0;
    boxs_array->a_box[i].points =  points;
    
    boxs_array->num_boxs++;

    #ifdef DEBUG
        log_info("insert_box", "Box inserted at (%.2f,%.2f)!", x, y);
    #endif
}

void init_boxs_array(Boxs *boxs_array)
{
    for(int i = 0; i < boxs_array->num_boxs_allocated; i++)
    {
        boxs_array->a_box[i].x_center = 0; 
        boxs_array->a_box[i].y_center = 0; 
        boxs_array->a_box[i].x_left = 0;
        boxs_array->a_box[i].x_left = 0;
        boxs_array->a_box[i].x_right = 0;
        boxs_array->a_box[i].y_top = 0;
        boxs_array->a_box[i].y_bottom =  0;
        boxs_array->a_box[i].x_vel =  0;
        boxs_array->a_box[i].y_vel =  0;
        boxs_array->a_box[i].points =  0;
    }
}

Objects *create_objects_array(int size)
{
    Objects *objs_array = malloc(sizeof(Objects));
    log_test_ptr(objs_array, "create_objects_array", "objs_array");

    objs_array->a_objects = malloc(sizeof(Object) * size);
    log_test_ptr(objs_array->a_objects, "create_objects_array", "objs_array->a_objects");

    objs_array->num_obj_allocated = size;

    for(int i = 0; i < size; i++)
        objs_array->a_objects[i].free = true;
    
    #ifdef DEBUG
        log_info("create_objects_array", "Objects array created with size %d!", size);
    #endif
    return objs_array;
}

void destroy_objects_array(Objects *objs_array)
{
    free(objs_array->a_objects);
    free(objs_array);
}

void insert_object(Objects *objs_array, float x, float y, int obj_id)
{
    int i = 0;
    while (i < objs_array->num_obj_allocated && objs_array->a_objects[i].free == false)
        i++;
    
    if(i >= objs_array->num_obj_allocated)
    {
        log_error("insert_object", "Couldn't insert an object! There's not enough space!");
        exit(1);
    }

    objs_array->a_objects[i].object_id = obj_id;
    objs_array->a_objects[i].x = x;
    objs_array->a_objects[i].y = y;
    objs_array->a_objects[i].free = false;
    objs_array->a_objects[i].r = OBJ_RADIUS;

    #ifdef DEBUG
        log_info("insert_object", "Object %d inserted! (%.2f,%.2f)", obj_id, x, y);
    #endif
    
}

void create_row(Boxs *boxs_array, Objects *objs_array, int level)
{
    log_test_ptr(boxs_array, "create_box_row", "boxs_array");

    int i;

    for(i = 0; i < N_BOXS_PER_ROW; i++)
    {

        float x = ((PA_W / N_BOXS_PER_ROW) / 2) + i * (PA_W / N_BOXS_PER_ROW);
        float y =  PA_MARGIN_H_TOP + (BUFFER_W / N_BOXS_PER_ROW) / 2;

        /* Put a box on that position */
        if( (rand() % 2) == 1)
            insert_box(boxs_array, x, y, (BUFFER_W / N_BOXS_PER_ROW) * BOX_X_SCALE, (BUFFER_W / N_BOXS_PER_ROW) * BOX_Y_SCALE, level, SECONDARY_COLOR); 
        /* Put an object on that position */
        else if( (rand() % 2) == 1)
        {
            if( (rand() % 2) == 1)
            {
                /* insert coin */
                insert_object(objs_array, x, y, OBJ_COIN);
            }
            else
            {
                /* insert a ball */
                insert_object(objs_array, x, y, OBJ_BALL);
            }
        }
    }
    #ifdef DEBUG
        log_info("create_box_row", "New row with %d boxs created!", i);
    #endif
}

/* Return false if a box hit the bottom */
/* otherwise, return true */
bool push_boxs_down(Boxs *boxs_array)
{
    for(int i = 0; i < boxs_array->num_boxs_allocated; i++)
    {
        if(boxs_array->a_box[i].points > 0)
        {
            boxs_array->a_box[i].y_bottom += (BUFFER_W / N_BOXS_PER_ROW) * BOX_Y_SCALE;
            boxs_array->a_box[i].y_top += (BUFFER_W / N_BOXS_PER_ROW) * BOX_Y_SCALE;
            boxs_array->a_box[i].y_center += (BUFFER_W / N_BOXS_PER_ROW) * BOX_Y_SCALE;

            if(boxs_array->a_box[i].y_top > BUFFER_H)
                return false;
        }
    }
    return true;
}

void push_objs_down(Objects *objs_array)
{
    for(int i = 0; i < objs_array->num_obj_allocated; i++)
        objs_array->a_objects[i].y += (BUFFER_W / N_BOXS_PER_ROW) * BOX_Y_SCALE;
}

void draw_boxs(Boxs *boxs_array, ALLEGRO_FONT *text_font)
{
    char a_points[10];
    for(int i = 0; i < boxs_array->num_boxs_allocated; i++)
    {
        if(boxs_array->a_box[i].points > 0)
        {
            al_draw_filled_rectangle(boxs_array->a_box[i].x_left, boxs_array->a_box[i].y_top, boxs_array->a_box[i].x_right, boxs_array->a_box[i].y_bottom, SECONDARY_COLOR);

            snprintf(a_points, 10, "%d", boxs_array->a_box[i].points);
            /* Draw points */
            al_draw_text(
                text_font,
                PRIMARY_COLOR,
                boxs_array->a_box[i].x_center,
                boxs_array->a_box[i].y_center + (boxs_array->a_box[i].y_bottom - boxs_array->a_box[i].y_top) / 4,
                ALLEGRO_ALIGN_CENTER,
                a_points 
            );
        }
    }
}

void draw_objects(Objects *objs_array)
{
    for(int i = 0; i < objs_array->num_obj_allocated; i++)
    {
        if(!objs_array->a_objects[i].free)
        {
            if(objs_array->a_objects[i].object_id == OBJ_COIN)
                draw_coin(objs_array->a_objects[i].x, objs_array->a_objects[i].y, objs_array->a_objects[i].r);
            else if(objs_array->a_objects[i].object_id == OBJ_BALL)
            {
                al_draw_circle(objs_array->a_objects[i].x, objs_array->a_objects[i].y, objs_array->a_objects[i].r, PIXEL(255, 255, 255), 2);
            }
        }
    }
}

void draw_coin(float x, float y, float r)
{
    al_draw_filled_circle(x, y, r, PIXEL(233,173,3));
    al_draw_circle(x, y, r, PIXEL(255, 223, 0), 2);
}

void draw_hud(ALLEGRO_FONT *text_font, GameInfo *g_info)
{
    char a_level[10];
    char a_coins[10];

    snprintf(a_level, 10, "%.3d", g_info->level);
    snprintf(a_coins, 10, "%.3d", g_info->coins);

    /* Draw Level */
    al_draw_text(
        text_font,
        PRIMARY_COLOR,
        BUFFER_W / 2,
        (BUFFER_H - PA_H) / 2,
        ALLEGRO_ALIGN_CENTER,
        a_level 
    );

    /* Draw num of coins*/
    al_draw_text(
        text_font,
        PRIMARY_COLOR,
        (BUFFER_W / 8) * 6.5,
        (BUFFER_H - PA_H) / 2,
        ALLEGRO_ALIGN_CENTER,
        a_coins 
    );

    /* Draw coin */
    draw_coin((BUFFER_W / 8) * 7.5, (BUFFER_H - PA_H) / 2 + 8, OBJ_RADIUS);

    /* Draw vertical lines */
    //al_draw_line(PA_MARGIN_W_LEFT, PA_MARGIN_H_TOP, PA_MARGIN_W_LEFT, BUFFER_H - PA_MARGIN_H_BOTTOM, PRIMARY_COLOR, 2);
    //al_draw_line(BUFFER_W - PA_MARGIN_W_RIGHT, PA_MARGIN_H_TOP, BUFFER_W - PA_MARGIN_W_RIGHT, BUFFER_H - PA_MARGIN_H_BOTTOM, PRIMARY_COLOR, 2);

    /* Draw horizontal lines */
    //al_draw_line(PA_MARGIN_W_LEFT, PA_MARGIN_H_TOP, BUFFER_W - PA_MARGIN_W_RIGHT, PA_MARGIN_H_TOP, PRIMARY_COLOR, 2);
    //al_draw_line(PA_MARGIN_W_LEFT, BUFFER_H - PA_MARGIN_H_BOTTOM, BUFFER_W - PA_MARGIN_W_RIGHT, BUFFER_H - PA_MARGIN_H_BOTTOM, PRIMARY_COLOR, 2);

}

void reset_first_ball_to_hit_bottom(Balls *balls_array)
{
    for(int i = 0; i < balls_array->num_balls_allocated; i++)
        balls_array->a_ball[i].first_ball_to_hit_bottom = false;
}

void init_game_info(GameInfo *g_info)
{
    g_info->balls = 1;
    g_info->coins = 0;
    g_info->level = 1;
}

State_t state_playing(ALLEGRO_DISPLAY **disp, ALLEGRO_BITMAP **buffer, ALLEGRO_EVENT_QUEUE *queue)
{
    State_t state = PLAYING;
    ALLEGRO_EVENT event;
    ALLEGRO_MOUSE_STATE mouse_state;
    ALLEGRO_FONT * tittle_font;
    ALLEGRO_FONT * text_font;

    GameInfo g_info;

   /* Intializes random number generator */
    time_t t;
    srand((unsigned) time(&t));


    bool draw_launch_line = false;

    bool launching_balls = false;
    bool new_row_created = true;
    int balls_launched = 0;


    double time_last_ball_launch = 0;

    init_game_info(&g_info);

    tittle_font = load_font(DEBUG_FONT, TITTLE_FONT_SIZE);
    log_test_ptr(tittle_font, "state_playing", "tittle_font");

    text_font = load_font(JOY_STICK_FONT, TEXT_FONT_SIZE);
    log_test_ptr(text_font, "state_playing", "text_font");

    Balls *balls_array = create_balls_array(BALLS_ARRAY_SIZE);
    log_test_ptr(balls_array, "state_playing", "balls_array");

    Boxs *boxs_array = create_boxs_array(BOXS_ARRAY_SIZE);
    log_test_ptr(boxs_array, "state_playing", "boxs_array");

    Objects *objs_array = create_objects_array(OBJECTS_ARRAY_SIZE);

    init_boxs_array(boxs_array);

    insert_ball(balls_array, BUFFER_W/2, BUFFER_H - PA_MARGIN_H_BOTTOM - 20, BALL_SIZE, BALL_COLOR);

    create_row(boxs_array, objs_array, g_info.level);

    while(state == PLAYING)
    {
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                    disp_pre_draw(*buffer);
                    al_clear_to_color(al_map_rgb(0,0,0));

                    update_balls_and_boxs(balls_array, boxs_array, objs_array, &g_info);

                    /* Check if the all boxes hit the bottom, if so spawn a new row */
                    if(!new_row_created && !launching_balls && balls_ready_for_launch(balls_array))
                    {
                        log_info("state_playing", "Ready for a new row");
                        g_info.level++;

                        /* Spawn new balls */
                        if(g_info.balls > balls_array->num_balls)
                            for(int i = balls_array->num_balls; i < g_info.balls; i++)
                                insert_ball(balls_array, balls_array->a_ball[0].x, balls_array->a_ball[0].y, BALL_SIZE, BALL_COLOR);

                        /* A box hit the bottom, so GAME OVER */
                        if(!push_boxs_down(boxs_array))
                        {
                            #ifdef DEBUG
                                log_info("state_playing", "Changing to GAME OVER state!");
                            #endif

                            return GAME_OVER;
                        }

                        push_objs_down(objs_array);

                        create_row(boxs_array, objs_array, g_info.level);
                        new_row_created = true;
                        reset_first_ball_to_hit_bottom(balls_array);
                    }

                    /* Launch the balls */
                    if(launching_balls && !(time_last_ball_launch + TIME_BETWEEN_LAUNCHED_BALLS > al_get_time()))
                    {
                        /* Launched all balls */
                        if(balls_launched == balls_array->num_balls)
                            launching_balls = false;
                        else
                        {
                            launch_ball(balls_array, balls_launched, balls_array->a_ball[balls_launched].x, balls_array->a_ball[balls_launched].y, mouse_state.x/DISP_SCALE, mouse_state.y/DISP_SCALE, BALL_SPEED);

                            balls_array->a_ball[balls_launched].at_bottom = false;
                            balls_launched++;

                            time_last_ball_launch = al_get_time();
                        }
                    }

                    if(draw_launch_line)
                    {
                        al_get_mouse_state(&mouse_state);

                        al_draw_line(balls_array->a_ball[0].x, balls_array->a_ball[0].y, (float)(mouse_state.x) / DISP_SCALE, (float)(mouse_state.y) / DISP_SCALE,  SECONDARY_COLOR, 2);
                    }


                    draw_hud(text_font, &g_info);
                    draw_boxs(boxs_array, text_font);
                    draw_balls(balls_array);
                    draw_objects(objs_array);

                    disp_post_draw(*disp, *buffer);
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:

                    /* Draw launch line */
                    if(!launching_balls && balls_ready_for_launch(balls_array))
                        draw_launch_line = true;

                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:

                    if(!launching_balls && balls_ready_for_launch(balls_array))
                    {
                        al_get_mouse_state(&mouse_state);
                        new_row_created = false;
                        launching_balls = true;
                        balls_launched = 0;
                        draw_launch_line = false;
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
    destroy_objects_array(objs_array);

    al_destroy_font(tittle_font);
    al_destroy_font(text_font);
    return state;
}