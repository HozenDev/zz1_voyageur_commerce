#ifndef _graph_h_
#define _graph_h_

struct graph_s {
    char ** matrix;
    unsigned short n;
};

typedef struct graph_s graph_t;

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
