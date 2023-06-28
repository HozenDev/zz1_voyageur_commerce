#include "graph.h"

int graph_is_edge_selected(SDL_Point p1, SDL_Point p2, SDL_Point * p_array, int n)
{
    /* vérifier si l'arete p1-p2 existe dans le tableau de p_array */
    int i;
    int is_selected = 0;

    for (i = 0; i < n-1 && !is_selected; ++i)
    {
        if ((graph_compare_points(p1, p_array[i])
            && graph_compare_points(p2, p_array[i+1]))
            || (graph_compare_points(p2, p_array[i])
            && graph_compare_points(p1, p_array[i+1]))) is_selected = 1;
    }

    return is_selected;
}

/**
 * \fn int graph_compare_points(SDL_Point p1, SDL_Point p2)
 * \brief Permet de comparer si 2 points sont les mêmes
 * 
 *  
 * \param[in] SDL_Point p1 
 * \param[in] SDL_Point p2
 * 
 * \return int : retourne 1 si ce sont les mêmes, 0 sinon
 * 
 */
int graph_compare_points(SDL_Point p1, SDL_Point p2)
{
    return (p1.x == p2.x && p1.y == p2.y);
}

/**
 * \fn int graph_is_neighbor(graph_sdl_t * gs, SDL_Point p1, SDL_Point p2)
 * \brief Permet de savoir si 2 points sont voisin
 * 
 *  
 * \param[in] graph_sdl_t * gs
 * \param[in] SDL_Point p1
 * \param[in] SDL_Point p2
 * 
 *  \return int : retourne 1 si ce sont les mêmes, 0 sinon
 * 
 */
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

/**
 * \fn int graph_point_is_selected(SDL_Point p1, SDL_Point * p_array, int n)
 * \brief Permet de savoir si le point est déjà sectionné 
 * 
 *  
 * \param[in] SDL_Point p1: position du clique de la souris
 * \param[in] SDL_Point * p_array : tableau de point
 * \param[in] int n : nombre de sommets 
 * 
 *  \return int : retourne 1 si le clique souris a été effectué sur un sommet , 0 sinon
 * 
 */
int graph_point_is_selected(SDL_Point p1, SDL_Point * p_array, int n)
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
 * \fn int graph_check_point_collide(int x, int y, SDL_Point * p, int n, SDL_Point * pt_intersect)
 * \brief Permet de savoir si il y a une collision entre (x,y) et un point du tableau p en enregistrant dans pt_intersect 
 * 
 *  
 * \param[in] int x : point x souris
 * \param[in] int y : point y souris
 * \param[in] int n : nombre de point du graphe
 * \param[in] SDL_Point * p : tableau de point
 * \param[in] SDL_Point * pt_intersect
 * 
 * \return int : retourne 1 si il y a collision, 0 sinon
 * 
 */
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

