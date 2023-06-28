#include "parallel.h"

int treatment(void * parameters) {
    int *p = (int*) parameters;  // p is now the address of an int, and not the address of a "???"
    printf("Compute with value: %d.\n", *p);

    return 0;
}

int treatment2(void * parameters){
    float p = *((float*) parameters); // the value starting at the address 'parameters' is interpreted as a float
    p = p * p - 1;
    printf("I have computed with this value: %f.\n", p);
    return 0;
}

int thread_main() {
    thrd_t thread_handle_a;
    int a = 0;

    thrd_t thread_handle_b;
    float b = 2;

    int error_code_of_thread_a = 0;
    int error_code_of_thread_b = 0;
    
    thrd_create(&thread_handle_a, treatment, &a);
    thrd_create(&thread_handle_b, treatment2, &b);

    thrd_join(thread_handle_a, &error_code_of_thread_a);
    thrd_join(thread_handle_b, &error_code_of_thread_b);
    return 0;
}
