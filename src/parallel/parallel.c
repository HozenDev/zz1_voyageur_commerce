#include "parallel.h"
#include "../graph/graph.h"
#include "../resolution/resolution.h"
#include "../log/log.h"
#include "../genetic/genetic.h"

#define N 50

int parallel_treatment_genetique(void * parameters){
    float ** min_dist = (float **) parameters;

    unsigned long millis;
    clock_t begin;
    clock_t end;
    int * best_tour = NULL;

    begin = clock();
    fprintf(stdout, "GENETIC SOLVE: %f\n", genetic_solve(min_dist, N));
    end = clock();
    millis = (end -  begin) * 1000 / CLOCKS_PER_SEC;
    zlog(stdout, INFO, "GENETIC SOLVE: Finished in %ld ms", millis);

    free(best_tour);

    return 0;
}

int parallel_treatment_ant_colony(void * parameters){
    float ** min_dist = (float **) parameters;

    unsigned long millis;
    clock_t begin;
    clock_t end;
    int * best_tour = NULL;

    begin = clock();
    fprintf(stdout, "COLONI DE FOURMI: %f\n", resolution_ant_colony(min_dist, N, &best_tour));
    end = clock();
    millis = (end -  begin) * 1000 / CLOCKS_PER_SEC;
    zlog(stdout, INFO, "COLONI DE FOURMI: Finished in %ld ms", millis);

    free(best_tour);

    return 0;
}

int parallel_treatment_recuit_simule(void * parameters){
    float ** min_dist = (float **) parameters;

    unsigned long millis;
    clock_t begin;
    clock_t end;

    begin = clock();
    fprintf(stdout, "RECUIT SIMULE: %f\n", resolution_recuis_simule(min_dist, N, &utils_descente_geometrique));
    end = clock();
    millis = (end -  begin) * 1000 / CLOCKS_PER_SEC;
    zlog(stdout, INFO, "RECUIT SIMULE: Finished in %ld ms", millis);

    return 0;
}

int parallel_treatment_glouton(void * parameters){
    float ** min_dist = (float **) parameters;

    unsigned long millis;
    clock_t begin;
    clock_t end;
    
    begin = clock();
    fprintf(stdout, "GLOUTON EXH: %f\n", glouton_exhaustive(min_dist, N));
    end = clock();
    millis = (end -  begin) * 1000 / CLOCKS_PER_SEC;
    zlog(stdout, INFO, "GLOUTON EXH: Finished in %ld ms", millis);

    return 0;
}

int thread_main()
{
    thrd_t thread_handle_a;
    thrd_t thread_handle_b;
    thrd_t thread_handle_c;
    thrd_t thread_handle_d;

    int error_code_of_thread_a = 0;
    int error_code_of_thread_b = 0;
    int error_code_of_thread_c = 0;
    int error_code_of_thread_d = 0;

    /* int n = generate_random_number(N_MIN, N_MAX); */
    int n = N;

    float ** min_dist;
    
    graph_sdl_t * gs = graph_initialize_graph_sdl(n, 800, 800, 0.8, 0.5);

    (void) thread_handle_d;
    (void) error_code_of_thread_d;

    floydWarshall(gs, &min_dist);
    
    thrd_create(&thread_handle_a, parallel_treatment_glouton, min_dist);
    thrd_create(&thread_handle_b, parallel_treatment_recuit_simule, min_dist);
    thrd_create(&thread_handle_c, parallel_treatment_ant_colony, min_dist);
    /* thrd_create(&thread_handle_d, parallel_treatment_genetique, min_dist); */

    thrd_join(thread_handle_a, &error_code_of_thread_a);
    thrd_join(thread_handle_b, &error_code_of_thread_b);
    thrd_join(thread_handle_c, &error_code_of_thread_c);
    /* thrd_join(thread_handle_d, &error_code_of_thread_d); */

    graph_free_graph_sdl(gs);
    free_matrix_float(min_dist, n);
    
    return 0;
}
