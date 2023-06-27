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
 * @brief Generate coordinates points for a graph
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
        g->p[i] = graph_generate_point(width, height, offset_x, offset_y);
    }
}

int game_loop(void)
{
    SDL_Window * window = NULL;
    SDL_Renderer * renderer = NULL;
    SDL_Event event;

    int width = 800;
    int height = 800;

        /* Boucle de jeu */
    while (running) {

	/* Boucle d'évènements */
	while (SDL_PollEvent(&event))
	{
	    switch(event.type)
	    {
	    case SDL_WINDOWEVENT:
		switch (event.window.event)
		{
		case SDL_WINDOWEVENT_CLOSE:
		    zlog(stdout, INFO, "sdl event window close", NULL);
		    break;
		default:
		    break;
		}
		break;
	    case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_SPACE)
		{
                    zlog(stdout, INFO, "SPACE tapped", NULL);
                }
		break;
	    case SDL_MOUSEBUTTONDOWN:
		zlog(stdout, INFO, "appui: x:%d y:%d", event.button.x, event.button.y);
		break;
            case SDL_MOUSEMOTION:
                px = event.motion.xrel;
                py = event.motion.yrel;
                break;
	    case SDL_QUIT:
		zlog(stdout, INFO, "event.type: SDL_QUIT", NULL);
		running = 0;
                break;
	    }
	}
        
        SDL_RenderPresent(renderer);
        
	/* delai minimal = 1 */
	SDL_Delay(30);
    }
    
    return 0;
}

/**
 *
 *
 */
void graph_print_sdl(SDL_Renderer * renderer, graph_sdl_t * g)
{
    int i, j;
    int radius;

    for (i = 0; i < g->g.n; ++i)
    {
        sdl_draw_circle(g->p[i].x, g->p[i].y, radius);
        for (j = i+1; j < N; ++j)
        {
            if (g->g.matrix[i][j] == 1)
            {
                sdl_draw_segment(renderer, g->p[i].x, g->p[i].y, g->p[j].x, g->p[j].y);
            }
        }
    }
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
