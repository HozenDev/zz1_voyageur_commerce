#ifndef _game_h_
#define _game_h_

#include "../graph/graph.h"

#define USER_POINTS_RADIUS 30
#define USER_LINE_WIDTH 3

#define N_MIN_GAME 5
#define N_MAX_GAME 8

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

struct game_state_s {
    int mx;
    int my;
    SDL_Point user_point;

    int number_of_selected_points;
    SDL_Point * selected_nodes;
    int selected_nodes_i;

    graph_sdl_t * gs;

    int running;

    SDL_Event event;

    float score;
};

typedef struct game_state_s game_state_t;  

struct game_s {
    SDL_Renderer * renderer;
    SDL_Window * window;
    TTF_Font * font;

    int sw;
    int sh;

    int number_of_points;

    float related_prob;
    float ratio;

    struct game_state_s state;
};

typedef struct game_s game_t;

void game_graphic_update(game_t game);

void game_state_update(game_state_t * g_state);

int game_initialisation(game_t ** game);

int game_loop(void);

#endif
