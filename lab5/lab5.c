/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 4 */
/* Codigo: Uso de variáveis de condição e suas operações básicas para sincronização por condição */

/***** Condicao logica da aplicacao: a thread B so podera imprimir a sentenca X=... quando x for multiplo de 10, e devera fazer isso uma unica vez e depois terminar sua execucao  ****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 3

/* Variaveis globais */
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

/* Thread 1 */
void *um(void *t){
    pthread_mutex_lock(&x_mutex);
    while (x < 3)
    {
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    printf("Volte sempre!\n");
    pthread_mutex_unlock(&x_mutex);

    pthread_exit(NULL);
}

/* Thread 2 */
void *dois(void *t){
    pthread_mutex_lock(&x_mutex);
    if (x == 0)
    {
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    printf("Sente-se por favor.\n");
    x++;
    pthread_cond_signal(&x_cond);
    pthread_mutex_unlock(&x_mutex);

    pthread_exit(NULL);
}

/* Thread 3 */
void *tres(void *t){
    pthread_mutex_lock(&x_mutex);
    if (x == 0)
    {
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    printf("Fique a vontade.\n");
    x++;
    pthread_cond_signal(&x_cond);
    pthread_mutex_unlock(&x_mutex);

    pthread_exit(NULL);
}

/* Threads 4 */
void *quatro(void *t){
    pthread_mutex_lock(&x_mutex);
    printf("Seja bem-vindo!\n");
    x++;
    pthread_cond_broadcast(&x_cond);
    pthread_mutex_unlock(&x_mutex);

    pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]){
    int i;
    int *t1, *t2, *t3, *t4;
    pthread_t threads[NTHREADS];
    // aloca espaco para os identificadores das threads
    t1 = malloc(sizeof(int));
    t2 = malloc(sizeof(int));
    t3 = malloc(sizeof(int));
    t4 = malloc(sizeof(int));
    *t1 = 1, *t2 = 2, *t3 = 3;
    *t4 = 4;

    /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init(&x_cond, NULL);

    /* Cria as threads */
    pthread_create(&threads[1], NULL, dois, (void *)t2);
    pthread_create(&threads[2], NULL, tres, (void *)t3);
    pthread_create(&threads[3], NULL, quatro, (void *)t4);
    pthread_create(&threads[0], NULL, um, (void *)t1);

    /* Espera todas as threads completarem */
    for (i = 0; i < NTHREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    printf("FIM.\n");

    /* Desaloca variaveis e termina */
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);

    free(t1);
    free(t2);
    free(t3);
    free(t4);

}