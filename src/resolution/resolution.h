#ifndef _resolution_h_
#define _resolution_h_

#include "../graph/graph.h"
int resolution_main(graph_sdl_t * graph,int * cyclemin);
float ** resolution_matrice_minimale(graph_sdl_t * graph_sdl,int ** predececeurs);

void reoslution_initialisation_predececeurs_matrix(int *** predececeurs, float ** dist, int taille);

int * resolution_construire_chemin(float ** dist,int ** predececeurs,int depart,int arrive,int taille);

float resolution_construire_cycle_min(int * cyclemin,int ** predececeurs,float ** matrixmin,int taille);

int res_main(void);
#endif
