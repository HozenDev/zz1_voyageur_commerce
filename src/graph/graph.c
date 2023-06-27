#include "graph.h"

#include "../seed/seed.h"
#include <stdlib.h>

graph_t * graph_generategraph(char ** g,unsigned short N,float p)
{
    graph_t * graph= (graph_t *) malloc (sizeof(graph_t));
    if(NULL != graph)
        generate_seed(0);
        for(int i=0; i<N; i++){
            for(int j=i+1; j<N; j++){
                if((float)rand()/RAND_MAX < p)
                {
                    g[i][j]=1;
                }
            }
        }
        graph->matrix=g;
        graph->n=N;
    return(graph);
    

}



int graph_main(void)
{
    char g[4][4]={{0,1,1,0},{1,0,0,1},{1,0,0,0},{0,1,0,0}};
    graph_t * graph=generegraph(g,4,4,0.1);
    char ** g2=graph_t->g;
    for (int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            printf("%c",g2[i][j]);
        }
    }
    return 0;
}


/**
 * \fn void genere(graphe_t ** matrice, int bas, int haut)
 * \brief Fonction qui génère un graphe connexe aléatoire
 * 
 *  
 * \param[in] graph_t graph
 * \param[in] unsigned short down
 * \param[in] unsigned short up
 * 
 * \return void : pas de retour, effet de bord sur le graphe
 * 
 */
void graph_generate_related(graph_t graph, unsigned short down, unsigned short up)
{
    unsigned int k = 0; 
    if (down < up)
    {
	k = rand() % (up - (down +1) +1) + (down +1); 
	graph.matrix[down][down +1] = 1;
	graph.matrix[down+1][down] = 1;
	if (k+1 <= up)
	{
	    graph.matrix[down][k+1] = 1;
	    graph.matrix[k+1][down] = 1;
	}
	graph_generate_related(graph, down+1, k);
	graph_generate_related(graph, k+1, up);
    }
}




void graph_initialize_graph();
