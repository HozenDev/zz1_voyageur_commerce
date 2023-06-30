#ifndef _graph_h_
#define _graph_h_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL2_gfxPrimitives.h>

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
#define N_MIN 500

/**
 * \def N_MIN
 *
 * \brief nombre de sommets maximum
 *
 */
#define N_MAX 2000

#define POINTS_RADIUS 20

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

/* graph_t functions */

void graph_generate_related(graph_t * graph, unsigned short down, unsigned short up);
graph_t * graph_generate_graph(graph_t * graph,float p);

graph_t *  graph_initialize_graph(unsigned short n);
void graph_initialize_dist(graph_sdl_t * graph);

void graph_print_file(FILE * flux, graph_t * graph);

void graph_free_graph(graph_t * graph);

/* graph_sdl_t functions */

graph_sdl_t * graph_initialize_graph_sdl(unsigned short n, int width, int height, float ratio, float p);
SDL_Point graph_generate_point(SDL_Rect rect);
void graph_generate_sdl(graph_sdl_t ** g, int width, int height, float ratio);
void graph_print_sdl(SDL_Renderer * renderer, graph_sdl_t * g);

int graph_is_neighbor(graph_sdl_t * gs, SDL_Point p1, SDL_Point p2);

void graph_free_graph_sdl(graph_sdl_t * graph);

/* points functions */

int graph_is_edge_selected(SDL_Point p1, SDL_Point p2, SDL_Point * p_array, int n);
int graph_check_point_collide(int x, int y, SDL_Point * p, int n, SDL_Point * pt_intersect);
int graph_compare_points(SDL_Point p1, SDL_Point p2);
int graph_point_is_selected(SDL_Point p1, SDL_Point * p_array, int n);
void graph_draw_point(SDL_Renderer * renderer, SDL_Point p, SDL_Color c, float radius);
void graph_draw_points(SDL_Renderer * renderer, SDL_Point * p, int n, SDL_Color c, float radius);
void graph_print_line(SDL_Renderer * renderer, SDL_Point * p, int n, SDL_Color c);
float graph_get_distance_selected(SDL_Point * p, int n);

#endif
