#include "resolution.h"
#include <math.h>
#include "../utils/utils.h"
#include "../seed/seed.h"


float resolution_recuis_simule(float ** dist,int taille)
{
    float distmin;
    float distance;
    int *solution=(int*)malloc(sizeof(int)*taille);
    int *new=(int*)malloc(sizeof(int)*taille);
    float temperature=1000,espsilon=0.001,tauxderefroidissement=0.999;
    generate_seed(0);
    for(int i=0;i<taille;i++)
    {
        solution[i]=i;
    }
    utils_distance_liste(solution,dist,&distmin,taille);
    while(temperature>espsilon)
    {
        utils_copy_list(solution,new,taille);
        utils_shuffle(new,taille,temperature);
        utils_distance_liste(new,dist,&distance,taille);
        
        if(distance<distmin || rand()<exp(-(distance-distmin)/temperature))
        {
            distmin=distance;
            utils_copy_list(new,solution,taille);
        }
        temperature=temperature*tauxderefroidissement;
    }
    return(distmin);
}   


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



void resolution_initialize_phero_visibility(float ** dist, float *** pheromones, float *** visibility, int numVertices)
{
    // Initialisation des tableaux de phéromones et de visibilité == inverse de la distance
    (*pheromones) = (float **)malloc(numVertices * sizeof(float *));
    (*visibility) = (float **)malloc(numVertices * sizeof(float *));

        
    for (int i = 0; i < numVertices; i++) {
        (*pheromones)[i] = (float *)malloc(numVertices * sizeof(float));
        (*visibility)[i] = (float *)malloc(numVertices * sizeof(float));
	
        for (int j = 0; j < numVertices; j++) {
            (*pheromones)[i][j] = 1.0;                  // Toutes les arêtes ont une quantité initiale de phéromones
            (*visibility)[i][j] = 1.0 / dist[i][j];     // Visibilité est l'inverse de la distance
        }
    }
}

float resolution_ant_colony(float **dist, int numVertices)
{
    int iteration = 0;
    int *bestTour = (int *)malloc(numVertices * sizeof(int));  // Meilleur tour global trouvé
    float bestTourLength = INFINITY;                            // Longueur du meilleur tour global

    float ** pheromones, ** visibility;

    resolution_initialize_phero_visibility(dist, &pheromones, &visibility, numVertices);
    
    // Boucle principale de l'algorithme
    while (iteration < 100) {  // Nombre d'itérations à effectuer

        int **antsTours = (int **)malloc(NUM_ANTS * sizeof(int *));     // Tours construits par chaque fourmi
        float *antsTourLengths = (float *)malloc(NUM_ANTS * sizeof(float)); // Longueurs des tours construits par chaque fourmi

        // Construction des tours pour chaque fourmi
        for (int ant = 0; ant < NUM_ANTS; ant++) {
            antsTours[ant] = (int *)malloc(numVertices * sizeof(int));
            int *visited = (int *)malloc(numVertices * sizeof(int));
            for (int i = 0; i < numVertices; i++) {
                visited[i] = 0; // Aucun sommet visité
            }

            int start = rand() % numVertices; // Choix aléatoire du sommet de départ
            antsTours[ant][0] = start;
            visited[start] = 1;

            // Construction du tour en utilisant la règle de probabilité de transition
            for (int i = 1; i < numVertices; i++) {
                int current = antsTours[ant][i - 1];
                int next = -1;
                float *  probabilities = malloc(sizeof(float)*numVertices);
                float sum = 0.0;

                // Calcul des probabilités de transition vers les sommets non visités
                for (int j = 0; j < numVertices; j++) {
                    if (visited[j] == 0) {
                        probabilities[j] = pow(pheromones[current][j], ALPHA) * pow(visibility[current][j], BETA);
                        sum += probabilities[j];
                    } else {
                        probabilities[j] = 0.0;
                    }
                }

                // Sélection du prochain sommet en utilisant les probabilités normalisées
                float random = ((float)rand() / RAND_MAX);
                float cumulativeProbability = 0.0;
                for (int j = 0; j < numVertices; j++) {
                    probabilities[j] /= sum;
                    cumulativeProbability += probabilities[j];
                    if (random <= cumulativeProbability) {
                        next = j;
                        break;
                    }
                }

                antsTours[ant][i] = next;
                visited[next] = 1;
            }

            // Calcul de la longueur du tour construit par la fourmi
            float tourLength = 0.0;
            for (int i = 0; i < numVertices - 1; i++) {
                int current = antsTours[ant][i];
                int next = antsTours[ant][i + 1];
                tourLength += dist[current][next];
            }
            tourLength += dist[antsTours[ant][numVertices - 1]][antsTours[ant][0]]; // Retour au sommet de départ
            antsTourLengths[ant] = tourLength;

            // Mise à jour du meilleur tour global si nécessaire
            if (tourLength < bestTourLength) {
                bestTourLength = tourLength;
                for (int i = 0; i < numVertices; i++) {
                    bestTour[i] = antsTours[ant][i];
                }
            }

            free(visited);
        }

        // Mise à jour des phéromones sur toutes les arêtes
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                if (i != j) {
                    // Évaporation des phéromones
                    pheromones[i][j] *= (1.0 - RHO);

                    // Dépôt de phéromones par chaque fourmi
                    for (int ant = 0; ant < NUM_ANTS; ant++) {
                        int current = antsTours[ant][i];
                        int next = antsTours[ant][j];
                        pheromones[current][next] += (Q / antsTourLengths[ant]);
                    }
                }
            }
        }

        // Libération de la mémoire
        for (int ant = 0; ant < NUM_ANTS; ant++) {
            free(antsTours[ant]);
        }
        free(antsTours);
        free(antsTourLengths);

        iteration++;
    }

    // Affichage du meilleur tour
    printf("Meilleur tour trouvé : ");
    for (int i = 0; i < numVertices; i++) {
        printf("%d ", bestTour[i]);
    }
    printf("%d\n", bestTour[0]); // Retour au sommet de départ

    // Libération de la mémoire
    for (int i = 0; i < numVertices; i++) {
        free(pheromones[i]);
        free(visibility[i]);
    }
    free(pheromones);
    free(visibility);
    free(bestTour);

    return bestTourLength;
}
