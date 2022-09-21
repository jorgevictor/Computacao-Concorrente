/* Multiplicacao de matriz-vetor (considerando matrizes quadradas) */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

//fluxo principal
int main(int argc, char* argv[]) {
    float *matrizA;
    float *matrizB;
    float *matrizFinal;
    int linhasA, colunasA, linhasB, colunasB;
    long long int tam;
    FILE *arquivoSaidaA = fopen(argv[1], "rb");
    FILE *arquivoSaidaB = fopen(argv[2], "rb");
    FILE *arquivoSaidaC = fopen(argv[3], "wb");
    //size_t ret;
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

    tam = colunasA * linhasA;
    matrizA = (float *) malloc(sizeof(float) * tam);
    if (matrizA == NULL) {printf("ERRO--malloc\n"); return 2;}
    fclose(arquivoSaidaA);

    fread(&linhasB, sizeof(int), 1, arquivoSaidaB);
    fread(&colunasB, sizeof(int), 1, arquivoSaidaB);
    
    tam = colunasB * linhasB;
    matrizB = (float *) malloc(sizeof(float) * tam);
    if (matrizB == NULL) {printf("ERRO--malloc\n"); return 2;}
    fclose(arquivoSaidaB);

    matrizFinal = (float *) malloc(sizeof(float) * tam);
    if (matrizFinal == NULL) {printf("ERRO--malloc\n"); return 2;}

    GET_TIME(fim);
    delta = fim - inicio;

    GET_TIME(inicio);

    //multiplicacao das matrizes
    for(int i = 0; i < linhasA; i++) {
        for(int j = 0; j < colunasB; j++){
            long long int acc = 0;
            for(int k = 0; k < linhasA; k++){
                acc += matrizA[i * linhasA + k] * matrizB[k * linhasA + j];
            }
            matrizFinal[i * linhasA + j] = acc;
        }
    }

    GET_TIME(fim);
    delta2 = fim - inicio;

    GET_TIME(inicio);

    fwrite(&linhasA, sizeof(int), 1, arquivoSaidaC);
    fwrite(&colunasB, sizeof(int), 1, arquivoSaidaC);

    fwrite(matrizFinal, sizeof(float), tam, arquivoSaidaC);


   //exibicao dos resultados
   /*puts("Vetor de saida:");
   for(int j=0; j<dim; j++)
      printf("%.1f ", saida[j]);
   puts("");
   */

   //liberacao da memoria
    fclose(arquivoSaidaC);
    free(matrizA);
    free(matrizB);
    free(matrizFinal);

    GET_TIME(fim);
    delta3 = fim - inicio;

    printf("Inicializacao:  %f s\n", delta);
    printf("Processamento:  %f s\n", delta2);
    printf("Finalizacao:    %f s\n", delta3);

    return 0;
}