#include "graph.h"

/**
 *
 *
 */
void graph_print_sdl(SDL_Renderer * renderer, graph_sdl_t * g)
{
    int i, j;
    int radius = 10;

    for (i = 0; i < g->g.n; ++i)
    {
        sdl_draw_circle(renderer, g->p[i].x, g->p[i].y, radius);
        for (j = i+1; j < g->g.n; ++j)
        {
            if (g->g.matrix[i][j] == 1)
            {
                sdl_draw_segment(renderer, g->p[i].x, g->p[i].y, g->p[j].x, g->p[j].y);
            }
        }
    }
}

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

    p.x = (rand()%(width-offset_x)) + offset_x;
    p.y = (rand()%(height-offset_y)) + offset_y;
    
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
void graph_generate_sdl(graph_sdl_t ** g, int width, int height, int ratio)
{
    int offset_x = width*ratio;
    int offset_y = height*ratio;

    int i;

    for (i = 0; i < (*g)->g.n; ++i)
    {
        (*g)->p[i] = graph_generate_point(width, height, offset_x, offset_y);
    }
}

int graph_game_loop(void)
{
    SDL_Window * window = NULL;
    SDL_Renderer * renderer = NULL;
    SDL_Event event;

    int width = 800;
    int height = 800;

    int running = 1;

    graph_t * g = NULL;
    graph_sdl_t * gs = NULL;

    /* SDL initialisation */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
	zlog(stderr, ERROR, "Erreur d'initialisation de la SDL : %s", SDL_GetError());
	return EXIT_FAILURE;
    }
    zlog(stdout, INFO, "OK '%s'", "SDL is initialized.");

    /* création de la fenetre principale */
    window = sdl_create_window("test", width, height);
    if (!window) exit(-1);
    zlog(stdout, INFO, "OK '%s'", "game_loop: Window is initialized.");

    /* création du renderer */
    renderer = sdl_create_renderer(window);
    if (!renderer) exit(-1);
    zlog(stdout, INFO, "OK '%s'", "game_loop: Renderer is initialized.");

    g = graph_initialize_graph(6);
    graph_generate_related(g, 0, 5);
    graph_generategraph(g, 0.5);
    
    gs = (graph_sdl_t *) malloc(sizeof(graph_sdl_t));
    gs->g = (*g);
    gs->p = (SDL_Point *) malloc(sizeof(SDL_Point)*6);
    graph_generate_sdl(&gs, width, height, 0.5);

    sdl_set_renderer_color(renderer, (SDL_Color) {.r = 255, .g = 0, .b = 0, .a = 255});
    
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
            case SDL_QUIT:
        	zlog(stdout, INFO, "event.type: SDL_QUIT", NULL);
        	running = 0;
                break;
            }
        }

        graph_print_sdl(renderer, gs);
        
        SDL_RenderPresent(renderer);
        
        /* delai minimal = 1 */
        SDL_Delay(30);
    }
    
    return 0;
}

graph_t * graph_generategraph(graph_t * graph, float p)
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
