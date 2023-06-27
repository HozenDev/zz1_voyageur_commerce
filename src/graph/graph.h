#ifndef _graph_h_
#define _graph_h_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../sdl/sdl.h"
#include "../seed/seed.h"
#include "../log/log.h"
#include "../utils/utils.h"

/**
 * \def N_MIN
 *
 * \brief nombre de sommets minimum
 *
 */
#define N_MIN 5

/**
 * \def N_MIN
 *
 * \brief nombre de sommets maximum
 *
 */
#define N_MAX 20

struct graph_s {
    char ** matrix;
    unsigned short n;
};

typedef struct graph_s graph_t;

struct graph_sdl_s {
    struct graph_s * g;
    SDL_Point * p;
    float ** dist;
};

typedef struct graph_sdl_s graph_sdl_t;
    
int graph_main(void);

int graph_game_loop(void);

void graph_initialize_dist(graph_sdl_t * graph);

void graph_print_file(FILE * flux, graph_t * graph);

//constructeur
graph_t *  graph_initialize_graph(unsigned short n);
graph_sdl_t *  graph_initialize_graph_sdl(unsigned short n, int width, int height, int ratio);

void graph_generate_related(graph_t * graph, unsigned short down, unsigned short up);
graph_t * graph_generate_graph(graph_t * graph,float p);



//destructeur
void graph_free_graph(graph_t * graph);
void graph_free_graph_sdl(graph_sdl_t * graph);

#endif
