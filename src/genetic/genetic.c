#include "genetic.h"
#include <stdlib.h>


/**
 * @brief Permet de creer un individu
 *
 * @param int nb_vertices
 *
 * @return l'individu créé en mémoire
 */

individu_t * genetic_create_individu(int nb_vertices)
{
    individu_t * individu = malloc(sizeof(individu));
    
    individu->path = malloc(nb_vertices * sizeof(int));
    individu->n    = nb_vertices;
    individu->distance = INFINITY;

    return individu;
}

/**
 * @brief Permet de détruire l'individu en libérant la mémoire
 *
 * @param individu_t * individu : pointeur dur l'individu à détruire
 *
 * @return void
 */
void genetic_destroy_individu(individu_t * individu)
{
    free(individu->path);
    free(individu);
}


/**
 * @brief Permet de créer la population initiale. (correspond à l'étape 1)
 *
 * tirage aléatoire, pas d'optimisation de zone de recherche
 *
 */
void genetic_initialize_population(individu_t *** population, int nb_vertices, int population_size)
{
    int k,i,j, tmp;
       
    * population = (individu_t **) malloc(population_size * sizeof(individu_t *));
    
    for (i = 0; i < population_size; i++)
    {
        (*population)[i] = genetic_create_individu(nb_vertices);
	
        for (j = 0; j < nb_vertices; j++)
	{
            (*population)[i]->path[j] = j;
        }
	
        // Mélanger le chemin aléatoirement
        for (j = nb_vertices - 1; j > 0; j--)
	{
            k = rand() % (j + 1);
            tmp = (*population)[i]->path[j];

	    (*population)[i]->path[j] = (*population)[i]->path[k];
            (*population)[i]->path[k] = tmp;
        }
    }
}


/**
 * @brief Permet de calculer la distance qu'effectu un individu
 *
 * @param float ** dist       : matrice de floyd warshall
 * @param individu * individu : individu/voyageur, membre d'une population
 *
 * @return la distance du chemin
 */
float genetic_evaluate_distance(float** dist, individu_t* individu)
{
    float total_distance = 0.0;
    int source, destination, last_vertex;
 
    for (int i = 0; i < individu->n - 1; i++)
    {	
        source         = individu->path[i];
        destination    = individu->path[i + 1];
        total_distance += dist[source][destination];
    }
    
    // Ajouter la distance du dernier au premier sommet
    last_vertex = individu->path[individu->n - 1];
    total_distance += dist[last_vertex][individu->path[0]];
    individu->distance = total_distance;
    return total_distance;
}


/**
 * @brief Permet d'effectuer un croisement entre 2 parents pour obtenir un individu fils
 *
 *
 * @param individu_t * p1 : parent 1
 * @param individu_t * p2 : parent 2
 *
 * @return individu_t * : l'individu fils créé 
 */
void genetic_croisement_generate_child(individu_t ** child, individu_t * p1, individu_t * p2)
{
    int startPos = rand() % p1->n;
    int endPos   = rand() % p2->n;
    
    int * visited = calloc(p1->n, sizeof(int)); // 0 si non visité, 1 sinon

    int i, j;
    

    // Copier la portion du parent 1 dans l'enfant
    for (i = startPos; i != endPos; i = (i + 1) % p1->n) // gere le cas si start < end ou si end < start
    {
        (*child)->path[i] = p1->path[i];
        visited[p1->path[i]] = true;
    }
    // Copier les éléments du parent 2 qui ne sont pas déjà dans l'enfant
    j = endPos;
    for (i = endPos; i < p2->n; i++)
    {
        if (!visited[p2->path[i]])
	{
            (*child)->path[j] = p2->path[i];
            visited[p2->path[i]] = true;
            j = (j + 1) % p2->n;
        }
    }
    
    for (int i = 0; i < endPos; i++)
    {
        if (!visited[p2->path[i]])
	{
            (*child)->path[j] = p2->path[i];
            visited[p2->path[i]] = true;
            j = (j + 1) % p2->n;
        }
    }
    free(visited);
}


/**
 * @brief Permet de faire des mutation de manière aléatoire sur un individu, le random se base sur la constante MUTATION_RATE
 *
 * @param individu_t * individu radioactif
 *
 * @return individu_t * : le meilleur individu de la population 
 */
void genetic_mutate(individu_t * individu)
{
    int i,j, tmp;
    for (i = 0; i < individu->n; i++)
    {
        if ((float) (rand() / RAND_MAX) < MUTATION_RATE)
	{
            j = rand() % individu->n;
            tmp = individu->path[i];
            individu->path[i] = individu->path[j];
	    individu->path[j] = tmp;
        }
    }
}


