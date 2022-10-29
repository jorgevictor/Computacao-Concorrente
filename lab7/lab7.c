#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5 
#define P 1 
#define C 4 

// Variaveis globais
int Buffer[N];     
sem_t condtInserir, condtRetirar;   // semaforos para exclusao mutua entre as threads
int count = 0, in = 0, out = 0; // variaveis de estado

void iniciaBuffer(){
    for (int i = 0; i < 5; i++){
        Buffer[i] = 0;
    }
}

void ImprimeBuffer(int n)
{
    for (int i = 0; i < n; i++){
        printf("%d ", Buffer[i]);
    }
    printf("\n");
}

void Insere(int item, int id){
    sem_wait(&condtInserir);
    printf("P[%d] quer inserir\n", id);
    for (int i = 0; i < N; i++){
        Buffer[i] = item;
    }
    count += N;
    printf("P[%d] inseriu\n", id);
    ImprimeBuffer(N);
    sem_post(&condtRetirar);
}

void Retira(int id){
    sem_wait(&condtRetirar);
    printf("C[%d] quer consumir\n", id);
    if (count == 0){
        sem_post(&condtInserir);
    }
    count--;
    printf("C[%d] consumiu %d\n", id, Buffer[out]);
    Buffer[out] = 0;
    out = (out + 1) % N;
    ImprimeBuffer(N);
    sem_post(&condtRetirar);
}

void *produtor(void *arg){
    int *id = (int *)arg;
    printf("Sou a thread produtora %d\n", *id);
    while (1){
        Insere(*id, *id);
        sleep(1);
    }
    free(arg);
    pthread_exit(NULL);
}

void *consumidor(void *arg){
    int *id = (int *)arg;
    printf("Sou a thread consumidora %d\n", *id);
    while (1){
        Retira(*id);
        sleep(1); // faz o processamento do item
    }
    free(arg);
    pthread_exit(NULL);
}

//funcao principal
int main(void){

    // identificadores das threads
    pthread_t tid[P + C];
    int *id[P + C];

    // aloca espaco para os IDs das threads
    for (int i = 0; i < P + C; i++)
    {
        id[i] = malloc(sizeof(int));
        if (id[i] == NULL){
            exit(-1);
        }
        *id[i] = i + 1;
    }

    iniciaBuffer(N);

    // inicializa as variaveis de sincronizacao
    sem_init(&condtInserir, 0, 1);
    sem_init(&condtRetirar, 0, 0);

    // cria as threads produtoras
    for (int i = 0; i < P; i++){
        if (pthread_create(&tid[i], NULL, produtor, (void *)id[i])){
            exit(-1);
        }
    }

    // cria as threads consumidoras
    for (int i = 0; i < C; i++){
        if (pthread_create(&tid[i + P], NULL, consumidor, (void *)id[i + P])){
            exit(-1);
        }
    }

    pthread_exit(NULL);
    return 1;
}
