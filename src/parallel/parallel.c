#include "parallel.h"
#include "../graph/graph.h"
#include "../resolution/resolution.h"
#include "../log/log.h"
#include "../genetic/genetic.h"

#define N_VERTICES 20

int all_time;

int parallel_treatment_not_a_function(void * parameters)
{
    (void) parameters;
    zlog(stdout, ERROR, "Ce n'est pas une fonction valide", NULL);
    return 0;
}

int parallel_treatment_genetique(void * parameters)
{
    float ** min_dist = (float **) parameters;

    unsigned long millis;
    clock_t begin;
    clock_t end;
    int * best_tour = NULL;
    float result;

    begin = clock();
    result = genetic_solve(min_dist, N_VERTICES);
    end = clock();
    millis = (end -  begin) * 1000 / CLOCKS_PER_SEC;
    zlog(stdout, INFO, "GENETIC SOLVE: %f - Finished in %ld ms", result, millis);

    free(best_tour);

    return 0;
}

int parallel_treatment_ant_colony(void * parameters)
{
    float ** min_dist = (float **) parameters;

    unsigned long millis;
    clock_t begin;
    clock_t end;
    int * best_tour = NULL;
    float result;

    begin = clock();
    result = resolution_ant_colony(min_dist, N_VERTICES, &best_tour);
    end = clock();
    millis = (end -  begin) * 1000 / CLOCKS_PER_SEC;
    zlog(stdout, INFO, "COLONI DE FOURMI: %f - Finished in %ld ms", result, millis);

    free(best_tour);

    return 0;
}

int parallel_treatment_recuis_simule(void * parameters)
{
    float ** min_dist = (float **) parameters;

    unsigned long millis;
    clock_t begin;
    clock_t end;
    float result;

    begin = clock();
    result = resolution_recuis_simule(min_dist, N_VERTICES, &utils_descente_geometrique);
    end = clock();
    millis = (end -  begin) * 1000 / CLOCKS_PER_SEC;
    zlog(stdout, INFO, "RECUIS SIMULE : %f - Finished in %ld ms", result, millis);

    return 0;
}

int parallel_treatment_glouton(void * parameters){
    float ** min_dist = (float **) parameters;

    unsigned long millis;
    clock_t begin;
    clock_t end;
    float result;
    
    begin = clock();
    result = glouton_exhaustive(min_dist, N_VERTICES);
    end = clock();
    millis = (end -  begin) * 1000 / CLOCKS_PER_SEC;
    zlog(stdout, INFO, "GLOUTON EXH : %f - Finished in %ld ms", result, millis);

    return 0;
}

int thread_main(int nb_thread)
{
    int i, running, i_function;
    
    thrd_t * thread_handle = NULL;
    int * error_code_of_thread = NULL;
    int (*fres) (void *) = NULL;

    /* int n = generate_random_number(N_MIN, N_MAX); */
    int n = N_VERTICES;
    float ** min_dist;
    graph_sdl_t * gs = graph_initialize_graph_sdl(n, 100, 800, 0.8, 0.5);
    floydWarshall(gs, &min_dist);

    thread_handle = (thrd_t *) malloc(sizeof(thrd_t)*nb_thread);
    error_code_of_thread = (int *) malloc(sizeof(int)*nb_thread);

    running = 1;
    
    while (running)
    {
        fprintf(stdout, "\nQuelle fonction ? [0: GE, 1: RS, 2: CDF, 3: G] ");
        fscanf(stdin, "%d", &i_function);
        fprintf(stdout, "\n");
        
        switch (i_function)
        {
        case 0: fres = parallel_treatment_glouton; break;
        case 1: fres = parallel_treatment_recuis_simule; break;
        case 2: fres = parallel_treatment_ant_colony; break;
        case 3: /* fres = parallel_treatment_genetique; */ break;
        default: fres = parallel_treatment_not_a_function; break;
        }
        
        for (i = 0; i < nb_thread; ++i)
        {
            error_code_of_thread[i] = 0;
            thrd_create(&thread_handle[i], *fres, min_dist);
        }
        for (i = 0; i < nb_thread; ++i) thrd_join(thread_handle[i], &error_code_of_thread[i]);

        fprintf(stdout, "\nContinuer ? [0: non, 1: oui] ");
        if (fscanf(stdin, "%d", &running) < 1)
            running = 0;
        fprintf(stdout, "\n");
    }

    graph_free_graph_sdl(gs);
    free_matrix_float(min_dist, n);
    free(thread_handle);
    free(error_code_of_thread);
    
    return 0;
}
