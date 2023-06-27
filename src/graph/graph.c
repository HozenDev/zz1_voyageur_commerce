#include "graph.h"

const struct colors_s colors_available = 
{
    .BLACK    = (SDL_Color) {30, 30, 30, 255},
    .WHITE    = (SDL_Color) {220, 220, 220, 255},
    .RED = (SDL_Color) {255, 100, 100, 255},
    .GREEN  = (SDL_Color) {100, 255, 100, 255},
    .BLUE = (SDL_Color) {100, 100, 255, 255},
    .YELLOW = (SDL_Color) {255, 255, 100, 255}
};

int max(int a, int b)
{
    return (a > b) ? a : b;
}

int min(int a, int b)
{
    return (a > b) ? b : a;
}

int graph_compare_points(SDL_Point p1, SDL_Point p2)
{
    return (p1.x == p2.x && p1.y == p2.y);
}

int graph_is_neighbor(graph_sdl_t * gs, SDL_Point p1, SDL_Point p2)
{
    int i;
    int i_p1, i_p2 = 0;
    
    for (i = 0; i < gs->g->n; ++i)
    {
        if (graph_compare_points(p1, gs->p[i]) == 1) i_p1 = i;
        if (graph_compare_points(p2, gs->p[i]) == 1) i_p2 = i;
    }

    zlog(stdout, DEBUG, "%d %d", i_p1, i_p2);
    zlog(stdout, DEBUG, "(%d,%d) (%d,%d)", gs->p[i_p1].x, gs->p[i_p1].y, gs->p[i_p2].x, gs->p[i_p2].y);
    
    return (int) gs->g->matrix[min(i_p1, i_p2)][max(i_p2, i_p1)];
}

int graph_is_selected(SDL_Point p1, SDL_Point * p_array, int n)
{
    int i;
    int is_selected = 0;

    for (i = 0; i < n && !is_selected; ++i)
    {
        if (graph_compare_points(p1, p_array[i]) == 1) is_selected = 1;
    }

    return is_selected;
}

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

void graph_print_line(SDL_Renderer * renderer, SDL_Point * p, int n, SDL_Color c)
{
    int i;
    
    for (i = 0; i < n-1; ++i)
    {
        thickLineRGBA(renderer, p[i].x, p[i].y, p[i+1].x, p[i+1].y, 3, c.r, c.g, c.b, c.a);
        /* sdl_draw_segment(renderer, g->p[i].x, g->p[i].y, g->p[j].x, g->p[j].y); */
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

    sdl_set_renderer_color(renderer, colors_available.BLACK);
    
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

    graph_draw_points(renderer, g->p, g->g->n, colors_available.RED, radius);
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

