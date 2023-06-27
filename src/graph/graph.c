#include "graph.h"

/** 
 * @brief Create and initialize an sdl graph
 *
 * @param g, graph corresponding to the sdl_graph
 *
 * @return graph_sdl_t *, the graph created
 */
graph_sdl_t * graph_create_sdl(graph_t * g)
{
    graph_sdl_t * g_sdl = NULL;

    g_sdl = (graph_sdl_t *) malloc(sizeof(graph_sdl_t));

    if (!g_sdl)
    {
        zlog(stderr, ERROR, "Bad memory allocation of graph_sdl_t: '%s'", __FUNCTION__);
        exit(-1);
    }
    
    g_sdl->p = (SDL_Point *) malloc(sizeof(SDL_Point)*g->n);

    if (!g_sdl->p)
    {
        zlog(stderr, ERROR, "Bad memory allocation of SDL_Point array: '%s'", __FUNCTION__);
        exit(-1);
    }

    g_sdl->g = g;

    return g_sdl;
}

int graph_check_point_collide(int x, int y, SDL_Point * p, int n, SDL_Point * pt_intersect)
{
    int i;
    int collide = 0;

    int offset = POINTS_RADIUS;

    for (i = 0; i < n && !collide; ++i)
    {
        if (x > p[i].x-offset && x < p[i].x+offset
            && y > p[i].y-offset && y < p[i].y+offset) {
            collide = 1;
            *pt_intersect = p[i];
        }
    }

    return collide;
}

/** @brief Draw a point
 *
 * @param renderer, renderer where points are printed
 * @param p, array of points
 * @param c, color of points
 * @param radius, radius of printed points
 */
void graph_draw_point(SDL_Renderer * renderer, SDL_Point p, SDL_Color c, float radius)
{
    sdl_set_renderer_color(renderer, c);
    sdl_draw_circle(renderer, p.x, p.y, radius);
}

/** @brief Draw many points
 *
 * @param renderer, renderer where points are printed
 * @param p, array of points
 * @param n, number of points
 * @param c, color of points
 * @param radius, radius of printed points
 */
void graph_draw_points(SDL_Renderer * renderer, SDL_Point * p, int n, SDL_Color c, float radius)
{
    int i;

    for (i = 0; i < n; ++i)
    {
        graph_draw_point(renderer, p[i], c, radius);
        /* sdl_draw_circle(renderer, p[i].x, p[i].y, radius); */
    }
}

/** @brief Print an sdl graph (draw points and line)
 *
 * @param renderer, renderer where graphe will be printed
 * @param g, sdl graph to be printed
 */
