#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 4 
#define Produtor 1 
#define Consumidoras 5 

// Variaveis globais
int Buffer[N];     
sem_t condtInserir, condtRetirar;   // semaforos para exclusao mutua entre as threads
int count = 0, out = 0; // variaveis de estado

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
    for (int i = 0; i < N; i++){
        Buffer[i] = item;
    }
    count += N;
    printf("Produtor[%d] inseriu\n", id);
    ImprimeBuffer(N);
    sem_post(&condtRetirar);
}

void Retira(int id){
    sem_wait(&condtRetirar);
    if (count == 0){
        sem_wait(&condtRetirar);
        sem_post(&condtInserir);
    }
    count--;
    printf("Consumidora[%d] consumiu na posicao %d\n", id, out+1);
    Buffer[out] = 0;
    out = (out + 1) % N;
    ImprimeBuffer(N);
    sem_post(&condtRetirar);
    if (count == 0) 
    {
        sem_wait(&condtRetirar);
        sem_post(&condtInserir);
    }
}

void *produtor(void *arg){
    int *id = (int *)arg;
    while (1){
        Insere(*id, *id);
        sleep(1);
    }
    free(arg);
    pthread_exit(NULL);
}

void *consumidor(void *arg){
    int *id = (int *)arg;
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
    pthread_t tid[Produtor + Consumidoras];
    int *id[Produtor + Consumidoras];

    // aloca espaco para os IDs das threads
    for (int i = 0; i < Produtor + Consumidoras; i++)
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
    for (int i = 0; i < Produtor; i++){
        if (pthread_create(&tid[i], NULL, produtor, (void *)id[i])){
            exit(-1);
        }
    }

    // cria as threads consumidoras
    for (int i = 0; i < Consumidoras; i++){
        if (pthread_create(&tid[i], NULL, consumidor, (void *)id[i])){
            exit(-1);
        }
    }

    pthread_exit(NULL);
    return 1;
}
