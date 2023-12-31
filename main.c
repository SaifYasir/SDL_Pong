#include <stdio.h>
#include <SDL.h>
#include <stdbool.h>
#include <stdlib.h>

#include "configuration.h"
#include "shapes.h"

#include "node.h"

bool initialise_window(void);
void process_input(void);
void update(void);
void render(void);
void destroy_window(void);
void draw_circle(int x_pos_midpoint, int y_pos_midpoint, int radius);
void move_paddle(SDL_Rect* paddle, int direction, int* paddle_movement_variable);
void create_halfway_line(void);
void render_halfway_line(void);
void destroy_node_list(node* list);
void calculate_ball_movement(double *delta_time);

bool game_running;
SDL_Window* window;
SDL_Surface* surface;
SDL_Renderer* renderer;
int last_frame_time = 0;

node* middle_rectangle_coordinates = NULL;

SDL_Rect left_paddle = {PADDLE_DISTANCE_FROM_BORDER,PADDLE_DISTANCE_FROM_BORDER,PADDLE_WIDTH,PADDLE_HEIGHT};
SDL_Rect right_paddle = {WINDOW_WIDTH - PADDLE_DISTANCE_FROM_BORDER - PADDLE_WIDTH,PADDLE_DISTANCE_FROM_BORDER,PADDLE_WIDTH,PADDLE_HEIGHT};
int left_paddle_movement = 0;
int right_paddle_movement = 0;

struct ball{
    int x_midpoint;
    int y_midpoint;
    int radius;

    int x_pixels_per_sec;
    int y_pixels_per_sec;
} ball;

int main(int argc, char *argv[]){
    game_running = initialise_window();

    while(game_running){
        process_input();
        update();
        render();
    }

    destroy_window();
    return 0;
}

bool initialise_window(void){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("Error initalising SDL: %s",SDL_GetError());
        return false;
    }
    
    window = SDL_CreateWindow("SDL Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == NULL){
        printf("Window could not be created!: %s",SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    ball.x_midpoint = 200;
    ball.y_midpoint = 400;
    ball.x_pixels_per_sec = PONG_BALL_PIXELS_PER_SECOND_X;
    ball.y_pixels_per_sec = PONG_BALL_PIXELS_PER_SECOND_Y;
    ball.radius = PONG_BALL_RADIUS;

    create_halfway_line();
}


void process_input(void){
    SDL_Event e;

    while (SDL_PollEvent(&e) != FALSE){
       if(e.type == SDL_QUIT){
        game_running = false;
        return;
       }

       else if(e.type == SDL_KEYDOWN){
        const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
        switch (e.key.keysym.sym)
        {
            case SDLK_ESCAPE :
                game_running = false;
                break;
            case SDLK_w:
                move_paddle(&left_paddle, PADDLE_PIXEL_PER_SECOND * -1, &left_paddle_movement);
                break;
            case SDLK_s:
                move_paddle(&left_paddle, PADDLE_PIXEL_PER_SECOND, &left_paddle_movement);
                break;
            case SDLK_UP:
                move_paddle(&right_paddle, PADDLE_PIXEL_PER_SECOND * -1, &right_paddle_movement);
                break;
            case SDLK_DOWN:
                move_paddle(&right_paddle, PADDLE_PIXEL_PER_SECOND, &right_paddle_movement);
                break;
        }
       }
    }
}

void update(void){

    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks64() - last_frame_time);
    if(time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME){
        SDL_Delay(time_to_wait);
    }

    //Get a delta time factory converted to seconds to be used to update my objects
    double delta_time = (SDL_GetTicks64() - last_frame_time) / 1000.0f;

    left_paddle.y += left_paddle_movement * delta_time;
    right_paddle.y += right_paddle_movement * delta_time;

    left_paddle_movement = 0;
    right_paddle_movement = 0;

    calculate_ball_movement(&delta_time);

    last_frame_time = SDL_GetTicks64();
}

void render(void){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    render_halfway_line();

    SDL_RenderFillRect(renderer,&left_paddle);
    SDL_RenderFillRect(renderer,&right_paddle);

    draw_circle(ball.x_midpoint,ball.y_midpoint,ball.radius);

    SDL_RenderPresent(renderer);
}

void destroy_window(void){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    destroy_node_list(middle_rectangle_coordinates);
    SDL_Quit();
}

void destroy_node_list(node* list){
    while(list != NULL){
        node* destroy_node = list;
        list = list -> next_ptr;
        free(destroy_node);
    }
} 

void draw_circle(int x_pos_midpoint, int y_pos_midpoint, int radius){
    node *coordinates = get_hollow_circle(x_pos_midpoint,y_pos_midpoint,radius);
    while(coordinates != NULL){
        SDL_RenderDrawPoint(renderer, coordinates -> coordinate[0], coordinates -> coordinate[1]);
        node *previous_value = coordinates;
        coordinates = coordinates -> next_ptr;
        free(previous_value);
    }
}

