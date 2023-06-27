#include "game.h"

void game_graphic_update(SDL_Renderer * renderer,
                          graph_sdl_t * gs,
                          SDL_Point * selected_nodes, int selected_nodes_i,
                          SDL_Point user_point,
                          int mx, int my)
{
    int i;
    
    if (graph_check_point_collide(mx, my, gs->p, gs->g->n, &user_point) == 1
        && graph_is_selected(user_point, selected_nodes, selected_nodes_i+1) == 0)
    {
        graph_draw_point(renderer, user_point,
                         colors_available.RED, USER_POINTS_RADIUS);
    }

    graph_print_line(renderer, selected_nodes, selected_nodes_i+1, colors_available.BLUE);
    for (i = 0; i < selected_nodes_i; ++i) {
        graph_draw_point(renderer, selected_nodes[i],
                         colors_available.BLUE, POINTS_RADIUS);
    }
    if (selected_nodes_i >= 0) graph_draw_point(renderer, selected_nodes[selected_nodes_i],
                                                colors_available.BLUE, POINTS_RADIUS);
        
    SDL_RenderPresent(renderer);    
}

void game_selection_update(graph_sdl_t * gs,
                            SDL_Point ** selected_nodes, int * selected_nodes_i,
                            SDL_Point * user_point,
                            int mx, int my)
{
    /*
      todo: interargir click souris:
      clique gauche:
      * si sur noeud non sélectionner -> ajouter à la liste des noeuds utilisateur si:
        - soit pile vide, soit il existe un chemin entre dernier noeud sélectionner et ce noeud
      * si sur noeud sélectionner -> si noeud tete de pile, l'enlever
    */
    if (graph_check_point_collide(mx, my, gs->p, gs->g->n, user_point) == 1)
    {
        if (graph_is_selected((*user_point), *selected_nodes, *selected_nodes_i+1) == 1)
        {
            zlog(stdout, INFO, "point sélectionner", NULL);
            if (graph_compare_points((*user_point), (*selected_nodes)[*selected_nodes_i]) == 1)
            {
                *selected_nodes_i -= 1;
                zlog(stdout, INFO, "point (%d,%d) enlever", (*user_point).x, (*user_point).y);
            }
        }
        else
        {
            zlog(stdout, INFO, "point pas sélectionner", NULL);
            if (*selected_nodes_i == -1
                || graph_is_neighbor(gs, (*user_point), (*selected_nodes)[*selected_nodes_i]) == 1)
            {
                *selected_nodes_i += 1;
                (*selected_nodes)[*selected_nodes_i] = (*user_point);
                zlog(stdout, INFO, "point (%d,%d) sélectionner", (*user_point).x, (*user_point).y);
            }
        }
    }
    zlog(stdout, DEBUG, "selectode_node_i: %d", selected_nodes_i);    
}

/**
 * @brief game loop, manage user events
 *
 *
 */
int game_loop(void)
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
    int selected_nodes_i = -1;

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
    zlog(stdout, INFO, "OK '%s'", "game_loop: graph_t is initialized.");

    /* création du graphe sdl correspondant au graphe */
    gs = graph_create_sdl(g);
    graph_generate_sdl(&gs, width, height, 1-ratio);
    zlog(stdout, INFO, "OK '%s'", "game_loop: graph_sdl_t is initialized.");

    /* création des états du jeu */
    /* liste des noeuds sélectionné par l'utilisateur */
    selected_nodes = (SDL_Point *) malloc(sizeof(selected_nodes)*number_of_points);
    if (!selected_nodes) exit(-1);
    zlog(stdout, INFO, "OK '%s'", "game_loop: selected_nodes is initialized.");
    
    SDL_GetMouseState(&mx, &my);
    
    /* Boucle de jeu */
    while (running) {

        sdl_set_renderer_color(renderer, colors_available.WHITE);
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
                game_selection_update(gs, &selected_nodes, &selected_nodes_i, &user_point, mx, my);
        	break;
            case SDL_QUIT:
        	zlog(stdout, INFO, "event.type: SDL_QUIT", NULL);
        	running = 0;
                break;
            }
        }

        game_graphic_update(renderer, gs, selected_nodes, selected_nodes_i, user_point, mx, my);
        
        /* delai minimal = 1 */
        SDL_Delay(30);
    }
    
    return 0;
}
