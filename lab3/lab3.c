/* Multiplicacao de matriz-vetor (considerando matrizes quadradas) */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

float *matrizA;
float *matrizB;
float *matrizFinal;
int nthreads;
pthread_t *tid;

typedef struct{
   int id; 
   int dim;
} tArgs;

//funcao que as threads executarao
void * tarefa(void *arg) { 
    tArgs *args = (tArgs*) arg;
    for(int i = args->id; i < args->dim; i += nthreads) {
        for(int j = 0; j < args->dim; j++){
            long long int acc = 0;
            for(int k = 0; k < args->dim; k++){
                acc += matrizA[i * args->dim + k] * matrizB[k * args->dim + j];
            }
            matrizFinal[i * args->dim + j] = acc;
        }
    }
    pthread_exit(NULL);
}

//fluxo principal
int main(int argc, char* argv[]) {
    int linhasA, colunasA, linhasB, colunasB;
    long long int tam;
    FILE *arquivoSaidaA = fopen(argv[1], "rb");
    FILE *arquivoSaidaB = fopen(argv[2], "rb");
    FILE *arquivoSaidaC = fopen(argv[3], "wb");
    // int dim = linhasA;
    pthread_t *tid;
    tArgs *args;
    double inicio, fim, delta, delta2, delta3;

    GET_TIME(inicio);

    // Verificacao de parametros
    if (argc < 2){
        fprintf(stderr, "INPUT INVÁLIDO!\nPor favor, digite %s <Arquivo de entrada 1> <Arquivo de entrada 2> <Arquivo de saída>\n", argv[0]);
        return 1;
    }

    if (!arquivoSaidaA){
        fprintf(stderr, "Falha ao abrir o primeiro arquivo!\n");
        return 2;
    }

    if (!arquivoSaidaB){
        fprintf(stderr, "Falha ao abrir o segundo arquivo!\n");
        return 2;
    }


    //alocacao de memoria para as estruturas de dados
    fread(&linhasA, sizeof(int), 1, arquivoSaidaA); 
    fread(&colunasA, sizeof(int), 1, arquivoSaidaA);

    tam = linhasA;
    matrizA = (float *) malloc(sizeof(float) * tam * tam);
    if (matrizA == NULL) {printf("ERRO--malloc1\n"); return 2;}
    fclose(arquivoSaidaA);

    fread(&linhasB, sizeof(int), 1, arquivoSaidaB);
    fread(&colunasB, sizeof(int), 1, arquivoSaidaB);
    
    tam = linhasB;
    matrizB = (float *) malloc(sizeof(float) * tam * tam);
    if (matrizB == NULL) {printf("ERRO--malloc2\n"); return 2;}
    fclose(arquivoSaidaB);

    matrizFinal = (float *) malloc(sizeof(float) * tam * tam);
    if (matrizFinal == NULL) {printf("ERRO--malloc3\n"); return 2;}

    nthreads = atoi(argv[4]);

    tid = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);
    if(tid==NULL) {puts("ERRO--malloc4"); return 2;}
    args = (tArgs*) malloc(sizeof(tArgs) * nthreads);
    if(args==NULL) {puts("ERRO--malloc5"); return 2;}

    GET_TIME(fim);
    delta = fim - inicio;

    GET_TIME(inicio);

    //criacao das threads
    for(int i = 0; i < nthreads; i++) {
        (args+i)->id = i;
        (args+i)->dim = tam;
        if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))){
            puts("ERRO--pthread_create"); return 3;
        }
    } 

    //espera pelo termino da threads
    for(int i = 0; i < nthreads; i++) {
        pthread_join(*(tid+i), NULL);
    }

    GET_TIME(fim);
    delta2 = fim - inicio;

    GET_TIME(inicio);

    fwrite(&linhasA, sizeof(int), 1, arquivoSaidaC);
    fwrite(&colunasB, sizeof(int), 1, arquivoSaidaC);

    fwrite(matrizFinal, sizeof(float), tam, arquivoSaidaC);

    //liberacao da memoria
    fclose(arquivoSaidaC);
    free(matrizA);
    free(matrizB);
    free(matrizFinal);
    free(args);
    free(tid);

    GET_TIME(fim);
    delta3 = fim - inicio;

    printf("Inicializacao:  %f s\n", delta);
    printf("Processamento:  %f s\n", delta2);
    printf("Finalizacao:    %f s\n", delta3);

    return 0;
}