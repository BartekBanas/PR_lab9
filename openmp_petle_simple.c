#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define WYMIAR 18
#define THREADS_AMOUNT 4


int main() {
    double a[WYMIAR];

    omp_set_num_threads(THREADS_AMOUNT);

#pragma omp parallel for default(none) shared(a)
    for (int i = 0; i < WYMIAR; i++) a[i] = 1.02 * i;

    // pętla sekwencyjna
    double suma = 0.0;
    for (int i = 0; i < WYMIAR; i++) {
        suma += a[i];
    }

    printf("Suma wyrazow tablicy: %lf\n", suma);

    // pętla do modyfikacji - docelowo równoległa w OpenMP

    double suma_parallel = 0.0;
    // ...
#pragma omp parallel for default(none) reduction(+:suma_parallel) shared(a) ordered schedule(static)

    for (int i = 0; i < WYMIAR; i++) {
        int id_w = omp_get_thread_num();
        // ...
        suma_parallel += a[i];
        //printf("a[%d] = %lf\n", i, a[i]);
        // ...
#pragma omp ordered

        printf("a[%2d]->W_%1d  \n", i, id_w);
    }

    printf("\nSuma wyrazow tablicy rownolegle (z klauzula - schedule(static): %lf\n", suma_parallel);

    printf("Suma wyrazow tablicy: %lf\n\n", suma);

    // pętla do modyfikacji - docelowo równoległa w OpenMP
    suma_parallel=0.0;
#pragma omp parallel for schedule(dynamic) default(none) ordered shared(a) reduction(+ : suma_parallel)
    // ...
    for(int i=0;i<WYMIAR;i++) {
        int id_w = omp_get_thread_num();
        // ...
        suma_parallel += a[i];
        // ...
#pragma omp ordered
        printf("a[%2d]->W_%1d  \n",i,id_w);
    }

    //printf("\nSuma wyrazów tablicy równolegle (z klauzulą - ....: %lf\n", suma_parallel);
    if (suma - suma_parallel > 1e-9 || suma - suma_parallel < -1e-9)
        printf("suma policzona nie poprawnie!\nsekwencyjnie: %lf\nrownolegle: %lf\n",suma, suma_parallel);

    printf("\nSuma wyrazow tablicy rownolegle (z klauzula - schedule(dynamic): %lf\n", suma_parallel);

    printf("Suma wyrazow tablicy: %lf\n\t", suma);
}
