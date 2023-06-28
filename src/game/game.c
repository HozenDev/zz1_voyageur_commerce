#include "game.h"
#include "../resolution/resolution.h"

/**
 * @brief Update game graphic
 * 
 * @param game_t to update graphic
 */
void game_graphic_update(game_t game)
{
    int i;
    
    if (graph_check_point_collide(game.state.mx, game.state.my, game.state.gs->p,
                                  game.state.gs->g->n, &game.state.user_point) == 1
        && graph_point_is_selected(game.state.user_point, game.state.selected_nodes, game.state.selected_nodes_i+1) == 0)
    {
        graph_draw_point(game.renderer, game.state.user_point,
                         colors_available.RED, USER_POINTS_RADIUS);
    }

    graph_print_line(game.renderer, game.state.selected_nodes,
                     game.state.selected_nodes_i+1, colors_available.BLUE);

    for (i = 0; i < game.state.selected_nodes_i; ++i) {
        graph_draw_point(game.renderer, game.state.selected_nodes[i],
                         colors_available.BLUE, POINTS_RADIUS);
    }
    if (game.state.selected_nodes_i >= 0)
        graph_draw_point(game.renderer,
                         game.state.selected_nodes[game.state.selected_nodes_i],
                         colors_available.GREEN, POINTS_RADIUS);
}

/**
 * @brief Update a game state
 *
 * @param g_state, the game's state to be updated
 */
void game_state_update(game_state_t * g_state)
{
    /*
      todo: interargir click souris:
      clique gauche:
      * si noeud = dernier sélectionner -> enleve le point
      * si (pile vide ou noeud voisin) -> ajouter
      */
    if (graph_check_point_collide(g_state->mx, g_state->my,
                                  g_state->gs->p, g_state->gs->g->n, &g_state->user_point) == 1)
    {
        if (graph_compare_points(g_state->user_point,
                                 g_state->selected_nodes[g_state->selected_nodes_i]) == 1)
        {
            g_state->selected_nodes_i -= 1;
            zlog(stdout, INFO, "point (%d,%d) dropped", g_state->user_point.x, g_state->user_point.y);
        }
        else if ((g_state->selected_nodes_i == -1
                 || graph_is_neighbor(g_state->gs, g_state->user_point,
                                      g_state->selected_nodes[g_state->selected_nodes_i])) == 1
                 && g_state->selected_nodes_i < g_state->number_of_selected_points)
        {
            g_state->selected_nodes_i += 1;
            g_state->selected_nodes[g_state->selected_nodes_i] = g_state->user_point;
            zlog(stdout, INFO, "point (%d,%d) added", g_state->user_point.x, g_state->user_point.y);
        }
    }
    zlog(stdout, DEBUG, "selected_node_i: %d", g_state->selected_nodes_i);
}

/**
 * @brief initialise a game_t structure
 *
 * @param game, game_t to be initialized
 *
 * @return exit code, 0 success, 1 failure
 */
int game_initialisation(game_t ** game)
{
    (*game) = (game_t *) malloc(sizeof(game_t));
    
    (*game)->sw = 800;
    (*game)->sh = 800;

    (*game)->related_prob = 0.4;
    (*game)->ratio = 0.8;
 
    /* initialize (*game) state */
    (*game)->state.mx = 0;
    (*game)->state.my = 0;
    (*game)->state.selected_nodes = NULL;
    (*game)->state.selected_nodes_i = -1;
    (*game)->state.running = 1;
    
    /* SDL initialisation */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
	zlog(stderr, ERROR, "Erreur d'initialisation de la SDL : %s", SDL_GetError());
	exit(-1);
    }
    zlog(stdout, INFO, "OK '%s'", "SDL is initialized.");

    /* création de la fenetre principale */
    (*game)->window = sdl_create_window("JEU DU VOYAGEUR", (*game)->sw, (*game)->sh);
    if (!(*game)->window) exit(-1);
    zlog(stdout, INFO, "OK '%s'", "(*game)_loop: Window is initialized.");

    /* création du renderer */
    (*game)->renderer = sdl_create_renderer((*game)->window);
    if (!(*game)->renderer) exit(-1);
    zlog(stdout, INFO, "OK '%s'", "(*game)_loop: Renderer is initialized.");

    generate_seed(0);
    (*game)->number_of_points = generate_random_number(N_MIN_GAME, N_MAX_GAME);
    
    /* création du graphe sdl correspondant au graphe */
    (*game)->state.gs = graph_initialize_graph_sdl((*game)->number_of_points, (*game)->sw, (*game)->sh, 1-(*game)->ratio, (*game)->related_prob);
    zlog(stdout, INFO, "OK '%s'", "(*game)_loop: graph_sdl_t is initialized.");

    /* création des états du jeu */
    /* liste des noeuds sélectionné par l'utilisateur */
    (*game)->state.number_of_selected_points = (int) ((*game)->number_of_points)*((*game)->number_of_points)*0.5;
    (*game)->state.selected_nodes =
        (SDL_Point *) malloc(sizeof(SDL_Point)*(*game)->state.number_of_selected_points);
    if (!(*game)->state.selected_nodes) exit(-1);
    zlog(stdout, INFO, "OK '%s'", "(*game)_loop: selected_nodes is initialized.");

    return EXIT_SUCCESS;
}

/**
 * @brief game loop, initialise, manage events, update graphics, free
 *
 * @return exit code: 0 success, != 0 failure
 */
int game_loop(void)
{
    game_t * game = NULL;
    
    SDL_Event event;
    
    game_initialisation(&game);

    /* Boucle de jeu */
    printf("floyd warshall %f \n", floydWarshall(game->state.gs));
    
    while (game->state.running == 1) {

        sdl_set_renderer_color(game->renderer, colors_available.WHITE);
        SDL_RenderClear(game->renderer);

        graph_print_sdl(game->renderer, game->state.gs);
        
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
                SDL_GetMouseState(&game->state.mx, &game->state.my);
                break;
            case SDL_MOUSEBUTTONDOWN:
                game_state_update(&game->state);
        	break;
            case SDL_QUIT:
        	zlog(stdout, INFO, "event.type: SDL_QUIT", NULL);
        	game->state.running = 0;
                break;
            }
        }

        game_graphic_update(*game);

        SDL_RenderPresent(game->renderer);    
        
        /* delai minimal = 1 */
        SDL_Delay(30);
    }

    graph_free_graph_sdl(game->state.gs);
    
    return 0;
}
