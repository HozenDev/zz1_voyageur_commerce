#ifndef _genetic_h_
#define _genetic_h_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>
 
#include "../graph/graph.h"
#include "../resolution/resolution.h"


#define MAX_ITERATIONS 1000
#define POPULATION_SIZE 100
#define MUTATION_RATE 0.1
#define NB_PARTICIPATION_TOURNOI 25 // doit etre > 1

/*
Grandes étapes :

1. Créer la population initiale.
2. Sélection des individus de la population qui deviendront des parents.
3. Ces parents ont des enfants qui constitueront la génération suivante.
4. Faire vivre et évoluer ces enfants.
5. Repartir au point 2 avec cette nouvelle génération.
*/

    
// Structure pour représenter une solution
struct individu_s {
    int * path;
    int n; // nombre de sommet
    float distance;
};
typedef struct individu_s individu_t;

individu_t * genetic_create_individu(int num_vertices);
void genetic_destroy_individu(individu_t ** individu);

void genetic_initialize_population(individu_t *** population, int nb_vertices, int population_size);
void genetic_copy_population(individu_t ** population_source, individu_t *** population_dest, int nb_vertices);
void genetic_copy_individu(individu_t src, individu_t * dst, int nb_vertices);
individu_t ** genetic_re_initialize_population(individu_t ** population);

individu_t * genetic_tournament_parent(individu_t ** population);
void genetic_croisement_generate_child(individu_t ** child, individu_t * p1, individu_t * p2);

individu_t * genetic_find_best_individu_in_pop(individu_t ** population, int population_size);
void genetic_mutate(individu_t * individu);
float genetic_solve (float** matrixFloydWarshall, int num_vertices, int ** best_path);
float genetic_evaluate_distance(float ** dist, individu_t * individu);

#endif
