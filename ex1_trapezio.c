/* Nome: Gabriel Taka - RA: 10723533
   Ex.1 - Regra do Trapezio com OpenMP (evolucao ate reduction)
   Compilar: gcc -fopenmp ex1_trapezio.c -o ex1 -lm
   Executar: ./ex1 <num_threads> */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

double f(double x) { return x * x; }

/* Versao 1: serial */
double trap_serial(double a, double b, long n) {
    double h = (b - a) / n, soma = (f(a) + f(b)) / 2.0;
    for (long i = 1; i < n; i++) soma += f(a + i * h);
    return soma * h;
}

/* Versao 2: paralela com critical */
double trap_critical(double a, double b, long n, int nt) {
    double h = (b - a) / n, total = 0.0;
    #pragma omp parallel num_threads(nt)
    {
        int id = omp_get_thread_num(), p = omp_get_num_threads();
        long local_n = n / p, ini = id * local_n;
        long fim = (id == p - 1) ? n : ini + local_n;
        double local = 0.0;
        for (long i = ini; i < fim; i++) {
            double x0 = a + i * h, x1 = x0 + h;
            local += (f(x0) + f(x1)) * h / 2.0;
        }
        #pragma omp critical
        total += local;
    }
    return total;
}

/* Versao 3: parallel for + reduction  */
double trap_reduction(double a, double b, long n, int nt) {
    double h = (b - a) / n, total = 0.0;
    #pragma omp parallel for num_threads(nt) reduction(+:total)
    for (long i = 0; i < n; i++) {
        double x0 = a + i * h, x1 = x0 + h;
        total += (f(x0) + f(x1)) * h / 2.0;
    }
    return total;
}

int main(int argc, char **argv) {
    int nt = (argc > 1) ? atoi(argv[1]) : 4;
    double a = 0.0, b = 3.0; long n = 100000000L;
    double t, r;

    t = omp_get_wtime(); r = trap_serial(a, b, n);
    printf("Serial    : %.10f  tempo %.4fs\n", r, omp_get_wtime() - t);
    t = omp_get_wtime(); r = trap_critical(a, b, n, nt);
    printf("Critical  : %.10f  tempo %.4fs (%d threads)\n", r, omp_get_wtime() - t, nt);
    t = omp_get_wtime(); r = trap_reduction(a, b, n, nt);
    printf("Reduction : %.10f  tempo %.4fs (%d threads)\n", r, omp_get_wtime() - t, nt);
    return 0;
}
