#include "resolution.h"
#include "../graph/graph.h"
#include <math.h>
/**
 * @brief Creates a copy of a matrix
 *
 * @param float ** srxmatrix ,source matrix
 * @param float ** destmatrix ,destination matrix
 * @param int n ,matrixes size
 */
void tools_matrix_copy(float ** srxmatrix,float ** destmatrix,int n)
{
    for (int i=0;i<n;i++){
        for(int j=i + 1; j<n;j++){
            destmatrix[i][j]=srxmatrix[i][j];
        }
    }
}



void reoslution_initialisation_predececeurs_matrix(int **predececeurs,float **dist,int taille)
{
    
    for(int i=0;i<taille;i++){
        for(int j=0;j<taille;j++){
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
float ** resolution_matrice_minimale(graph_sdl_t * graph_sdl)
{   
    int taille=graph_sdl->g.n;
    float ** matrixmin;
    int ** predececeurs;
    matrixmin = (float **)malloc(sizeof(float *)* taille);
    
    predececeurs= (int **)malloc(sizeof(int *)*taille);
    reoslution_initialisation_predececeurs_matrix(predececeurs,matrixmin,taille);
    

    
    /*initialising matrixmin*/
    tools_matrix_copy(graph_sdl->dist,matrixmin,taille);

    /* for each k we allow the path to cross the k-1 first vertexes of the graph*/
    for(int k=0;k<taille;k++){
        for(int i=0;i<taille;i++){
            for(int j=i+1;j<taille;j++){
                if(matrixmin[i][j]>matrixmin[i][k]+matrixmin[k][j])
                {
                    matrixmin[i][j]=matrixmin[i][k]+matrixmin[k][j];
                    predececeurs[i][j]=predececeurs[k][j];
                }
            }
        }
        
    }
    return(matrixmin);
}   


int resolution_main()
{
    return(0);
}