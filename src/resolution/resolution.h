#ifndef _resolution_h_
#define _resolution_h_

#include "../graph/graph.h"



#define NUM_ANTS 100 // Nombre de fourmis
#define ALPHA 5  // Influence du phéromone
#define BETA 1    // Influence de la distance
#define RHO 0.5     // Taux d'évaporation des phéromones
#define Q 100       // Quantité de phéromone déposée par chaque fourmi


int resolution_main(graph_sdl_t * graph,int * cyclemin);

float ** resolution_matrice_minimale(graph_sdl_t * graph_sdl,int ** predececeurs);

void reoslution_initialisation_predececeurs_matrix(int ** predececeurs, float ** dist, int taille);

int * resolution_construire_chemin(float ** dist,int ** predececeurs,int depart,int arrive,int taille);

int resolution_construire_cycle_min(int * cyclemin,int ** predececeurs,float ** matrixmin,int taille);
void resolution_genere_solution_initial(graph_sdl_t* graph,int depart,int *solution);

void floydWarshall(graph_sdl_t * graph, float *** matrixFloyd);
float glouton_exhaustive(float ** dist, int numVertices);

float resolution_ant_colony(float **dist, int numVertices);

float resolution_recuis_simule(float ** dist,int taille);


#endif