void graph_print_sdl(SDL_Renderer * renderer, graph_sdl_t * g)
{
    int i, j;
    int radius = POINTS_RADIUS;

    sdl_set_renderer_color(renderer, (SDL_Color) {.r = 0, .g = 0, .b = 0, .a = 255});
    
    for (i = 0; i < g->g->n; ++i)
    {
        for (j = i+1; j < g->g->n; ++j)
        {
            if (g->g->matrix[i][j] == 1)
            {
                /* thickLineRGBA(renderer, g->p[i].x, g->p[i].y, g->p[j].x, g->p[j].y, 2, 0, 0, 0, 255); */
                sdl_draw_segment(renderer, g->p[i].x, g->p[i].y, g->p[j].x, g->p[j].y);
            }
        }
    }

    graph_draw_points(renderer, g->p, g->g->n, (SDL_Color) {.r = 255, .g = 0, .b = 0, .a = 255}, radius);
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

    p.x = (rand()%(width-offset_x)) + offset_x/2;
    p.y = (rand()%(height-offset_y)) + offset_y/2;
    
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
void graph_generate_sdl(graph_sdl_t ** g, int width, int height, float ratio)
{
    int offset_x = width*ratio;
    int offset_y = height*ratio;

    int i;

    for (i = 0; i < (*g)->g->n; ++i)
    {
        (*g)->p[i] = graph_generate_point(width, height, offset_x, offset_y);
    }
}

/**
 * @brief game loop, manage user events
 *
 *
 */
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

    int number_of_points = 6;
    float p = 0.4;
    float ratio = 0.8;

    SDL_Point user_point;
    int mx, my;

    SDL_Point * selected_nodes;

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

    /* création du graphe */
    g = graph_initialize_graph(number_of_points);
    graph_generate_related(g, 0, number_of_points-1);
    graph_generate_graph(g, p);

    /* création du graphe sdl correspondant au graphe */
    gs = graph_create_sdl(g);
    graph_generate_sdl(&gs, width, height, 1-ratio);

    /* création des états du jeu */
    /* liste des noeuds sélectionné par l'utilisateur */
    
    SDL_GetMouseState(&mx, &my);
    
    /* Boucle de jeu */
    while (running) {

        sdl_set_renderer_color(renderer, (SDL_Color) {.r = 255, .g = 255, .b = 255, .a = 255});
        SDL_RenderClear(renderer);

        graph_print_sdl(renderer, gs);

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
        	if (event.key.keysym.sym == SDLK_RETURN)
        	{
                    /* todo: vérifier la solution, l'afficher et rejouer */
                    zlog(stdout, INFO, "enter tapped", NULL);
                }
        	break;
            case SDL_MOUSEMOTION:
                /* update mouse position */
                SDL_GetMouseState(&mx, &my);
                break;
            case SDL_MOUSEBUTTONDOWN:
                /*
                  todo: interargir click souris:
                  clique gauche:
                  * si sur noeud non sélectionner -> ajouter à la liste des noeuds utilisateur si:
                      - soit file vide, soit existe un chemin entre dernier noeud sélectionner et ce noeud
                  * si sur noeud sélectionner -> si noeud tete de file, l'enlever

                  numéroter les noeuds pour l'ui
                 */
                if (graph_check_point_collide(mx, my, gs->p, gs->g->n, &user_point) == 1)
                {
                    /* todo */
                }
        	zlog(stdout, INFO, "appui: x:%d y:%d", event.button.x, event.button.y);
        	break;
            case SDL_QUIT:
        	zlog(stdout, INFO, "event.type: SDL_QUIT", NULL);
        	running = 0;
                break;
            }
        }

        if (graph_check_point_collide(mx, my, gs->p, gs->g->n, &user_point) == 1)
        {
            graph_draw_point(renderer, user_point,
                             (SDL_Color) {.r = 0, .b = 255, .g = 0, .a = 255}, USER_POINTS_RADIUS);
        }
        
        SDL_RenderPresent(renderer);
        
        /* delai minimal = 1 */
        SDL_Delay(30);
    }
    
    return 0;
}

/**
 * @brief Complete covering tree with random branches
 * 
 * @param graph_t * graph, entering covering tree 
 * @param float p,probability for each random branch to appear
 */
graph_t * graph_generate_graph(graph_t * graph,float p)
{
    generate_seed(0);
    for(int i=0; i<graph->n; i++)
    {
        for(int j=i+1; j<graph->n; j++)
	{
            if((float) ((float) rand()/ (float) RAND_MAX) < p)
            {
                (graph->matrix)[i][j]=1;
            }
        }
    }
    return(graph);
}

/**
 * \fn void graph_generate_related(graph_t * graph, unsigned short down, unsigned short up)
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


/**
 * \fn graph_t *  graph_initialize_graph(unsigned short n)
 * \brief Permet d'initialiser un graphe sans arrete
 * 
 *  
 * \param[in] unsigned short n
 * 
 * \return graph_t * : retourne un pointeur sur le graphe créé
 * 
 */
graph_t *  graph_initialize_graph(unsigned short n)
{
    int i=0, j=0;           // var parcours
    graph_t * graph = NULL;

    graph = malloc(sizeof(graph_t));
    
    if (NULL == graph)
    {
	zlog(stderr, ERROR, "error in allocation of graph in graph_initialize_graph\n", NULL);
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
		zlog(stderr, ERROR, "error in allocation of graph in graph_initialize_graph\n", NULL);
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



/**
 * \fn void graph_print_file(FILE * flux, graph_t * graph)
 * \brief Permet d'afficher le graphe de manière jolie
 * 
 *  
 * \param[in] FILE * fmux
 * \param[in] graph_t * graph
 * 
 * \return void : ne retourne rien
 * 
 */
void graph_print_file(FILE * flux, graph_t * graph)
{
    if(NULL == flux)
    {
	zlog(stderr, ERROR, "error in flux at NULL in graph_print\n", NULL);
	return ;
    }
    if (NULL == graph)
    {
	printf("graph == NULL in  graph_print_terminal\n");
    }
    
    for (int i = 0; i < graph->n; i++)
    {
        // Affichage de la ligne supérieure
        for (int j = 0; j < graph->n; j++)
	{
            printf("%c ", graph->matrix[i][j] + '0');
	}
	printf("\n");
    }
}

