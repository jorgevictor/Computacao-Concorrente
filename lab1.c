#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS 2 

int vector[10000];


void * incrementa10PorCento (void * arg) {
    int ident = * (int *) arg;
    int i = (10000/(ident)) - 1;
    int j = 0;
    while(j != 5000){
        vector[i] += vector[i]/10;
        j++;
        i--;
    }
    pthread_exit(NULL);
}

int main() {

    int c;
    for(c=0; c<10000;c++){
        vector[c] = c;
    }

    pthread_t tid[NTHREADS]; 
    int ident[NTHREADS];
     for(int i=1; i<=NTHREADS; i++) {
       ident[i-1] = i;
       if (pthread_create(&tid[i-1], NULL, incrementa10PorCento, (void *)&ident[i-1])) 
          printf("ERRO -- pthread_create\n");
    }

    for(int i=0; i<NTHREADS; i++) {
       if (pthread_join(tid[i], NULL)) 
          printf("ERRO -- pthread_join\n");
    }

    for(int i=0; i<10000; i++) {
       if (vector[i] != (i + i/10)) {
          printf("ERRO -- incrementa10PorCento\n");
          break;
       }
    }
    
    return 0;
}
