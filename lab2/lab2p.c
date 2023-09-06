//Disciplina: Programacao Concorrente 
 //Nome: Gustavo Vilares Mariz de Oliveira 
 //DRE: 121073784
 //Laboratório: 2 

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <math.h>
#include "timer.h"

int ehPrimo(long long int n) {
    int i;
    if (n<=1) return 0;
    if (n==2) return 1;
    if (n%2==0) return 0;
    for (i=3; i<sqrt(n)+1; i+=2){
        if(n%i==0) return 0;
    }
    return 1;
}

pthread_mutex_t mutex;
int primos_total = 0;
int global_index = 1;

void* checkPrimos(void* arg) {
    long long int n = (long long int) arg;
    while (1) {
        int local_index;
        pthread_mutex_lock(&mutex);
        local_index = global_index++;
        pthread_mutex_unlock(&mutex);

        if (local_index > n) break;

        if (ehPrimo(local_index)) {
            pthread_mutex_lock(&mutex);
            primos_total++;
            pthread_mutex_unlock(&mutex);
        }
    }

    pthread_exit(NULL);
}

void verifica(n, valor){
    int cnt = 0;
    for(int i = 0; i<n ; i++){
        if(ehPrimo(i)){cnt++;}
    }
    if(cnt==valor){printf("Tudo certo!\n");}
    else{printf("Contou os primos errado!\n");}
}

int main(int argc, char *argv[]) {

    long long int N = atoll(argv[1]);
    int NUM_THREADS = atoi(argv[2]);

    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&mutex, NULL);
    double inicio, fim, delta;

    GET_TIME(inicio);

    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, checkPrimos, (void *)N)) {
        printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    GET_TIME(fim);
    delta = fim - inicio;

    verifica(N, primos_total);

    printf("Total de números primos encontrados de 1 a %d: %d\n", N, primos_total);
    printf("Foram necessários %f segundos.", delta);
    return 0;
}
