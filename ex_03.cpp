/*
Exercício 3 — Expressão Vetorial
Implemente a expressão: a[i] = x[i]^2 + y[i]^2 + z[i]^2 para vetores de tamanho 1.000.000.
a) Faça a versão sequencial.
b) Paralelize com #pragma omp parallel for schedule(static).
c) Mostre o tempo total de execução em cada versão.

Aluna: Amanda Ferreira Dahm
Matrícula: 2422130022
Disciplina: Programação Paralela
Professor: Rodrigo Gonçalves Pinto
*/

#include <stdio.h>
#include <omp.h>

#define N 1000000

/* Vetores globalmente alocados */
static double x[N];
static double y[N];
static double z[N];
static double a[N];

/* Lista de números de threads a testar */
static const int THREADS_TO_TEST[] = {1, 2, 4, 8};
static const int NUM_THREAD_OPTIONS = sizeof(THREADS_TO_TEST)/sizeof(int);

int main(void) {
    /* Inicialização sequencial dos vetores:
       - x[i] = i * 1e-3
       - y[i] = (N - i) * 1e-3
       - z[i] = (i % 100) * 1e-3
       Esses valores são arbitrários, apenas para garantir cálculo distinto
     */
    for (int i = 0; i < N; ++i) {
        x[i] = (double)i * 1e-3;
        y[i] = (double)(N - i) * 1e-3;
        z[i] = (double)(i % 100) * 1e-3;
    }

    /* --- Versão sequencial --- */
    double t_seq_start = omp_get_wtime();
    for (int i = 0; i < N; ++i) {
        a[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];
    }
    double t_seq = omp_get_wtime() - t_seq_start;

    /* Guardamos uma soma de checagem para evitar otimizações agressivas */
    double check_seq = a[0] + a[N/2] + a[N-1];

    printf("Tempo sequencial (1 thread): %f s\n", t_seq);
    printf("Verificacao (soma de 3 elementos): %e\n\n", check_seq);

    /* Cabeçalho da tabela de resultados */
    printf("%10s %15s %12s\n", "threads", "tempo (s)", "speedup");

    /* Testamos cada configuração de threads */
    for (int opt = 0; opt < NUM_THREAD_OPTIONS; ++opt) {
        int nth = THREADS_TO_TEST[opt];

        /* Solicita ao runtime que use 'nth' threads na próxima região paralela */
        omp_set_num_threads(nth);

        /* Mede tempo da versão paralela com schedule static.
           A diretiva '#pragma omp parallel for schedule(static)' divide o loop
           em blocos contíguos entre as threads (baixo overhead)
         */
        double t_par_start = omp_get_wtime();
        #pragma omp parallel for schedule(static)
        for (int i = 0; i < N; ++i) {
            a[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];
        }
        double t_par = omp_get_wtime() - t_par_start;

        /* Checagem para garantir que o cálculo ocorreu */
        double check_par = a[0] + a[N/2] + a[N-1];

        /* Calcula speedup relativo à versão sequencial */
        double speedup = t_seq / t_par;

        printf("%10d %15.6f %12.4f\n", nth, t_par, speedup);

        /* Em geral check_seq e check_par devem ser numericamente iguais */
        (void)check_par; /* Evitamos warning se não usado */
    }

    return 0;
}

