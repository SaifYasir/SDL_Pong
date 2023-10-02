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

    draw_circle(400,400,100);

    SDL_RenderPresent(renderer);
}

void destroy_window(void){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void draw_circle(int x_pos_midpoint, int y_pos_midpoint, int radius){
    node *coordinates = get_filled_circle(x_pos_midpoint,y_pos_midpoint,radius);
    while(coordinates != NULL){
        SDL_RenderDrawPoint(renderer, coordinates -> coordinate[0], coordinates -> coordinate[1]);
        node *previous_value = coordinates;
        coordinates = coordinates -> next_ptr;
        free(previous_value);
    }
}