 //Disciplina: Programacao Concorrente 
 //Nome: Gustavo Vilares Mariz de Oliveira 
 //DRE: 121073784
 //Módulo 2 - Laboratório: 1 
 //Codigo: Com 8 threads, elevar ao quadrado cada elemento de um vetor de 10000 elementos. 

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <time.h>

#define NTHREADS  8 //total de threads a serem criadas
#define DIV  125 //divisão dos elementos para cada thread

//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
   int *vetor;
   int div;
} t_Args;

void *square(void *arg){
    t_Args *args = (t_Args *) arg;
    for (int i = args->div; i < (args->div + DIV); i++){
        args->vetor[i] = args->vetor[i] * args->vetor[i]; //eleva ao quadrado o item do vetor
    }
    free(arg);
    pthread_exit(NULL);
}

void verifica(int vetor_normal[1000], int vetor_quadrado[1000]){
    int check = 0;
    int cnt = 0;
    while(cnt < 1000){
        if(vetor_quadrado[cnt] != vetor_normal[cnt] * vetor_normal[cnt]){
            printf("Erro no índice %d", cnt); 
            check = 1; break;
        }
        cnt++;
    }
    if(check == 1){
        printf("ERRO");
    }
    else{
        printf("Todos os números foram elevados ao quadrado corretamente!");
    }
}

int main(void) {

    pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
    t_Args *args;

    int aux[1000];
    int iniciais[1000]; //vetor para salvar os valores iniciais

    //gerando 1000 elementos inteiros aleatórios
    time_t t;
    srand((unsigned) time(&t));
    for(int i = 0; i < 1000 ; i++){
        int r = rand();
        aux[i] = r;
        iniciais[i] = r;   
    }

    //printf("inicio: %d | fim: %d", aux[0], aux[999]); Teste para ver se os números foram gerados
    
    int part = 0;

    for(int i=0; i<NTHREADS; i++) {
        

        args = malloc(sizeof(t_Args));
        if (args == NULL) {
            printf("--ERRO: malloc()\n"); exit(-1);
        }

        //Definindo os argumentos
        args->vetor = aux;
        args->div = part; 
        part += DIV; //Avançar a divisão dos elementos do vetor

        //Cria as threads
        if (pthread_create(&tid_sistema[i], NULL, square, (void*) args)) {
            printf("--ERRO: pthread_create()\n"); exit(-1);
        }
  } 

    //--espera todas as threads terminarem
    for (int i=0; i<NTHREADS; i++) {
        if (pthread_join(tid_sistema[i], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
        } 
    }

    verifica(iniciais, args->vetor); //Realiza a verificação com os valores iniciais e o vetor com seus quadrados

    pthread_exit(NULL);
}