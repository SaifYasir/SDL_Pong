#include <stdio.h>
#include <SDL.h>
#include <stdbool.h>

#include "configuration.h"
#include "shapes.h"

#include "node.h"

bool initialise_window(void);
void process_input(void);
void update(void);
void render(void);
void destroy_window(void);
void draw_circle(int x_pos_midpoint, int y_pos_midpoint, int radius);

bool game_running;
SDL_Window* window;
SDL_Surface* surface;
SDL_Renderer* renderer;
int last_frame_time = 0;

struct circle{
    int x_midpoint;
    int y_midpoint;
    int radius;
} circle;

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
    circle.x_midpoint = 200;
    circle.y_midpoint = 100;
    circle.radius = 10;
}


void process_input(void){
    SDL_Event e;
    while (SDL_PollEvent(&e) != FALSE){

       if(e.type == SDL_QUIT){
        game_running = false;
        return;
       }

       else if(e.type == SDL_KEYDOWN){
        switch (e.key.keysym.sym)
        {
            case SDLK_ESCAPE:
                game_running = false;
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
    float delta_time = (SDL_GetTicks64() - last_frame_time) / 1000.0f;

    last_frame_time = SDL_GetTicks64();

    circle.x_midpoint+= 70 * delta_time;
    circle.y_midpoint+= 70 * delta_time;
}

void render(void){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // SDL_Rect fillRect = {
    //     WINDOW_WIDTH / 4,
    //     WINDOW_HEIGHT / 2,
    //     WINDOW_WIDTH / 4,
    //     WINDOW_HEIGHT / 2
    // };

    // SDL_Rect fillRect = {
    //     100,
    //     100,
    //     100,
    //     100
    // };

    SDL_SetRenderDrawColor(renderer,0xFF, 0x00, 0x00, 0xFF);
    // SDL_RenderFillRect(renderer, &fillRect);

    // SDL_RenderDrawPoint(renderer,400,400);

    draw_circle(circle.x_midpoint,circle.y_midpoint,circle.radius);

    SDL_RenderPresent(renderer);
}

void destroy_window(void){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
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