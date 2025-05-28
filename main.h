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
    bool quit;
    struct KeyPresses KeyPresses_queued;
};

void load_level(struct SDL_Renderer *);
void init_game_state(struct GameState *);
void check_inputs(struct GameState *);
void act_on_inputs(struct GameState *);