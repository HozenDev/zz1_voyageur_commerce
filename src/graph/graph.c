#include "graph.h"



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
        g.
    }
}

/**
 *
 *
 */
void graph_print_sdl(SDL_Renderer * renderer, graph_sdl_t * g)
{
    
}

graph_t * graph_generategraph(char ** g,unsigned short N,float p)
{
    graph_t * graph= (graph_t *) malloc (sizeof(graph_t));
    if(NULL != graph)
        generate_seed(0);
        for(int i=0; i<N; i++){
            for(int j=i+1; j<N; j++){
                if((float)rand()/RAND_MAX < p)
                {
                    g[i][j]=1;
                }
            }
        }
        graph->matrix=g;
        graph->n=N;
    return(graph);
    

}

int graph_main(void)
{
    char g[4][4]={{0,1,1,0},{1,0,0,1},{1,0,0,0},{0,1,0,0}};
    graph_t * graph=generegraph(g,4,4,0.1);
    char ** g2=graph_t->g;
    for (int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            printf("%c",g2[i][j]);
        }
    }
    return 0;
}


/**
 * \fn void genere(graphe_t ** matrice, int bas, int haut)
 * \brief Fonction qui génère un graphe connexe aléatoire
 * 
 *  
 * \param[in] graph_t * graph
 * \param[in] unsigned short down
 * \param[in] unsigned short up
 * 
 * \return void : pas de retour, effet de bord sur le graphe
 * 
 */
void graph_generate_related(graph_t * graph, unsigned short down, unsigned short up)
{
    unsigned int k = 0; 
    if (down < up)
    {
	k = rand() % (up - (down +1) +1) + (down +1); 
	graph->matrix[down][down +1] = 1;
	graph->matrix[down+1][down] = 1;
	if (k+1 <= up)
	{
	    graph->matrix[down][k+1] = 1;
	    graph->matrix[k+1][down] = 1;
	}
	graph_generate_related(graph, down+1, k);
	graph_generate_related(graph, k+1, up); 
    }
}




graph_t *  graph_initialize_graph(unsigned short n)
{
    int i=0, j=0;           // var parcours
    graph_t * graph = NULL;

    graph = malloc(sizeof(graph_t));
    
    if (NULL == graph)
    {
	zlog(stdout, ERROR, "error in allocation of graph in graph_initialize_graph\n", NULL);
	return NULL;
    }

    graph->matrix = malloc(sizeof(char *)* n);
    if (NULL != graph->matrix )
    {
	for(int i = 0; i < n; ++i) // initialize matrix
	{
	    graph->matrix[i] = malloc(sizeof(char) * n);
	    if (graph->matrix[i] == NULL)
	    {
		zlog(stdout, ERROR, "error in allocation of graph in graph_initialize_graph\n", NULL);
		for(j=0; j<i; ++j)
		{
		    free(graph->matrix[j]);
		}
		free(graph);
		return NULL; 
	    }
	}
	
    }
    
    for (i = 0; i < n; ++i)
    {
	for (j = 0; j < n; ++j)
	{
	    graph->matrix[i][j] = 0;
	}
    }
    graph->n = n;
    
    return graph;
}
