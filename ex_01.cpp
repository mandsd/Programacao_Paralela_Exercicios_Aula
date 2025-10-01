/* Exercício 1 — “Hello World” Paralelo
a) Crie uma região paralela com #pragma omp parallel.
b) Cada thread deve imprimir uma mensagem indicando seu número (omp_get_thread_num()) e o total de threads (omp_get_num_threads()).
c) Configure o programa para rodar com 4 threads.

Aluna: Amanda Ferreira Dahm
Matrícula: 2422130022
Disciplina: Programação Paralela
Professor: Rodrigo Gonçalves Pinto
*/

#include <stdio.h>   /* printf */
#include <omp.h>     /* Todas as funções e diretivas OpenMP */

int main(void) {
    /* Número fixo de threads que queremos usar */
    int nthreads = 4;

    /* omp_set_num_threads(n) solicita ao runtime do OpenMP para usar n threads
       nas próximas regiões paralelas 
     */
    omp_set_num_threads(nthreads);

    /* Paralelizamos um loop com 'parallel for ordered'. A variável de controle
       'i' corre de 0 até nthreads-1; a cláusula 'ordered' permite que
       cada iteração execute a região '#pragma omp ordered' na mesma ordem de i
     
       Ordered funciona com 'parallel for ordered'e garante que as seções '#pragma omp ordered' 
       sejam executadas na ordem de índice i. Sem ordered, as prints acontecem em ordem 
       não-determinística
     */
    #pragma omp parallel for ordered
    for (int i = 0; i < nthreads; ++i) {
        /* Cada thread tem um thread id (tid) fornecido pelo OpenMP.
           tid varia de 0 a nthreads-1
         */
        int tid = omp_get_thread_num();

        /* Recupera quantas threads o runtime forneceu efetivamente para
           esta região paralela
         */
        int total = omp_get_num_threads();

        /* A diretiva ordered garante que a chamada printf abaixo ocorra na
           ordem crescente de 'i' (não do tid). Como queremos ver 1..4, usamos
           tid+1 na impressão (para converter 0-based -> 1-based)
         
           Observação: A ordem é a ordem lógica do loop (i), que coincide
           com a sequência que queremos
         */
        #pragma omp ordered
        printf("Hello from thread %d of %d\n", tid + 1, total);
    }

    return 0;
}

