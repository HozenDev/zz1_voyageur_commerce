#include "resolution.h"
#include <math.h>
#include "../utils/utils.h"


/**
 *@brief initialise the predececeurs matrix i.e. each i,j cells contains the previous vertex of j in the path from i to j
 *
 *@param int ** predececeurs,empty matrix that will be filled with the predececeurs
 *@param float ** dist,matrix of the distances within the graph
 *@param taille, number of rows and collumns
 */
void reoslution_initialisation_predececeurs_matrix(int **predececeurs,float **dist,int taille)
{
    
    for(int i=0;i<taille;i++){
        for(int j=i+1;j<taille;j++){
            if(dist[i][j]!=INFINITY)
                predececeurs[i][j]=i;
            else
                predececeurs[i][j]=-1;
        }
    }
    

}

/**
  *@brief generate the matrix of minimal distances using  Floyd Warshal methode
  *
  *@param graph_sdl_t * grapht_sdl, graph a optimiser
  */
float ** resolution_matrice_minimale(graph_sdl_t * graph_sdl,int ** predececeurs)
{   
    int taille=graph_sdl->g->n;
    float ** matrixmin;
    matrixmin = (float **)malloc(sizeof(float *)* taille);
    for(int i=0;i<taille;i++)
    {
        matrixmin[i]=(float *) malloc(sizeof(float)*taille);
    }
    printf("ma\n");

    if(matrixmin!=NULL)
    {
        
            
        /*initialising predeceurs matrix*/
        reoslution_initialisation_predececeurs_matrix(predececeurs,matrixmin,taille);
        printf("ma\n");


        /*initialising matrixmin*/
        utils_matrix_copy(graph_sdl->dist,matrixmin,taille);
        printf("ma\n");


        /* for each k we allow the path to cross the k-1 first vertexes of the graph*/
        for(int k=0;k<taille;k++){
            for(int i=0;i<taille;i++){
                for(int j=0;j<taille;j++){
                    if(matrixmin[i][j]>matrixmin[i][k]+matrixmin[k][j])
                    {
                        matrixmin[i][j]=matrixmin[i][k]+matrixmin[k][j];
                        predececeurs[i][j]=predececeurs[k][j];
                    }
                }
            }
        }
            
        
    }
    return(matrixmin);

}   
/**
 *@brief build the path between two points
 *
 *@param float ** dist, distances matrix within the graph
 *@param int ** predececeurs, previous vertexes matrix i.e. i,j cell has the previous vertex of j in the path from i to j
 *@param int depart,vertex of departure
 *@param int arrive,vertex of landing
 */
int * resolution_construire_chemin(float ** dist,int ** predececeurs,int depart,int arrive,int taille)
{
    int *chemin=(int *)malloc(sizeof(int)*taille);
    int iteration=0;
    if (dist[depart][arrive]==INFINITY)
        return(chemin);
    chemin[iteration]=depart;
    while(depart!=arrive){  
        iteration++;
        depart=predececeurs[depart][arrive];
        chemin[iteration]=depart;

    }
    return(chemin);
    

    
}
/**
 *@brief using floyd Warshal methods calculate the best cycle possible starting from a any point
 *
 *@param int * cyclemin, table built to contain the cycle of minimal distance
 *@param int ** predececeurs, predececeurs matrix containing the before last vertexes of the path
 *@param float ** distance, matrix containing minimal distances to go from i to j
 *@param int taille, size of matrixes = number of points
 */
float resolution_construire_cycle_min(int * cyclemin,int ** predececeurs,float ** matrixmin,int taille){
    int depart=0,current=0,nbpointparc=0,*cycle,cycledist=0;
    float distmin=INFINITY;
    cycle=(int *)malloc(sizeof(int)*taille);

    //On parcours tout les points de départs
    for(depart=0;depart<taille-1;depart++){
        nbpointparc=0;
        current=depart+1;

        /*on cherche un cycle*/
        do{
            cycle[nbpointparc++]=current;
            current=predececeurs[current][depart];
            printf("%d,%d,%d\n",current,depart,nbpointparc);
        }while(current!=depart && current!=-1);

        /* on verifie que ce cycle passe par tout les points*/
        if(nbpointparc==taille && current==depart)
        {
            cycledist=0;
            /*on calcul la distance de ce cycle*/
            for(int i=0;i<taille;i++)
            {
                cycledist+=matrixmin[cycle[i]][cycle[i+1]];
            }
            /* si cette distance est plus petite alors on met ajour le cyclemin et la distance min */
            if(cycledist<distmin)
            {
                distmin=cycledist;
                for(int j=0;j<taille;j++)
                {
                    cyclemin[j]=cycle[j];
                }
            }
        }

    }
    return(distmin);
}

float resolution_main(graph_sdl_t * graph,int * cyclemin)
{
    float ** matrixmin;

    int taille=graph->g->n;
    int ** predececeurs = (int **)malloc(sizeof(int *)* taille);
    for(int i=0;i<taille;i++)
    {
        predececeurs[i]=(int *) malloc(sizeof(int)*taille);
    }
    printf("ma\n");

    if(predececeurs!=NULL){
        matrixmin=resolution_matrice_minimale(graph,predececeurs);
        printf("ma\n");

        return(resolution_construire_cycle_min(cyclemin,predececeurs,matrixmin,taille));
    }
    else
        return(-1);
}
int resolution_est_complet_cycle(int* taken,int taille){
    int res=1;
    for(int i=0;i<taille;i++)
    {
        if(taken[i]!=1)
        {
            res=0;
        }
    }
    return(res);
}
void resolution_genere_solution_initial(graph_sdl_t* graph,int depart,int *solution){
    
    int current=depart;
    int iteration=0;
    int temporary;
    int *taken=(int*)malloc(sizeof(int)*graph->g->n);
    utils_initlist0(taken,graph->g->n);
    while(resolution_est_complet_cycle(taken,graph->g->n)!=1){
        utils_initlist0(taken,graph->g->n);
        iteration=0;

        solution[iteration++]=depart;
        do{
            temporary=rand()%graph->g->n;
            if(temporary!=current && graph->g->matrix[min(temporary,current)][(max(temporary,current))]!=0){
                current=temporary;
                taken[current]=1;
                solution[iteration++]=current;


            }
            
        }while(current!=depart && iteration<150);
    }
    
    solution[iteration]=-1;
    
}
/*
float resolution_gloutonne_aleatoire(graph_sdl_t * graph,int * cyclemin){
    int * solution[200];
    for(int depart=0;depart<graph->g->n;depart++){
        resolution_genere_solution_initial(graph,depart,solution);
        
    }
}
*/