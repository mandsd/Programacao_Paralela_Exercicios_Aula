/*
Exercício 4 — Medindo tempo por thread
Adapte o código do exercício anterior para:
a) Medir e exibir o tempo total de execução.
b) Medir e exibir o tempo gasto por cada thread.
c) Mostrar quantas threads foram utilizadas no cálculo.

Aluna: Amanda Ferreira Dahm
Matrícula: 2422130022
Disciplina: Programação Paralela
Professor: Rodrigo Gonçalves Pinto
*/

#include <stdio.h>
#include <omp.h>

#define N 1000000       // Tamanho dos vetores
#define MAX_THREADS 8   // Número máximo de threads
#define REPEATS 100     // Número de repetições para aumentar o tempo de medição

/* Vetores globalmente alocados */
static double x[N];
static double y[N];
static double z[N];
static double a[N];

/* Lista de números de threads a testar */
static const int THREADS_TO_TEST[] = {1, 2, 4, 8};
static const int NUM_THREAD_OPTIONS = sizeof(THREADS_TO_TEST)/sizeof(int);

int main(void) {

    /* Inicialização sequencial dos vetores */
    for (int i = 0; i < N; ++i) {
        x[i] = (double)i * 1e-3;
        y[i] = (double)(N - i) * 1e-3;
        z[i] = (double)(i % 100) * 1e-3;
    }

    /* Cabeçalho da tabela de resultados */
    printf("%10s %15s %12s %30s\n", 
           "threads", "tempo total (s)", "speedup", "tempos por thread (s)");

    /* Tempo sequencial */
    double t_seq_start = omp_get_wtime();
    for (int r = 0; r < REPEATS; r++) {
        for (int i = 0; i < N; ++i) {
            a[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];
        }
    }
    double t_seq = omp_get_wtime() - t_seq_start;

    /* Soma de checagem para evitar otimização */
    double check_seq = a[0] + a[N/2] + a[N-1];
    (void)check_seq;

    /* Testa diferentes números de threads */
    for (int opt = 0; opt < NUM_THREAD_OPTIONS; ++opt) {
        int nth = THREADS_TO_TEST[opt];
        omp_set_num_threads(nth);

        double thread_times[MAX_THREADS] = {0.0}; // Inicializa array fixo

        double t_par_start = omp_get_wtime();

        /* Região paralela */
        #pragma omp parallel
        {
            int tid = omp_get_thread_num();
            double t_start = omp_get_wtime();

            #pragma omp for schedule(static)
            for (int r = 0; r < REPEATS; r++) {
                for (int i = 0; i < N; ++i) {
                    a[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];
                }
            }

            double t_end = omp_get_wtime();
            thread_times[tid] = t_end - t_start;
        } // Fim da região paralela

        double t_par_total = omp_get_wtime() - t_par_start;

        /* Checagem para garantir que o cálculo não foi otimizado */
        double check_par = a[0] + a[N/2] + a[N-1];
        (void)check_par;

        /* Calcula speedup */
        double speedup = t_par_total > 0.0 ? t_seq / t_par_total : 0.0;

        /* Impressão dos resultados */
        printf("%10d %15.6f %12.4f ", nth, t_par_total, speedup);
        for (int i = 0; i < nth; ++i) {
            printf("%6.4f ", thread_times[i]);
        }
        printf("\n");
    }

    return 0;
}
