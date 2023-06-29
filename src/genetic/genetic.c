#include "genetic.h"


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
 * @return individu ** : la population crée en mémoire, contenant des individu/chemin aléatoire
 */
individu_t ** genetic_initialize_population(int nb_vertices, int population_size)
{    
    individu_t ** population = malloc(population_size * sizeof(individu_t *));
    int k,i,j, tmp;
    
    for (i = 0; i < population_size; i++)
    {
        population[i] = genetic_create_individu(nb_vertices);
	
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
 * @brief Permet de calculer la distance qu'effectu un individu
 *
 * @param float ** dist       : matrice de floyd warshall
 * @param individu * individu : individu/voyageur, membre d'une population
 *
 * @return la distance du chemin
 */
float genetic_evaluate_distance(float ** dist, individu_t * individu)
{
    float res = FLT_MAX;
    
    utils_distance_liste(individu->path, dist, &res, individu->n);
    
    return res;
}


/**
 * @brief Permet d'effectuer un croisement entre 2 parents pour obtenir un individu fils
 *
 * On tire un 
 *
 * @param individu_t * p1 : parent 1
 * @param individu_t * p2 : parent 2
 *
 * @return individu_t * : l'individu fils créé 
 */
individu_t * genetic_croisement_generate_child(individu_t * p1, individu_t * p2)
{
    individu_t * child = genetic_create_individu(p1->n);

    int startPos = rand() % p1->n;
    int endPos   = rand() % p2->n;
    
    bool* visited = calloc(p1->n, sizeof(bool)); // 0 si non visité, 1 sinon

    int i, j;
    
    // Copier la portion du parent 1 dans l'enfant
    for (i = startPos; i != endPos; i = (i + 1) % p1->n) // gere le cas si start < end ou si end < start
    {
        child->path[i] = p1->path[i];
        visited[p1->path[i]] = true;
    }
    
    // Copier les éléments du parent 2 qui ne sont pas déjà dans l'enfant
    j = endPos;
    for (i = endPos; i < p2->n; i++)
    {
        if (!visited[p2->path[i]])
	{
            child->path[j] = p2->path[i];
            visited[p2->path[i]] = true;
            j = (j + 1) % p2->n;
        }
    }
    
    for (int i = 0; i < endPos; i++)
    {
        if (!visited[p2->path[i]])
	{
            child->path[j] = p2->path[i];
            visited[p2->path[i]] = true;
            j = (j + 1) % p2->n;
        }
    }
    free(visited);
    return child;
}



/**
 * @brief Permet de trouver le meilleur individu au sein d'une population
 *

 * @param individu_t ** population :
 * @param int population_size
 *
 * @return individu_t * : le meilleur individu de la population 
 */
individu_t * genetic_find_best_individu_in_pop(individu_t ** population, int population_size) {
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



/*
individu_t * solveTSPGenetic(float** matrixFloydWarshall, int num_vertices)
{
    individu_t ** population = genetic_initialize_population(num_vertices, POPULATION_SIZE);
    float bestFitness = FLT_MAX;
    int iteration = 0;
    
    while (iteration < MAX_ITERATIONS) {
        for (int i = 0; i < POPULATION_SIZE; i++) {
            float genetic_evaluate_distance(matrixFloydWarshall, population[i], num_vertices);
        }
        
        individu_t * bestSolution = genetic_find_best_individu_in_pop(population, POPULATION_SIZE);
        if (bestSolution->distance < bestFitness) {
            bestFitness = bestSolution->distance;
        }
        
        individu_t** newPopulation = malloc(POPULATION_SIZE * sizeof(individu_t*));
        newPopulation[0] = bestSolution;  // Elitisme
        
        // Sélection, croisement et mutation pour générer la nouvelle population
        for (int i = 1; i < POPULATION_SIZE; i++) {
            Solution* parent1 = population[rand() % POPULATION_SIZE];
            Solution* parent2 = population[rand() % POPULATION_SIZE];
            Solution* child = crossover(parent1, parent2, num_vertices);
            mutate(child, num_vertices);
            newPopulation[i] = child;
        }
        
        // Remplacer l'ancienne population par la nouvelle
        for (int i = 0; i < POPULATION_SIZE; i++) {
            destroySolution(population[i]);
        }
        free(population);
        population = newPopulation;
        
        iteration++;
    }
    
    Solution* bestSolution = findBestSolution(population, POPULATION_SIZE);
    
    // Libérer la mémoire
    for (int i = 0; i < POPULATION_SIZE; i++) {
        if (population[i] != bestSolution) {
            destroySolution(population[i]);
        }
    }
    free(population);
    
    return bestSolution;
}
*/