void move_paddle(SDL_Rect* paddle, int direction, int* paddle_movement_variable){
    if(paddle -> y <= 0 && direction < 0){
        return;
    }
    else if(paddle -> y + PADDLE_HEIGHT >= WINDOW_HEIGHT && direction > 0){
        return;
    }
    *paddle_movement_variable += direction;
}

void calculate_ball_movement(double *delta_time){
    //because positions are counted in ints the decimal is left behind, this should be saved
    static double x_ball_decimal_add = 0;
    static double y_ball_decimal_add = 0;

    x_ball_decimal_add += fmod((ball.x_pixels_per_sec * *delta_time),1);
    y_ball_decimal_add += fmod((ball.y_pixels_per_sec * *delta_time),1);

    //checks bottom of screen
    if(ball.y_midpoint + ball.radius >= WINDOW_HEIGHT){
        ball.y_pixels_per_sec = -ball.y_pixels_per_sec;
        y_ball_decimal_add =  -y_ball_decimal_add;

        ball.y_midpoint -= 1;
    }
    //checks top boundary
    else if (ball.y_midpoint - ball.radius <= 0)
    {
        ball.y_pixels_per_sec = - ball.y_pixels_per_sec; 
        y_ball_decimal_add =  -y_ball_decimal_add;

        ball.y_midpoint += 1;
    }
    //check left paddle
    else if (ball.x_midpoint - ball.radius == left_paddle.x + PADDLE_WIDTH)
    {
        //check bottom of ball for collision
        if(ball.y_midpoint + ball.radius <= left_paddle.y + PADDLE_HEIGHT && ball.y_midpoint + ball.radius >= left_paddle.y){
            ball.x_pixels_per_sec = - ball.x_pixels_per_sec; 
            x_ball_decimal_add =  -x_ball_decimal_add;

            ball.x_midpoint += 1;
        }
        //check top of ball for collision
        else if(ball.y_midpoint - ball.radius <= left_paddle.y + PADDLE_HEIGHT && ball.y_midpoint - ball.radius >= left_paddle.y){
            ball.x_pixels_per_sec = - ball.x_pixels_per_sec; 
            x_ball_decimal_add =  -x_ball_decimal_add;

            ball.x_midpoint += 1;
        }
        /* code */
    }
    //check right paddle
    else if (ball.x_midpoint + ball.radius == right_paddle.x)
    {
        //check bottom of ball for collision
        if(ball.y_midpoint + ball.radius <= right_paddle.y + PADDLE_HEIGHT && ball.y_midpoint + ball.radius >= right_paddle.y){
            ball.x_pixels_per_sec = -ball.x_pixels_per_sec; 
            x_ball_decimal_add =  -x_ball_decimal_add;

            ball.x_midpoint -= 1;
        }
        //check top of ball for collision
        else if(ball.y_midpoint - ball.radius <= right_paddle.y + PADDLE_HEIGHT && ball.y_midpoint - ball.radius >= right_paddle.y){
           ball.x_pixels_per_sec = - ball.x_pixels_per_sec; 
            x_ball_decimal_add =  -x_ball_decimal_add;

            ball.x_midpoint -= 1;
        }
    }
    else{
        ball.x_midpoint += (int)x_ball_decimal_add;
        ball.y_midpoint += (int)y_ball_decimal_add;

        x_ball_decimal_add -= (int)x_ball_decimal_add;
        y_ball_decimal_add -= (int)y_ball_decimal_add;
    }
    
    ball.x_midpoint += (int)(ball.x_pixels_per_sec * *delta_time);
    ball.y_midpoint += (int)(ball.y_pixels_per_sec * *delta_time);
}

void create_halfway_line(void){
    int x_coord = (WINDOW_WIDTH / 2) - (CENTRE_RECTANGLE_WIDTH / 2);
    int y_coord = 0;

    while(y_coord < WINDOW_HEIGHT){
        node* coordinates = malloc(sizeof(node));
        coordinates -> coordinate[0] = x_coord;
        coordinates -> coordinate[1] = y_coord;

        coordinates -> next_ptr = NULL;

        node* current_coordinate = middle_rectangle_coordinates;

        if(coordinates == NULL){
            printf("Error creating node pointer");
            return;
        }
        if(middle_rectangle_coordinates == NULL){
            middle_rectangle_coordinates = coordinates;
        }
        else{
            while(current_coordinate -> next_ptr != NULL){
                current_coordinate = current_coordinate -> next_ptr;
            }
            current_coordinate -> next_ptr = coordinates;
        }
        y_coord += CENTRE_RECTANGLE_GAP + CENTRE_RECTANGLE_HEIGHT;
    }
}

void render_halfway_line(void){
    node* current_coordinate = middle_rectangle_coordinates;

    while (current_coordinate != NULL)
    {
        SDL_Rect middle_rect = {current_coordinate->coordinate[0],current_coordinate->coordinate[1],CENTRE_RECTANGLE_WIDTH,CENTRE_RECTANGLE_HEIGHT};;
        SDL_RenderFillRect(renderer, &middle_rect);
        current_coordinate = current_coordinate -> next_ptr;
    }
}