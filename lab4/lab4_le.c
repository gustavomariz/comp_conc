/* Disciplina: Programacao Concorrente */
/* Nome: Gustavo Vilares Mariz de Oliveira */
/* DRE: 121073784 */
/* Laboratório: 4*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "list_int.h"
#include "timer.h"


int leit=0, escr=0; //globais


pthread_mutex_t mutex;
pthread_cond_t cond_leit; //definindo cond de leitura
pthread_cond_t cond_escr; //definindo cond de escrita
 

//Confirma a entrada de uma leitura
void EntraLeitura() {

   pthread_mutex_lock(&mutex);

   while(escr > 0) {
      pthread_cond_wait(&cond_leit, &mutex);
   }
   leit++;

   pthread_mutex_unlock(&mutex);
}

//Confirma a saída de uma leitura
void SaiLeitura() {
   pthread_mutex_lock(&mutex);
   leit--;
   if(leit==0) pthread_cond_signal(&cond_escr);
   pthread_mutex_unlock(&mutex);
}

//Tenta realizar uma escrita condicionada
void EntraEscrita () {

   pthread_mutex_lock(&mutex);

   while((leit>0) || (escr>0)) {
   pthread_cond_wait(&cond_escr, &mutex);
   }
   escr++;

   pthread_mutex_unlock(&mutex);
}

//Finaliza a escrita e libera 
void SaiEscrita () {
   pthread_mutex_lock(&mutex);
   escr--;
   pthread_cond_signal(&cond_escr);
   pthread_cond_broadcast(&cond_leit);
   pthread_mutex_unlock(&mutex);
}


//Reaproveitando a main do programa visto em sala


#define QTDE_OPS 10000000 //quantidade de operacoes sobre a lista (insercao, remocao, consulta)
#define QTDE_INI 100 //quantidade de insercoes iniciais na lista
#define MAX_VALUE 100 //valor maximo a ser inserido


//lista compartilhada iniciada
struct list_node_s* head_p = NULL;
//qtde de threads no programa
int nthreads;
//lock de exclusao mutua
pthread_mutex_t mutex;


//tarefa das threads
void* tarefa(void* arg) {
  long int id = (long int) arg;
  int op;
  int in, out, read;
  in=out=read = 0;


  //realiza operacoes de consulta (98%), insercao (1%) e remocao (1%)
  //Nesse trecho substituímos os locks ou rwlocks pelas funções elaboradas para cada situação de entrada ou saída
  for(long int i=id; i<QTDE_OPS; i+=nthreads) {
     op = rand() % 100;
     if(op<98) {
        EntraLeitura();    
        Member(i%MAX_VALUE, head_p);   /* Ignore return value */
        SaiLeitura();   
    read++;
     } else if(98<=op && op<99) {
        EntraEscrita();    
        Insert(i%MAX_VALUE, &head_p);  /* Ignore return value */
        SaiEscrita(); 
    in++;
     } else if(99<=op) {
        EntraEscrita();   
        Delete(i%MAX_VALUE, &head_p);  /* Ignore return value */
        SaiEscrita();    
    out++;
     }
  }
  //registra a qtde de operacoes realizadas por tipo
  printf("Thread %ld: in=%d out=%d read=%d\n", id, in, out, read);
  pthread_exit(NULL);
}


/*-----------------------------------------------------------------*/
int main(int argc, char* argv[]) {
  pthread_t *tid;
  double ini, fim, delta;
 
  //verifica se o numero de threads foi passado na linha de comando
  if(argc<2) {
     printf("Digite: %s <numero de threads>\n", argv[0]); return 1;
  }
  nthreads = atoi(argv[1]);


  //insere os primeiros elementos na lista
  for(int i=0; i<QTDE_INI; i++)
     Insert(i%MAX_VALUE, &head_p);  /* Ignore return value */
 
  //tomada de tempo inicial
  GET_TIME(ini);


  //aloca espaco de memoria para o vetor de identificadores de threads no sistema
  tid = malloc(sizeof(pthread_t)*nthreads);
  if(tid==NULL) { 
     printf("--ERRO: malloc()\n"); return 2;
  }


  //inicializa a variavel mutex
  pthread_mutex_init(&mutex, NULL);
 
  //cria as threads
  for(long int i=0; i<nthreads; i++) {
     if(pthread_create(tid+i, NULL, tarefa, (void*) i)) {
        printf("--ERRO: pthread_create()\n"); return 3;
     }
  }
 
  //aguarda as threads terminarem
  for(int i=0; i<nthreads; i++) {
     if(pthread_join(*(tid+i), NULL)) {
        printf("--ERRO: pthread_join()\n"); return 4;
     }
  }


  //tomada de tempo final
  GET_TIME(fim);
  delta = fim-ini;
  printf("Tempo: %lf\n", delta);


  //libera o mutex
  pthread_mutex_destroy(&mutex);
  //libera o espaco de memoria do vetor de threads
  free(tid);
  //libera o espaco de memoria da lista
  Free_list(&head_p);


  return 0;
}  /* main */
