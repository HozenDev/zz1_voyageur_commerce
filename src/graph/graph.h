#ifndef _graph_h_
#define _graph_h_

struct graph_s {
    char ** g;
    unsigned short w;
    unsigned short h;
};

typedef struct graph_s graph_t;

int graph_main(void);

#endif
