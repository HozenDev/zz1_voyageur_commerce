#include "graph.h"
#include "../seed/seed.h"
#include <stdlib.h>

/**
 * @brief Generate coordiantes of a point
 *
 * @param width, screen width point
 * @param height, screen height point
 * @param offset_x, offset_x of the point
 * @param offset_y, offset_y of the point
 */
SDL_Point graph_generate_point(int width, int height, int offset_x, int offset_y)
{
    SDL_Point p;

    p.x = rand()%(width-offset_x) + offset_x;
    p.y = rand()%(height-offset_y) + offset_y;
    
    return p;
}

/**
 * @brief Generate coordiantes points for a graph
 *
 * @param graph_sdl_t ** g, graph to generate point
 * @param width, screen width point
 * @param height, screen height point
 * @param ratio, ratio for compute offsets
 */
graph_sdl_t * graph_generate_sdl(graph_sdl_t * g, int width, int height, int ratio)
{
    int offset_x = width*ratio;
    int offset_y = height*ratio;

    int i;

    for (i = 0; i < g->g.n; ++i)
    {
        //g.
    }
}

/**
 *
 *
 */
void graph_print_sdl(SDL_Renderer * renderer, graph_sdl_t * g)
{
    
}

graph_t * graph_generategraph(graph_t * graph,float p)
{
    generate_seed(0);
    for(int i=0; i<graph->n; i++){
        for(int j=i+1; j<graph->n; j++){
            if((float)rand()/RAND_MAX < p)
            {
                (graph->matrix)[i][j]=1;
            }
        }
    }
    return(graph);
}

int graph_main(void)
{
    
    return 0;
}


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
void graph_generate_related(graph_t graph, unsigned short down, unsigned short up)
{
    unsigned int k = 0; 
    if (down < up)
    {
	k = rand() % (up - (down +1) +1) + (down +1); 
	graph.matrix[down][down +1] = 1;
	graph.matrix[down+1][down] = 1;
	if (k+1 <= up)
	{
	    graph.matrix[down][k+1] = 1;
	    graph.matrix[k+1][down] = 1;
	}
	graph_generate_related(graph, down+1, k);
	graph_generate_related(graph, k+1, up);
    }
}




void graph_initialize_graph();
