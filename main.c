

#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_keyboard.h>
#include "main.h"


int main(int argc, char *argv[])
{
    
    struct GameState *game_state;
    SDL_Window *window;
	SDL_Renderer *renderer;
    game_state = malloc(sizeof(struct GameState));
    init_game_state(game_state);
    long timer_begin;
    long delay;
    int width=1000;
    int height=500;
    const long RR = 16;
    SDL_CreateWindowAndRenderer("TITLE", width, height, 0, &window, &renderer);
    load_level(renderer);
    while (!game_state->quit){

        timer_begin = SDL_GetTicks();

        check_inputs(game_state);
        act_on_inputs(game_state);

        delay = (RR-(SDL_GetTicks()-timer_begin));
        if(delay > RR){
            delay = 0;
        }
        SDL_Delay(delay);
    }
    SDL_Quit();
    free(game_state);
    return 0;
}
void act_on_inputs(struct GameState *game_state){
    if(game_state->KeyPresses_queued.left){
        game_state->KeyPresses_queued.left=false;
        printf("left");
    }
}
void init_game_state(struct GameState *game_state){
    game_state->quit=false;
    game_state->KeyPresses_queued.left=false;
    game_state->KeyPresses_queued.right=false;
    game_state->KeyPresses_queued.up=false;
    game_state->KeyPresses_queued.esc=false;
    game_state->KeyPresses_queued.down=false;
};

void check_inputs(struct GameState *game_state){
    SDL_Event event;
    SDL_PollEvent(&event);
    const bool *keystate = SDL_GetKeyboardState(NULL);
    if(keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A]){
        game_state->KeyPresses_queued.left = true;
    }
    if(keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_SPACE]){
        game_state->KeyPresses_queued.up = true;
    }
    if(keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D]){
        game_state->KeyPresses_queued.right = true;
    }
    if(keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S]){
        game_state->KeyPresses_queued.down = true;
    }
    if(keystate[SDL_SCANCODE_ESCAPE]){
        game_state->KeyPresses_queued.esc = true;
    }
    if (event.type == SDL_EVENT_QUIT){
        game_state->quit = true;
    }
}


void load_level(struct SDL_Renderer *renderer){

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(renderer);

    SDL_Rect window_in_this_case;
    window_in_this_case.x = 10;
    window_in_this_case.y = 10;
    window_in_this_case.w = 1000;
    window_in_this_case.h = 500;

    SDL_Surface *mask = SDL_LoadBMP("src\\collision\\Collision_map.bmp");
    SDL_Surface *graphics = SDL_LoadBMP("src\\collision\\foreground_map.bmp");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, graphics);
    int copy = SDL_RenderCopy(renderer, texture, NULL, &window_in_this_case);
    // SDL_FreeSurface(graphics);
}

// $env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")
// cmake -G "MinGW Makefiles" -S . -B dist
// cd dist
// cmake --build .
// .\app.exe