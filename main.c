

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
    const long RR = 16;
    SDL_CreateWindowAndRenderer("TITLE", game_state->window_width, game_state->window_height, 0, &window, &renderer);
    load_level(game_state, renderer);
    while (!game_state->quit){
        timer_begin = SDL_GetTicks();
        render(game_state, renderer);
        check_inputs(game_state);
        act_on_inputs(game_state);
        
        test_collision(game_state);
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
    if(game_state->KeyPresses_queued.right){
        game_state->KeyPresses_queued.right=false;
        move_background_x_y(game_state,-1.0,0.0);
    }
    if(game_state->KeyPresses_queued.left){
        game_state->KeyPresses_queued.left=false;
        move_background_x_y(game_state,1.0,0.0);
    }
    if(game_state->KeyPresses_queued.up){
        game_state->KeyPresses_queued.up=false;
        move_background_x_y(game_state,0.0,1.0);
    }
    if(game_state->KeyPresses_queued.down){
        game_state->KeyPresses_queued.down=false;
        move_background_x_y(game_state,0.0,-1.0);
    }
    if(game_state->KeyPresses_queued.esc){
        printf("quitting");
        game_state->quit=true;
    }
}


void move_background_x_y(struct GameState *game_state,float direction_x,float direction_y){
    if(!test_collision(game_state)){
        int new_position_x = game_state->x+direction_x*game_state->background_speed;
        if(new_position_x > game_state->x_upper_limit){
            new_position_x=game_state->x_upper_limit;
        }   
        if(new_position_x < game_state->x_lower_limit){
            new_position_x=game_state->x_lower_limit;
        }
        int new_position_y = game_state->y+direction_y*game_state->background_speed;
        if(new_position_y > game_state->y_upper_limit){
            new_position_y=game_state->y_upper_limit;
        }   
        if(new_position_y < game_state->y_lower_limit){
            new_position_y=game_state->y_lower_limit;
        }
            
        game_state->y=new_position_y;
        game_state->x=new_position_x;
    }
}


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


int test_collision(struct GameState *game_state){
    // basic way, this will need to volume or spheres at some point depending on what works better
    //pretend the rectangle is there always because testing
    
    // int rectangle_x=((game_state->window_width/2)-10)+game_state->x;
    // int rectangle_y=((game_state->window_height/2)-10)+game_state->y;
    // int rectangle_h=20;
    // int rectangle_w=20;
    int collision=0;
    bool corner_reported=false;
    for(
        int x = game_state->rectangle_for_testing->x - game_state->x ;
        x < game_state->rectangle_for_testing->x - game_state->x + game_state->rectangle_for_testing->w;
        x++
    ){
        for(
            int y = game_state->rectangle_for_testing->y - game_state->y;
            y<game_state->rectangle_for_testing->y - game_state->y + game_state->rectangle_for_testing->h;
            y++
        ){
            if(!corner_reported){
                printf("corner: %d %d\n",x,y);
                corner_reported=true;
            }
            if(game_state->background_collisions[(y*game_state->background_texture->w)+x]==0){
                printf("collision at (%d, %d)\n",(y*game_state->background_texture->w)+x,x);
                return true;

            }

        }
    }
}


void render(struct GameState *game_state, struct SDL_Renderer *renderer){
    SDL_FRect dest;
    dest.x=game_state->x;
    dest.y=game_state->y;
    dest.h=500.0;
    dest.w=1000.0;
    
    // Not sure why these exist in most projects, seems not needed atleast for now
    // SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	// SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, game_state->background_texture, NULL, &dest);
    SDL_RenderFillRect(renderer, game_state->rectangle_for_testing);
    SDL_RenderPresent(renderer);
}


void load_level(struct GameState *game_state,struct SDL_Renderer *renderer){
    game_state->background_texture = load_file_as_texture("C:\\Users\\Nate\\Documents\\game\\SDL_Expirement\\src\\foreground\\foreground_map.bmp", game_state, renderer);
    SDL_Surface* collision_surface = SDL_LoadBMP("C:\\Users\\Nate\\Documents\\game\\SDL_Expirement\\src\\collision\\collision_map.bmp");
    game_state->background_collisions = malloc(collision_surface->w*collision_surface->h*sizeof(int));
    uint8_t *collision_surface_pixels = (uint8_t*) collision_surface->pixels;
    bool found_one = false;
    for (int j=0;j<(collision_surface->w*collision_surface->h);j++)
        if(collision_surface_pixels[j*4] == 0x00 ){                
            game_state->background_collisions[j]=0;
        }else{
            game_state->background_collisions[j]=1;
        }

    game_state->x_lower_limit=game_state->window_width-game_state->background_texture->w;
    game_state->y_lower_limit=game_state->window_height-game_state->background_texture->h;
    SDL_FRect * rectangle_for_testing = malloc(sizeof(SDL_FRect));
    rectangle_for_testing->x=(game_state->window_width/2)-10;
    rectangle_for_testing->y=(game_state->window_height/2)-10;
    rectangle_for_testing->h=20;
    rectangle_for_testing->w=20;
    game_state->rectangle_for_testing=rectangle_for_testing;
}



SDL_Texture *load_file_as_texture(char *path_to_file,struct GameState *game_state, struct SDL_Renderer *renderer){
    SDL_Surface *graphics = SDL_LoadBMP(path_to_file);
    if (graphics == NULL) {
        // Handle graphic creation error
        printf("BMP files %s Not Found!", path_to_file);
        game_state->quit=true;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, graphics);
    if (texture == NULL) {
        // Handle texture creation error
        printf("texture failed for %s!", path_to_file);
        game_state->quit=true;
    }
    SDL_DestroySurface(graphics);
    return texture;
}


void init_game_state(struct GameState *game_state){
    game_state->quit=false;
    game_state->KeyPresses_queued.left=false;
    game_state->KeyPresses_queued.right=false;
    game_state->KeyPresses_queued.up=false;
    game_state->KeyPresses_queued.esc=false;
    game_state->KeyPresses_queued.down=false;
    game_state->x=0.0;
    game_state->y=0.0;
    game_state->window_height=200;
    game_state->window_width=500;
    game_state->x_upper_limit=0;
    game_state->x_lower_limit=0;
    game_state->y_upper_limit=0;
    game_state->y_lower_limit=0;
    game_state->background_speed=2;
};


// $env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")
// cmake -G "MinGW Makefiles" -S . -B dist
// cd dist
// cmake --build .
// .\app.exe