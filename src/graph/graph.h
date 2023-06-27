#ifndef _graph_h_
#define _graph_h_

#include <stdlib.h>
#include "../sdl/sdl.h"
#include "../seed/seed.h"
#include "../log/log.h"

struct graph_s {
    char ** matrix;
    unsigned short n;
};

typedef struct graph_s graph_t;

struct graph_sdl_s {
    struct graph_s g;
    SDL_Point * p;
};

typedef struct graph_sdl_s graph_sdl_t;
    
int graph_main(void);

void graph_generate_related(graph_t * graph, unsigned short down, unsigned short up);

graph_t *  graph_initialize_graph(unsigned short n);

int graph_game_loop(void);

graph_t * graph_generate_graph(graph_t * graph,float p);

void graph_print_file_pretty(FILE * flux, graph_t * graph);

void graph_print_file(FILE * flux, graph_t * graph);

#endif
