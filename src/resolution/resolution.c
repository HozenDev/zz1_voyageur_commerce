#include "resolution.h"
#include <math.h>
#include "../utils/utils.h"
#include "../seed/seed.h"

/**
 * @brief méthode de résolution par recuis simulé
 *
 * @param dist, matrice des distances
 * @param taille, nombre de noeuds du graphe
 *
 * @return la distance du cycle minimale
 */
float resolution_recuis_simule(float ** dist, int taille,float (*pf)(float))
{
    /* paramètres */
    float distmin=taille*2000;
    float distance=0;
    float temperature = 1000, espsilon = 0.001;
   
    /* allocation des tableaux de solutions et solutions + 1 */
    int * solution = (int *) malloc(sizeof(int)*taille);
    int * new = (int *) malloc(sizeof(int)*taille);

    for(int i=0;i<taille;i++) solution[i]=i; /* initialisation solution */

    utils_distance_liste(solution, dist, &distmin, taille);

    while (temperature > espsilon)
    {
        utils_copy_list(solution,new,taille);
        utils_shuffle(new,taille);
        utils_distance_liste(new,dist,&distance,taille);
        
        if(distance<distmin || rand()/RAND_MAX<exp(-(distance-distmin)/temperature))
        {
            distmin=distance;
            utils_copy_list(new,solution,taille);
        }
        temperature=pf(temperature);
    }

    free(solution);
    free(new);
    return distmin;
}   

/** @brief Algorithme glouton exhaustive: prendre le meilleur parmi tous les voisins
 *
 * @param dist, matrice des distances
 * @param taille, nombre de noeuds du graphe
 *
 * @return la distance du cycle minimale
 */
float glouton_exhaustive(float ** dist, int n)
{
    float dist_min_global = INFINITY;
    int next, dist_actuelle, curr;
    int * visite;
    float dist_min_local;
    int i, j;

    visite = (int*) malloc(sizeof(int)*n);
    
    for (i = 0; i < n; ++i)
    {
	dist_actuelle = 0;
	curr = i;

        for (j = 0; j < n; ++j) visite[j] = 0;

        // Parcourir tous les sommets en partant de i
        do {
            visite[curr] = 1;
            next = -1;
            dist_min_local = INFINITY;

            // Chercher le sommet voisin non visité avec la distance minimale
            for (j = 0; j < n; ++j) {
                if (j != curr && !visite[j] && dist[curr][j] < dist_min_local) {
                    dist_min_local = dist[curr][j];
                    next = j;
                }
            }

            if (next != -1) {
                dist_actuelle += dist_min_local;
                curr = next;
            }
        } while (next != -1);

        // Vérifier si la distance actuelle est plus petite que la distance minimale trouvée jusqu'à présent
        if (dist_actuelle + dist[i][curr] < dist_min_global) {
            dist_min_global = dist_actuelle + dist[i][curr];
        }
    }

    free(visite);

    return dist_min_global;
}

/** @brief Algorithme Floyd Warshall
 *
 * @param graph, le graphe qui contient les distances initialisées
 * @param matrixFloyd, matrice des plus petites distances retournées par l'algorithme
 */
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

/**
 * @brief Initialise les phéromones et la visibilé dans la résolution par colonisation de fourmi
 *
 * @param dist, matrix of distances
 * @param pheromones, adress of the matrix of pheromones
 * @param pheromones, adress of the matrix of visibilities
 * @param n, number of points
 */
void resolution_initialize_phero_visibility(float ** dist, float *** pheromones, float *** visibility, int n)
{
    int i, j;
    
    /* Initialisation des tableaux de phéromones et de visibilité == inverse de la distance */
    (*pheromones) = (float **) malloc(n*sizeof(float *));
    (*visibility) = (float **) malloc(n*sizeof(float *));
        
    for (i = 0; i < n; i++)
    {
        (*pheromones)[i] = (float *) malloc(n * sizeof(float));
        (*visibility)[i] = (float *) malloc(n * sizeof(float));
        for (j = 0; j < n; j++)
        {
            (*pheromones)[i][j] = 1.0;                  // Toutes les arêtes ont une quantité initiale de phéromones
            (*visibility)[i][j] = 1.0 / dist[i][j];     // Visibilité est l'inverse de la distance
        }
    }
}

/**
 * @brief Update a tour of an ant
 *
 * @param fourmi_parcours, adress of a tour of an ant
 * @param n, size of the tour
 * @param pheromones, pheromones to compute probabilities
 * @param visibility, visibilities to compute probabilities
 */
