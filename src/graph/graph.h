#ifndef _graph_h_
#define _graph_h_

#include "../sdl/sdl.h"

struct graph_s {
<<<<<<< HEAD
    char ** g;
=======
    char ** matrix;
>>>>>>> refs/remotes/origin/master
    unsigned short n;
};

typedef struct graph_s graph_t;

struct graph_sdl_s {
    struct graph_s g;
    SDL_Point * p;
};

typedef struct graph_sdl_s graph_sdl_t;
    
int graph_main(void);

/**
 * \fn void genere(graphe_t ** matrice, int bas, int haut)
 * \brief Fonction qui génère un graphe connexe aléatoire
 * 
 *  
 * \param[in] graph_t graph
 * \param[in] unsigned short down
 * \param[in] unsigned short up
 * 
 * \return void : pas de retour, effet de bord sur le graphe
 * 
 */
void graph_generate_related(graph_t graph, unsigned short down, unsigned short up);

void graph_initialize_graph();

    
#endif
