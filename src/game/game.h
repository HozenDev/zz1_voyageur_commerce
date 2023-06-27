#ifndef _game_h_
#define _game_h_

#include "../graph/graph.h"

void game_graphic_update(SDL_Renderer * renderer,
                         graph_sdl_t * gs,
                         SDL_Point * selected_nodes, int selected_nodes_i,
                         SDL_Point user_point,
                         int mx, int my);

void game_selection_update(graph_sdl_t * gs,
                           SDL_Point ** selected_nodes, int * selected_nodes_i,
                           SDL_Point * user_point,
                           int mx, int my);

int game_loop(void);

#endif
