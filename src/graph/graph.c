#include "graph.h"
#include "../seed/seed.h"

graph_t * graph_generategraph(char ** g,unsigned short N,float p)
{
    graph_t * graph=(graph_t *)malloc(sizeof(graph_t));
    if(NULL!=)
        generate_seed(0);
        for(int i=0;i<w;i++){
            for(int j=i+1;j<h;j++){
                if((float)rand()/RAND_MAX<p)
                {
                    g[i][j]=1;
                }
            }
        }
        graph->mattrix=g;
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