void resolution_ant_colony_compute_next_tour(int ** fourmi_parcours, int n,
                                            float ** pheromones, float ** visibility)
{
    int start;
    int curr;
    int next;

    int * visited;

    float sum = 0.0;
    float * probabilities = (float *) malloc(sizeof(float)*n);
    float random;
    float cumulativeProbability;

    int i, j;
    
    /* initialisation du parcours de la fourmi */
    visited = (int *) calloc(n, sizeof(int));

    start = rand() % n; /* choix aléatoire du sommet de départ */
    (*fourmi_parcours)[0] = start; /* initialisatio sommet de départ */
    visited[start] = 1; /* le départ devient donc visité */
    
    // Construction du tour en utilisant la règle de probabilité de transition
    for (i = 1; i < n; i++)
    {
        curr = (*fourmi_parcours)[i - 1];
        next = -1;
        sum = 0.0;

        // Calcul des probabilités de transition vers les sommets non visités
        for (j = 0; j < n; j++) {
            if (visited[j] == 0) {
                probabilities[j] = pow(pheromones[curr][j], T_PHEROMONE) * pow(visibility[curr][j], T_DISTANCE);
                sum += probabilities[j];
            } else {
                probabilities[j] = 0.0;
            }
        }

        // Sélection du prochain sommet en utilisant les probabilités normalisées
        random = ((float)rand() / RAND_MAX);
        cumulativeProbability = 0.0;
        for (j = 0; j < n; j++) {
            probabilities[j] /= sum;
            cumulativeProbability += probabilities[j];
            if (random <= cumulativeProbability) {
                next = j;
                break;
            }
        }

        (*fourmi_parcours)[i] = next;
        visited[next] = 1;
    }

    free(probabilities);
    free(visited);
}

/**
 * @brief Compute a distance of a tour
 *
 * @param dist, the distances matrix
 * @param fourmi_parcours, a tour of an ant
 * @param n, size of the tour
 */
float resolution_ant_colony_compute_distance(float ** dist, int * fourmi_parcours, int n)
{
    // Calcul de la longueur du tour construit par la fourmi
    float total_dist = 0.0;
    int i;

    for (i = 0; i < n - 1; i++)
        total_dist += dist[fourmi_parcours[i]][fourmi_parcours[i+1]];
    total_dist += dist[fourmi_parcours[n-1]][fourmi_parcours[0]]; // Retour au sommet de départ

    return total_dist;
}

/**
 * @brief Update pheromones matrix
 *
 * @param pheromones, adress of pheromones to update
 * @param fourmi_parcours, array of all ant tour
 * @param fourmi_distances, array of all ant distance compute with result tour
 * @param n, size of the tour
 */
void resolution_ant_colony_update_pheromones(float *** pheromones, int ** fourmi_parcours, float * fourmi_distances, int n)
{
    int i, j, ant;

    int curr;
    int next;
    
    // Mise à jour des phéromones sur toutes les arêtes
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (i != j) {
                // Évaporation des phéromones
                (*pheromones)[i][j] *= (1.0 - T_EVAP);

                // Dépôt de phéromones par chaque fourmi
                for (ant = 0; ant < NUM_ANTS; ant++) {
                    curr = fourmi_parcours[ant][i];
                    next = fourmi_parcours[ant][j];
                    (*pheromones)[curr][next] += (Q / fourmi_distances[ant]);
                }
            }
        }
    }
}

/**
 * @brief Solves the traveller's problem with the ant collony methods
 *
 * @param dist, the distances matrix
 * @param n, size of the tour
 * @param meilleur_parcours, best tour compute by the ant colony algorithm
 *
 * @return dist_min_parcours, the minimal distance founded
 */
float resolution_ant_colony(float **dist, int n, int ** meilleur_parcours)
{
    int jour = 0;

    float ** pheromones, ** visibility;

    float dist_parcours;
    float dist_min_parcours = INFINITY;

    int ** fourmi_parcours;
    float * fourmi_distances;

    int i, ant;

    *meilleur_parcours = (int *) malloc(n * sizeof(int));  /* meilleur parcours */

    resolution_initialize_phero_visibility(dist, &pheromones, &visibility, n);

    fourmi_parcours = (int **) malloc(NUM_ANTS * sizeof(int *)); /* parcours construits par chaque fourmi */
    fourmi_distances = (float *) malloc(NUM_ANTS * sizeof(float)); /* distance parcours pour chaque fourmi */

    for (i = 0; i < NUM_ANTS; ++i)
    {
        fourmi_parcours[i] = (int *) malloc(sizeof(int)*n);
    }

    /* Nombre de jour à faire travailler la colonie */
    while (jour < MAX_ANT_DAYS) {

        /* construction des parcours pour chaque fourmi */
        for (ant = 0; ant < NUM_ANTS; ant++)
        {
            /* calcul le tour d'une fourmi */
            resolution_ant_colony_compute_next_tour(&(fourmi_parcours[ant]), n, pheromones, visibility);

            /* met à jour la distance du parcours */
            dist_parcours = resolution_ant_colony_compute_distance(dist, fourmi_parcours[ant], n);
            fourmi_distances[ant] = dist_parcours;

            /* met à jour la meilleure distance */
            if (dist_parcours < dist_min_parcours) {
                dist_min_parcours = dist_parcours;
                /* copie du meilleurs parcours */
                for (i = 0; i < n; i++)
                    (*meilleur_parcours)[i] = fourmi_parcours[ant][i];
            }
        }
        resolution_ant_colony_update_pheromones(&pheromones, fourmi_parcours, fourmi_distances, n);
        jour++;
    }

    /* Libération de la mémoire */
    for (i = 0; i < n; i++) {
        free(pheromones[i]);
        free(visibility[i]);
    }
    for (i = 0; i < NUM_ANTS; ++i) {
        free(fourmi_parcours[i]);
    }
    free(pheromones);
    free(visibility);
    free(fourmi_parcours);
    free(fourmi_distances);

    return dist_min_parcours;
}