/**
 * @brief Permet de trouver le meilleur individu au sein d'une population
 *
 * @param individu_t ** population :
 * @param int population_size
 *
 * @return individu_t * : le meilleur individu de la population 
 */
individu_t * genetic_find_best_individu_in_pop(individu_t ** population, int population_size)
{
    individu_t * best_individu = population[0];
    for (int i = 1; i < population_size; i++)
    {
        if (population[i]->n < population[i]->distance)
	{
            best_individu = population[i];
        }
    }
    return best_individu;
}



void genetic_copy_popultaion(individu_t *** population_source, individu_t *** population_dest, int nb_vertices)
{
    
    for (int i=0; i < POPULATION_SIZE ; ++i)
    {	
	(*population_dest)[i]->n = nb_vertices;
	
	for (int j=0 ; j < nb_vertices; ++j)
	{	    
	    (*population_dest)[i]->path[j] =  (*population_source)[i]->path[j];
	}
    }
}

float  genetic_solve (float** matrixFloydWarshall, int num_vertices, int ** best_path)
{
    individu_t ** population;
    individu_t ** new_population;
    
    float best_distance          = FLT_MAX;
    int iteration = 0;
    individu_t * best_individu = NULL;

    individu_t *p1, *p2;

    (void) best_path;
	
    genetic_initialize_population(&population, num_vertices, POPULATION_SIZE);
    
    genetic_initialize_population(&new_population, num_vertices, POPULATION_SIZE);

    while (iteration < MAX_ITERATIONS) { // MAX_ITERATIONS
        // calcule distance
        for (int i = 0; i < POPULATION_SIZE; i++)
	{
	    genetic_evaluate_distance(matrixFloydWarshall, population[i]);
	}


	// cherche le meilleur individu 
	best_individu = genetic_find_best_individu_in_pop(population, POPULATION_SIZE);
	
        if (best_individu->distance < best_distance)
	    best_distance = best_individu->distance;

        new_population[0] = best_individu;
	
        // Sélection, croisement et mutation pour générer la nouvelle population
        for (int i = 1; i < POPULATION_SIZE; i++) {

	    // organise 2 tournoi, pour récuperer chaque parent
	    p1 = genetic_tournament_parent(population);
	    p2 = genetic_tournament_parent(population);;
	    
	    // créer le fils (new_pop[i] == fils)
            genetic_croisement_generate_child(&new_population[i], p1, p2);

	    //génère une mutation
	    if((float) (rand() / RAND_MAX) < MUTATION_RATE) genetic_mutate(new_population[i]);
            
        }

	genetic_copy_popultaion(&new_population, &population, num_vertices);
	
        iteration++;
    }
    
    // free les individu de la population
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
	genetic_destroy_individu(population[i]);

    }
    free(population);
    
    // free les individu de la population sans free le meilleur individu
    for (int i = 1; i < POPULATION_SIZE; i++)
    {
	genetic_destroy_individu(new_population[i]);

    }
    free(new_population);
    
    
    
    return best_distance;
}


/**
 * @brief Permet de recréer une population.
 *
 * tirage aléatoire, pas d'optimisation de zone de recherche
 *
 * @return individu ** : la population crée en mémoire, contenant des individu/chemin aléatoire
 */
individu_t ** genetic_re_initialize_population(individu_t ** population)
{    
    int k,i,j, tmp;
    int nb_vertices = population[0]->n;
    
    for (i = 0; i < POPULATION_SIZE; i++)
    {
	
        for (j = 0; j < nb_vertices; j++)
	{
            population[i]->path[j] = j;
        }
	
        // Mélanger le chemin aléatoirement
        for (j = nb_vertices - 1; j > 0; j--)
	{
            k = rand() % (j + 1);
            tmp = population[i]->path[j];

	    population[i]->path[j] = population[i]->path[k];
            population[i]->path[k] = tmp;
        }
    }
    return population;
}


/**
 * @brief Pemet de créer et récuperer le meilleur parent d'un tournoi. (taille du tournoi est fixée grâce à la MACRO NB_PARTICIPATION_TOURNOI
 *
 * @param individu_t ** population
 * 
 *@return individu * : meilleur parent du tournoi
 *
 */
individu_t * genetic_tournament_parent(individu_t ** population)
{
    float best_distance = FLT_MAX;
    individu_t * best_parent;
    int iteration =0;
    int random_indice;
    
    do
    {
	random_indice = rand()%POPULATION_SIZE;
	if(population[random_indice]->distance < best_distance)
	{
	    best_parent = population[random_indice];
	}
	++ iteration;
    }while(iteration < NB_PARTICIPATION_TOURNOI);

    return best_parent;
}

