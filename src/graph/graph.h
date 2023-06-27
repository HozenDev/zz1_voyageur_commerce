#ifndef _graph_h_
#define _graph_h_

#include "../sdl/sdl.h"

struct graph_s {
    char ** g;
    unsigned short n;
};

typedef struct graph_s graph_t;

struct graph_sdl_s {
    struct graph_s g;
    SDL_Point * p;
};

typedef struct graph_sdl_s graph_sdl_t;
    
int graph_main(void);

#endif
