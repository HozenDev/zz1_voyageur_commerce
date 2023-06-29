#ifndef _resolution_h_
#define _resolution_h_

#include "../graph/graph.h"

#define MAX_ANT_DAYS 100 /* Nombre de jours de colonisation de la fourmi */
#define NUM_ANTS 100 /* Nombre de fourmis */
#define T_PHEROMONE 5  /* Taux de phéromone */
#define T_DISTANCE 1    /* Influence de la distance */
#define T_EVAP 0.5     /* Taux d'évaporation des phéromones */
#define Q 100       /* Quantité de phéromone déposée par chaque fourmi */

/* résolution matrice des distances */

void floydWarshall(graph_sdl_t * graph, float *** matrixFloyd);

/* méthode de résolution */

/* glouton */
float glouton_exhaustive(float ** dist, int numVertices);

/* fourmi */
float resolution_ant_colony(float **dist, int n, int ** meilleur_parcours);

/* recuis_simule */
float resolution_recuis_simule(float ** dist,int taille,float (*pf)(float));


#endif
