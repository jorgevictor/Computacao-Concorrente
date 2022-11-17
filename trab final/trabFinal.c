#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(int argc, char*argv[]) {
   float *matriz; //matriz que será gerada
   float *vetor;
   float *matrizFinal;
   int linhas; //dimensoes da matriz
   long long int tam; //qtde de elementos na matriz
   
   //recebe o argumento de entrada
   if(argc < 2) {
      fprintf(stderr, "Digite: %s <variaveis>\n", argv[0]);
      return 1;
   }
   linhas = atoi(argv[1]);
   tam = linhas * linhas;

   //aloca memoria para a matriz
   matriz = (float*) malloc(sizeof(float) * tam);
   if(!matriz) {
      fprintf(stderr, "Erro de alocao da memoria da matriz\n");
      return 2;
   }

   //aloca memoria para o vetor
   vetor = (float*) malloc(sizeof(float) * linhas);
   if(!matriz) {
      fprintf(stderr, "Erro de alocao da memoria do vetor\n");
      return 2;
   }

   //aloca memoria para a matriz final
   matrizFinal = (float*) malloc(sizeof(float) * tam);
   if(!matrizFinal) {
      fprintf(stderr, "Erro de alocao da memoria da matriz final \n");
      return 2;
   }

   //preenche a matriz com valores float aleatorios
   //randomiza a sequencia de numeros aleatorios
   srand(time(NULL));
   for(long int i=0; i<tam; i++) {
      *(matriz+i) = (rand() % 10);
   }

   for(long int i = 0; i < linhas; i++){
      *(vetor+i) = (rand() % 10);
   }

   for(long int i = 0; i < linhas; i++){
      fprintf(stdout, "%.0f ", vetor[i]);
   }

   fprintf(stdout, "\n");
   fprintf(stdout, "-------------------------------------");
   fprintf(stdout, "\n");

   for(int i=0; i<linhas; i++) {
      for(int j=0; j<linhas; j++)
        fprintf(stdout, "%.0f ", matriz[i*linhas+j]);
      fprintf(stdout, "\n");
   }

   //multiplicacao da matriz pelo vetor
   for(int i = 0; i < linhas; i++){
      for(int j = 0; j < linhas; j++){ 
         matrizFinal[i] += matriz[i*(linhas) + j] * vetor[j];
      }
   }

   fprintf(stdout, "\n");
   fprintf(stdout, "-------------------------------------");
   fprintf(stdout, "\n");

   for(long int i = 0; i < linhas; i++){
      fprintf(stdout, "%.0f ", matrizFinal[i]);
   }

   free(matriz);
   return 0;
}