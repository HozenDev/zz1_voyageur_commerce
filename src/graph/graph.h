#ifndef _graph_h_
#define _graph_h_

#include "../sdl/sdl.h"
#include "../seed/seed.h"
#include "../log/log.h"

#include <stdlib.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#define POINTS_RADIUS 10
#define USER_POINTS_RADIUS 15
#define USER_LINE_WIDTH 3

struct colors_s {
    SDL_Color BLACK;
    SDL_Color WHITE;
    SDL_Color RED;
    SDL_Color GREEN;
    SDL_Color BLUE;
    SDL_Color YELLOW;
};

extern const struct colors_s colors_available;

struct graph_s {
    char ** matrix;
    unsigned short n;
};

typedef struct graph_s graph_t;

struct graph_sdl_s {
    struct graph_s * g;
    SDL_Point * p;
};

typedef struct graph_sdl_s graph_sdl_t;

/* graph_t functions */

void graph_generate_related(graph_t * graph, unsigned short down, unsigned short up);
graph_t * graph_generate_graph(graph_t * graph,float p);

graph_t *  graph_initialize_graph(unsigned short n);

void graph_print_file(FILE * flux, graph_t * graph);

/* graph_sdl_t functions */

graph_sdl_t * graph_create_sdl(graph_t * g);
SDL_Point graph_generate_point(int width, int height, int offset_x, int offset_y);
void graph_generate_sdl(graph_sdl_t ** g, int width, int height, float ratio);
void graph_print_sdl(SDL_Renderer * renderer, graph_sdl_t * g);

int graph_is_neighbor(graph_sdl_t * gs, SDL_Point p1, SDL_Point p2);

/* points functions */

int graph_check_point_collide(int x, int y, SDL_Point * p, int n, SDL_Point * pt_intersect);
int graph_compare_points(SDL_Point p1, SDL_Point p2);
int graph_is_selected(SDL_Point p1, SDL_Point * p_array, int n);
void graph_draw_point(SDL_Renderer * renderer, SDL_Point p, SDL_Color c, float radius);
void graph_draw_points(SDL_Renderer * renderer, SDL_Point * p, int n, SDL_Color c, float radius);
void graph_print_line(SDL_Renderer * renderer, SDL_Point * p, int n, SDL_Color c);

#endif