/**
 * \fn void graph_print_line(SDL_Renderer * renderer, SDL_Point * p, int n, SDL_Color c)
 * \brief Permet de tracer une droite entre les points du tableau p
 * 
 *  
 * \param[in] SDL_Renderer * renderer
 * \param[in]  SDL_Point * p : tableau de point
 * \param[in] int n : nombre de point du graphe
 * \param[in] SDL_Color c : couleur du trait
 * \param[in] SDL_Point * pt_intersect
 * 
 * \return void : ne retourne rien
 * 
 */
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
SDL_Point graph_generate_point(SDL_Rect rect)
{
    SDL_Point p;

    p.x = (rand()%rect.w) + rect.x;
    p.y = (rand()%rect.h) + rect.y;
    
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
    int r, i, j, k, l;

    int nb_cases = (*g)->g->n * 9;
    int nb_row = (int) sqrt(nb_cases) + 1;
    int nb_col = (int) sqrt(nb_cases) + 1;
    
    SDL_Rect rect = (SDL_Rect) {.x = 0, .y = 0, .w = width/nb_col, .h = height/nb_row};
    
    int nb_cases_restantes = nb_cases;
    
    int ** t = (int **) malloc(sizeof(int*)*nb_row);

    (void) ratio;
    
    for (i = 0; i < nb_row; ++i)
    {
        t[i] = (int *) malloc(sizeof(int)*nb_col);
    }

    for (l = 0; l < (*g)->g->n; l++)
    {
        r = rand()%nb_cases_restantes;
        
        k = 0;
        for (i = 0; i < nb_row-1 && k < r; ++i)
        {
            for (j = 0; j < nb_col-1 && k < r; ++j)
            {
                if (t[i][j] != 1) k++;
            }
        }

        t[i][j] = 1;
        nb_cases_restantes--;
        if (i+1 < nb_row)
        {
            t[i+1][j] = 1;
            nb_cases_restantes--;
        }
        if (j+1 < nb_col)
        {
            t[i][j+1] = 1;
            nb_cases_restantes--;
        }
        if (i-1 > 0) 
        {
            t[i-1][j] = 1;
            nb_cases_restantes--;
        }
        if (j-1 > 0)
        {
            t[i][j-1] = 1;
            nb_cases_restantes--;
        }
        if (j-1 > 0 && i-1 > 0)
        {
            t[i-1][j-1] = 1;
            nb_cases_restantes--;
        }
        if (j-1 > 0 && i+1 < nb_row)
        {
            t[i+1][j-1] = 1;
            nb_cases_restantes--;
        }
        if (j+1 < nb_col && i+1 < nb_row)
        {
            t[i+1][j+1] = 1;
            nb_cases_restantes--;
        }
        if (j+1 < nb_col && i-1 > 0)
        {
            t[i-1][j+1] = 1;
            nb_cases_restantes--;
        }

        rect.x = i*rect.w;
        rect.y = j*rect.h;
        (*g)->p[l] = graph_generate_point(rect);
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
    for(int i=0; i<graph->n; i++)
    {
        for(int j=i+1; j<graph->n; j++)
	{
            if((float) ((float) rand()/ (float) RAND_MAX) < p && i != j)
            {
                graph->matrix[i][j]=1;
                graph->matrix[j][i]=1;
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

    // remplie la matrix a 0 (aucune arrete de base)
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



/**
 * \fn void graph_initialize_dist(graph_sdl_t * graph)
 * \brief construit le tableau de distance dans le graphe (attribut float ** dist)
 * 
 *  
 * \param[in] graph_sdl_t * graph
 * 
 * \return void : ne retourne rien
 * 
 */
void graph_initialize_dist(graph_sdl_t * graph)
{
    for(int i=0; i < graph->g->n ; ++i) // ligne
    {
	for(int j=0; j < graph->g->n; ++j)      // colonne
	{
	    if(graph->g->matrix[i][j] == 1 && i != j)       // il y a une arrete entre les sommets i et j
	    { 
		graph->dist[i][j] = distance(graph->p[i], graph->p[j]); // calcule la distance
		graph->dist[j][i] = graph->dist[i][j];
	    }
	}
    }
}



/**
 * \fn graph_sdl_t *  graph_initialize_graph_sdl(unsigned short n, int width, int height, int ratio)
 * \brief Permet de construire une structure graphe_sql_t
 * 
 *  
 * \param[in] unsigned short n : nombre de sommets du graphe
 * @param width, screen width point
 * @param height, screen height point
 * @param ratio, ratio for compute offset
 * 
 * \return void : ne retourne rien
 * 
 */
graph_sdl_t *  graph_initialize_graph_sdl(unsigned short n, int width, int height, float ratio, float prob)
{
    graph_sdl_t * graph = NULL;
    SDL_Point * p = NULL;
    graph = malloc(sizeof(graph_sdl_t));

    if (NULL == graph)
    {
	zlog(stderr, ERROR, "error in allocation of graph in graph_initialize_graph_sdl \n", NULL);
	return NULL;
    }
    
    graph->g = graph_initialize_graph(n);  // pas sur pour moi il faudrait changer la struct graph_s * g
    graph_generate_related(graph->g, 0, n-1);
    graph_generate_graph(graph->g, prob);
    
    graph->dist = (float **) malloc(sizeof(float *)* n);
    if (NULL != graph->dist)
    {
        for(int i = 0; i < n; ++i) // initialize matrix
        {
            graph->dist[i] = (float *) malloc(sizeof(float) * n);
	    
            if (graph->dist[i] == NULL)
            {
        	zlog(stderr, ERROR, "error in allocation of graph in graph_initialize_graph_sdl \n", NULL);
        	for(int j=0; j<i; ++j)
        	{
        	    free(graph->dist[j]);
        	}
        	free(graph);
        	return NULL;
            }
        }
	
    }

    // initialise la distance à l'infinie
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            graph->dist[i][j] = INFINITY; // macro def dans math.h norme C99
        }
    }

    // construit le tableau de SDL point
    p  = (SDL_Point *) malloc(sizeof(SDL_Point)*n) ;

    if (NULL == p)
    {
        zlog(stderr, ERROR, "error in allocation of graph in graph_initialize_graph_sdl \n", NULL);
        free_matrix_char(graph->g->matrix, n);
        return NULL;
    }

    graph->p = p;
    graph_generate_sdl(&graph, width, height, ratio);

    graph_initialize_dist(graph);
    
    return graph;
}


/**
 * \fn void graph_free_graph(graph_t * graph)
 * \brief Permet de libérer une structure graph_t
 * 
 *  
 * \param[in] unsigned short n : nombre de sommets du graphe
 * 
 * \return void : ne retourne rien
 * 
 */
void graph_free_graph(graph_t * graph)
{
    if (NULL != graph)
    {
	free_matrix_char(graph->matrix, graph->n);
	free(graph);
    }
}


/**
 * \fn void graph_free_graph(graph_t * graph)
 * \brief Permet de libérer une structure graph_sdl_t
 * 
 *  
 * \param[in] unsigned short n : nombre de sommets du graphe
 * 
 * \return void : ne retourne rien
 * 
 */
void graph_free_graph_sdl(graph_sdl_t * graph)
{
    if (NULL != graph)
    {
        graph_free_graph(graph->g);                      // libere graphe
	free_matrix_float(graph->dist, graph->g->n);     // libere tableau des distances
	
	free(graph->p);                                  // libere tableau SDL point
	graph->p = NULL;
    }
}
