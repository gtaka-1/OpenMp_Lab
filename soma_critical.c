/* Nome: Gabriel Taka - RA: 10723533
   Ex.2 - Soma de vetor - versao CRITICAL (variavel compartilhada)
   Compilar: gcc -O2 -fopenmp soma_critical.c -o soma_critical
   Executar: ./soma_critical <N> <threads>   (ex.: ./soma_critical 1073741824 4) */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <omp.h>

int main(int argc, char **argv) {
    long N = atol(argv[1]); int nt = atoi(argv[2]);
    omp_set_num_threads(nt);
    uint8_t *v = malloc(N);
    if (!v) { printf("sem memoria\n"); return 1; }
    for (long i = 0; i < N; i++) v[i] = i % 10;   /* dados sempre iguais */

    double tempo_total = 0; unsigned long long soma = 0;
    for (int e = 0; e < 10; e++) {
        soma = 0;
        double t = omp_get_wtime();
        #pragma omp parallel
        {
            unsigned long long parcial = 0;
            #pragma omp for
            for (long i = 0; i < N; i++) parcial += v[i];
            #pragma omp critical
            soma += parcial;
        }
        tempo_total += omp_get_wtime() - t;
    }
    printf("CRITICAL N=%ld threads=%d soma=%llu tempo_medio=%.6f s\n", N, nt, soma, tempo_total / 10);
    free(v); return 0;
}
