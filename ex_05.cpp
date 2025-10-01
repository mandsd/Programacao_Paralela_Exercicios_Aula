/*
Exercício 5 — Escalonamento
Use novamente o cálculo de a[i] = x[i]^2 + y[i]^2 + z[i]^2, mas:
a) Execute com schedule(static) e schedule(dynamic, 1000).
b) Compare os tempos em diferentes quantidades de threads (2, 4, 8).
c) Explique em quais situações static e dynamic são mais adequados.

Aluna: Amanda Ferreira Dahm
Matrícula: 2422130022
Disciplina: Programação Paralela
Professor: Rodrigo Gonçalves Pinto
*/

#include <stdio.h>
#include <omp.h>

#define N 1000000       // Tamanho dos vetores
#define MAX_THREADS 8
#define REPEATS 100     // Repetições para aumentar tempo de medição

/* Vetores globais */
static double x[N];
static double y[N];
static double z[N];
static double a[N];

/* Lista de threads a testar */
static const int THREADS_TO_TEST[] = {2, 4, 8};
static const int NUM_THREAD_OPTIONS = sizeof(THREADS_TO_TEST)/sizeof(int);

int main(void) {

    /* Inicialização dos vetores */
    for (int i = 0; i < N; ++i) {
        x[i] = (double)i * 1e-3;
        y[i] = (double)(N - i) * 1e-3;
        z[i] = (double)(i % 100) * 1e-3;
    }

    printf("%10s %15s %15s\n", "threads", "static(s)", "dynamic(s)");

    /* Loop sobre cada número de threads */
    for (int opt = 0; opt < NUM_THREAD_OPTIONS; ++opt) {
        int nth = THREADS_TO_TEST[opt];
        omp_set_num_threads(nth); // define número de threads

        double t_static_start = omp_get_wtime();

        /* Schedule static */
        #pragma omp parallel for schedule(static)
        for (int r = 0; r < REPEATS; r++) {
            for (int i = 0; i < N; ++i) {
                a[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];
            }
        }

        double t_static = omp_get_wtime() - t_static_start;

        /* Schedule dynamic com chunk 1000 */
        double t_dynamic_start = omp_get_wtime();

        #pragma omp parallel for schedule(dynamic, 1000)
        for (int r = 0; r < REPEATS; r++) {
            for (int i = 0; i < N; ++i) {
                a[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];
            }
        }

        double t_dynamic = omp_get_wtime() - t_dynamic_start;

        /* Impressão dos tempos */
        printf("%10d %15.6f %15.6f\n", nth, t_static, t_dynamic);
    }

    return 0;
}
