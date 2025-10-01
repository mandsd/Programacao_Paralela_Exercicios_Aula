/*
Exercício 2 — Paralelizando um for simples
a) Crie um vetor v de tamanho 100 e inicialize todos os elementos com o valor 1.
b) Escreva um loop sequencial que soma todos os elementos.
c) Refaça o loop com #pragma omp parallel for reduction(+:soma).
d) Compare os resultados e explique por que a diretiva reduction é necessária.

Aluna: Amanda Ferreira Dahm
Matrícula: 2422130022
Disciplina: Programação Paralela
Professor: Rodrigo Gonçalves Pinto
*/

#include <stdio.h>
#include <omp.h>

int main(void) {
    /* Constante com tamanho do vetor */
    const int N = 100;

    /* Array estático alocado na área de dados do programa (não na pilha) */
    int v[N];

    /* Inicializa todos elementos com 1:
       - for i = 0..N-1: v[i] = 1
       Esta inicialização é sequencial e determinística
     */
    for (int i = 0; i < N; ++i) v[i] = 1;

    /* Soma sequencial:
       - Soma_seq acumula o total percorrendo todas as posições do vetor
       - Tipo long é usado para maior segurança em casos de N grande
     */
    long soma_seq = 0;
    for (int i = 0; i < N; ++i) soma_seq += v[i];

    /* Soma paralela:
       - Declaramos soma_par fora do pragma para que depois do reduction
         possamos imprimir o resultado aqui
       - A diretiva 'parallel for reduction(+:soma_par)' faz:
           * cria cópia privada de soma_par para cada thread, inicializada a 0;
           * cada thread acumula nas suas cópias privadas durante as iterações
             atribuídas a ela;
           * ao final, o runtime combina (faz soma) das cópias privadas e
             armazena o resultado em soma_par
     */
    long soma_par = 0;
    #pragma omp parallel for reduction(+:soma_par)
    for (int i = 0; i < N; ++i) {
        soma_par += v[i];
    }

    /* Impressão dos resultados */
    printf("Soma sequencial = %ld\n", soma_seq);
    printf("Soma paralela   = %ld\n", soma_par);

    /* Explicação adicional:
       - Sem reduction, se várias threads atualizassem soma_par simultaneamente,
         teríamos condição de corrida: operações de leitura/escrita intercaladas
         e resultado incorreto
     */

    return 0;
}
