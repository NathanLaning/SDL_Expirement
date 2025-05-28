#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>


struct KeyPresses{
    bool up;
    bool esc;
    bool down;
    bool left;
    bool right;

};


struct GameState{
    int level;
    bool quit;
    struct KeyPresses KeyPresses_queued;
    int window_width;
    int window_height;
    float x;
    float y;
    float x_upper_limit;
    float x_lower_limit;
    float y_upper_limit;
    float y_lower_limit;
    float background_speed;
    SDL_FRect* rectangle_for_testing;
    SDL_Texture* background_texture;
    int* background_collisions;
};


SDL_Texture *load_file_as_texture(char *, struct GameState *, struct SDL_Renderer *);
void load_level(struct GameState *, struct SDL_Renderer *);
void init_game_state(struct GameState *);
void check_inputs(struct GameState *);
void act_on_inputs(struct GameState *);
void render(struct GameState *, struct SDL_Renderer *);
void move_background_x_y(struct GameState *,float,float);
bool test_collision(struct GameState *);