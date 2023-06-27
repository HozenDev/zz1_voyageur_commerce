#include "graph.h"
#include "stdlib.h"


void graph_print_sdl

int graph_main(void)
{
    return 0;
}


/**
 * \fn void genere(graphe_t ** matrice, int bas, int haut)
 * \brief Fonction qui génère un graphe connexe aléatoire
 * 
 *  
 * \param[in] 
 * \param[in] 
 * \param[in] 
 * 
 * \return void : pas de retour, effet de bord sur le graphe
 * 
 */
void genere(graphe_t matrice, unsigned int bas, unsigned int haut)
{
    int k = 0; 
    if (bas < haut)
    {
	k = rand() % (haut - (bas +1) +1) + (bas +1); 
	matrice[bas][bas +1] = 1;
	
    }
}
