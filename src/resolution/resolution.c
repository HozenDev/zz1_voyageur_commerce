#include "resolution.h"
#include <math.h>
#include "../utils/utils.h"
#include "../seed/seed.h"


/* prendre le meilleur parmi tous les voisins (méthode gloutonne exhaustive)  en partant de tous les sommets du graphe*/

/* prendre le meilleur parmi tous les voisins (méthode gloutonne exhaustive)  en partant de tous les sommets du graphe */

float glouton_exhaustive(float ** dist, int numVertices)
{
    float distMinimale = INFINITY;
    int sommetSuivant, distActuelle, sommetActuel;
    int *visite;
    float distMinVoisin;
    int i, j;
    
    for (i = 0; i < numVertices; ++i)
    {
	distActuelle = 0;
	sommetActuel = i;
	
        // Tableau pour marquer les sommets visités
        visite = (int*)malloc(numVertices * sizeof(int));
        for (j = 0; j < numVertices; ++j)
            visite[j] = 0;

        // Parcourir tous les sommets en partant de i
        do {
            visite[sommetActuel] = 1;
            sommetSuivant = -1;
            distMinVoisin = INFINITY;

            // Chercher le sommet voisin non visité avec la distance minimale
            for (j = 0; j < numVertices; ++j) {
                if (j != sommetActuel && !visite[j] && dist[sommetActuel][j] < distMinVoisin) {
                    distMinVoisin = dist[sommetActuel][j];
                    sommetSuivant = j;
                }
            }

            if (sommetSuivant != -1) {
                distActuelle += distMinVoisin;
                sommetActuel = sommetSuivant;
            }
        } while (sommetSuivant != -1);

        // Vérifier si la distance actuelle est plus petite que la distance minimale trouvée jusqu'à présent
        if (distActuelle + dist[i][sommetActuel] < distMinimale) {
            distMinimale = distActuelle + dist[i][sommetActuel];
        }

        free(visite);
    }

    return distMinimale;
}



void floydWarshall(graph_sdl_t * graph, float *** matrixFloyd)
{
    int numVertices = graph->g->n;

    *matrixFloyd = (float **) malloc(numVertices * sizeof(float *));

    for (int i = 0; i < numVertices; i++) {
        (*matrixFloyd)[i] = (float*)malloc(numVertices * sizeof(float));

        for (int j = 0; j < numVertices; j++) {
            (*matrixFloyd)[i][j] = graph->dist[i][j];
        }
    }
    
    for (int k = 0; k < numVertices; k++) {
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                if (   (*matrixFloyd)[i][k] != INFINITY
		    && (*matrixFloyd)[k][j] != INFINITY
		    && (*matrixFloyd)[i][k] + (*matrixFloyd)[k][j] < (*matrixFloyd)[i][j]) {
                    (*matrixFloyd)[i][j] = (*matrixFloyd)[i][k] + (*matrixFloyd)[k][j];
                }
            }
        }
    }
}
