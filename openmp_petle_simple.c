#include <stdio.h>
#include <omp.h>

#define SIZE 18
#define THREADS_AMOUNT 4

int main() {
    double a[SIZE];

    omp_set_num_threads(THREADS_AMOUNT);

#pragma omp parallel for default(none) shared(a)
    for (int i = 0; i < SIZE; i++) a[i] = 1.02 * i;

    // sequential loop
    double suma = 0.0;
    for (int i = 0; i < SIZE; i++) {
        suma += a[i];
    }

    printf("The sum of the array's terms: %lf\n", suma);

    // parallel loop

    double suma_parallel = 0.0;
#pragma omp parallel for default(none) reduction(+:suma_parallel) shared(a) ordered

    for (int i = 0; i < SIZE; i++) {
        int threadId = omp_get_thread_num();
        suma_parallel += a[i];

#pragma omp ordered
        printf("a[%2d]->W_%1d  \n", i, threadId);
    }

    printf("\nSum of array terms in parallel (with clause - schedule(static): %lf\n\n", suma_parallel);

    suma_parallel = 0.0;

#pragma omp parallel for schedule(dynamic) default(none) ordered shared(a) reduction(+ : suma_parallel)
    for (int i = 0; i < SIZE; i++) {
        int id_w = omp_get_thread_num();
        suma_parallel += a[i];

#pragma omp ordered
        printf("a[%2d]->W_%1d  \n", i, id_w);
    }

    if (suma - suma_parallel > 1e-9 || suma - suma_parallel < -1e-9)
        printf("Sum counted incorrectly!\nsequential: %lf\nparallel: %lf\n", suma, suma_parallel);

    printf("\nSum of array terms in parallel (with clause - schedule(dynamic): %lf\n", suma_parallel);
}