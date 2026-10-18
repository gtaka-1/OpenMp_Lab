/* Nome:Gabriel Taka - RA: 10723533
   Ex.2 - Soma de vetor - versao REDUCTION
   Compilar: gcc -O2 -fopenmp soma_reduction.c -o soma_reduction
   Executar: ./soma_reduction <N> <threads> */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <omp.h>

int main(int argc, char **argv) {
    long N = atol(argv[1]); int nt = atoi(argv[2]);
    omp_set_num_threads(nt);
    uint8_t *v = malloc(N);
    if (!v) { printf("sem memoria\n"); return 1; }
    for (long i = 0; i < N; i++) v[i] = i % 10; 

    double tempo_total = 0; unsigned long long soma = 0;
    for (int e = 0; e < 10; e++) {
        soma = 0;
        double t = omp_get_wtime();
        #pragma omp parallel for reduction(+:soma)
        for (long i = 0; i < N; i++) soma += v[i];
        tempo_total += omp_get_wtime() - t;
    }
    printf("REDUCTION N=%ld threads=%d soma=%llu tempo_medio=%.6f s\n", N, nt, soma, tempo_total / 10);
    free(v); return 0;
}
